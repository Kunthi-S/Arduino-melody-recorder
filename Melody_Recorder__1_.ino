#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

/* ---------- OLED ---------- */
#define SCREEN_W 128
#define SCREEN_H 64
Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);
bool oledOK = false;

/* ---------- BUZZER ---------- */
#define BUZZER_PIN 2
bool g_toneActive = false;
unsigned long g_toneEnd = 0;

/* ---------- NOTES ---------- */
struct KeyNote {
  char key;
  const char* name;
  uint16_t freq;
};

const KeyNote KNOTES[] = {
  {'1',"C4",262},{'2',"D4",294},{'3',"E4",330},
  {'4',"F4",349},{'5',"G4",392},{'6',"A4",440},
  {'7',"B4",494},{'8',"C5",523},{'9',"D5",587}
};

const uint8_t NKNOTES = sizeof(KNOTES)/sizeof(KNOTES[0]);

/* ---------- KEYPAD ---------- */
const byte ROWS = 4;
const byte COLS = 4;

char keyMap[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {10,9,8,7};
byte colPins[COLS] = {6,5,4,3};

Keypad kpad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

/* ---------- RECORD ---------- */
#define MAX_SEQ 50

struct NoteEvent {
  uint16_t freq;
  uint16_t gap;
};

NoteEvent seq[MAX_SEQ];
uint8_t len = 0;

bool recording = false;
unsigned long lastPress = 0;

/* ---------- OCTAVE ---------- */
int octave = 0;

uint16_t applyOctave(uint16_t f){
  if(octave > 0) return f << octave;
  if(octave < 0) return f >> (-octave);
  return f;
}

/* ---------- HELPERS ---------- */
uint16_t freqOf(char k){
  for(uint8_t i=0;i<NKNOTES;i++){
    if(KNOTES[i].key==k) return KNOTES[i].freq;
  }
  return 0;
}

const char* nameOf(uint16_t f){
  for(uint8_t i=0;i<NKNOTES;i++){
    uint16_t base = KNOTES[i].freq;
    if(f==base || f==(base<<1) || f==(base<<2) ||
                  f==(base>>1) || f==(base>>2)){
      static char n[2];
      n[0] = KNOTES[i].name[0];
      n[1] = '\0';
      return n;
    }
  }
  return "?";
}

/* ---------- OLED ---------- */
void drawScreen(const char* top, const char* mid){
  if(!oledOK) return;

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0,0);
  display.print(top);

  display.setCursor(80,0);
  display.print("O:");
  display.print(octave);

  display.setTextSize(2);
  display.setCursor(0,20);
  display.print(mid);

  display.setTextSize(1);
  display.setCursor(0,54);
  display.print("Len:");
  display.print(len);

  display.display();
}

/* ---------- SOUND ---------- */
void idle(){
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN,HIGH);
  g_toneActive=false;
}

void play(uint16_t f,uint16_t d){
  if(!f||!d) return;
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN,f);
  g_toneEnd=millis()+d;
  g_toneActive=true;
}

void updateSound(){
  if(g_toneActive && millis()>=g_toneEnd){
    idle();
  }
}

/* ---------- NOTE ---------- */
void note(char k){
  uint16_t base = freqOf(k);
  if(!base) return;

  uint16_t f = applyOctave(base);
  unsigned long now = millis();

  play(f,180);
  drawScreen(recording?"REC":"PLAY", nameOf(f));

  if(recording && len<MAX_SEQ){
    seq[len].freq = f;

    if(len==0) seq[len].gap=0;
    else{
      unsigned long g=now-lastPress;
      seq[len].gap=(g>65535)?65535:g;
    }

    lastPress=now;
    len++;
  }
}

/* ---------- PLAYBACK ---------- */
void playback(){
  if(len==0){
    drawScreen("PLAY","EMPTY");
    delay(400);
    drawScreen("READY","");
    return;
  }

  recording=false;

  for(uint8_t i=0;i<len;i++){
    uint16_t gap=(i+1<len)?seq[i+1].gap:0;
    uint16_t dur=180;

    if(gap>0 && gap<dur) dur=gap;

    drawScreen("PLAY", nameOf(seq[i].freq));

    play(seq[i].freq,dur);
    delay(dur);

    idle();

    if(gap>dur) delay(gap-dur);
  }

  drawScreen("DONE","");
  delay(300);
  drawScreen("READY","");
}

/* ---------- CONTROL ---------- */
void control(char k){
  if(k=='A'){
    len=0;
    recording=true;
    lastPress=millis();
    drawScreen("REC","START");
  }
  else if(k=='B'){
    recording=false;
    drawScreen("READY","");
  }
  else if(k=='C'){
    playback();
  }
  else if(k=='D'){
    len=0;
    recording=false;
    drawScreen("CLEAR","");
    delay(300);
    drawScreen("READY","");
  }
  else if(k=='*'){
    if(octave > -2) octave--;
    drawScreen("OCT","DOWN");
  }
  else if(k=='#'){
    if(octave < 2) octave++;
    drawScreen("OCT","UP");
  }
}

/* ---------- SETUP ---------- */
void setup(){
  pinMode(BUZZER_PIN,OUTPUT);
  idle();

  kpad.setDebounceTime(20);

  Wire.begin();
  Wire.setClock(100000);

  oledOK = display.begin(SSD1306_SWITCHCAPVCC,0x3C);

  if(oledOK){
    drawScreen("MELODY","READY");
  }
}

/* ---------- LOOP ---------- */
void loop(){
  char k=kpad.getKey();

  if(k){
    if(freqOf(k)) note(k);
    else control(k);
  }

  updateSound();
}