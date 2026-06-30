#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>

#define MY_SDA 6
#define MY_SCL 7

#define B1 26
#define B2 27
#define B3 28
#define B4 0
#define B5 1
#define B6 29

#define LED1 2
#define LED2 4
#define LED3 3

Adafruit_SSD1306 display(128, 64, &Wire, -1);
bool displayOK = false;

int currentMode = 0;
int menuIndex = 0;
const char* menuLabels[] = {"> Shortcuts", "> Morse code", "> Generator"};

String morseBuffer = "";
String textBuffer = "";
const char* morseTable[] = {
  ".-","-...","-.-.","-..",".","..-.","--.","....","..",
  ".---","-.-",".-..","--","-.","---",".--.","--.-",".-.",
  "...","-","..-","...-",".--","-..-","-.--","--.."
};

int passLen = 13;
String currentPass = "QJA2mep%TrWwJ";
char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);
  pinMode(B5, INPUT_PULLUP);
  pinMode(B6, INPUT_PULLUP);

  pinMode(MY_SDA, INPUT_PULLUP);
  pinMode(MY_SCL, INPUT_PULLUP);
  delay(10);
  
  Wire.setSDA(MY_SDA);
  Wire.setSCL(MY_SCL);
  Wire.setClock(100000);
  Wire.begin();

  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C) || display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    displayOK = true;
  }
  
  Keyboard.begin();
  randomSeed(analogRead(26));
  delay(1000);
  drawMenu();
}

void loop() {
  if (currentMode == 0) {
    if (digitalRead(B1) == LOW) { menuIndex--; if(menuIndex < 0) menuIndex = 2; drawMenu(); delay(200); }
    if (digitalRead(B2) == LOW) { menuIndex++; if(menuIndex > 2) menuIndex = 0; drawMenu(); delay(200); }
    if (digitalRead(B3) == LOW) { currentMode = menuIndex + 1; drawMode(); delay(200); }
  } 
  else if (currentMode == 1) {
    if (digitalRead(B6) == LOW) { currentMode = 0; drawMenu(); delay(200); }
    if (digitalRead(B1) == LOW) { Keyboard.press(KEY_SCAN_PREVIOUS); Keyboard.releaseAll(); delay(200); }
    if (digitalRead(B2) == LOW) { Keyboard.press(KEY_PLAY_PAUSE); Keyboard.releaseAll(); delay(200); }
    if (digitalRead(B5) == LOW) { Keyboard.press(KEY_LEFT_GUI); Keyboard.press(KEY_LEFT_SHIFT); Keyboard.press('s'); Keyboard.releaseAll(); delay(200); }
  } 
  else if (currentMode == 2) {
    if (digitalRead(B6) == LOW) { currentMode = 0; drawMenu(); delay(200); }
    if (digitalRead(B1) == LOW) { morseBuffer += "."; drawMorse(); delay(200); }
    if (digitalRead(B2) == LOW) { morseBuffer += "-"; drawMorse(); delay(200); }
    if (digitalRead(B3) == LOW) {
      for (int i = 0; i < 26; i++) {
        if (morseBuffer == morseTable[i]) {
          textBuffer += (char)('A' + i);
          Keyboard.print((char)('a' + i));
          break;
        }
      }
      morseBuffer = "";
      drawMorse();
      delay(200);
    }
    if (digitalRead(B4) == LOW) { morseBuffer = ""; drawMorse(); delay(200); }
    if (digitalRead(B5) == LOW) { textBuffer += " "; Keyboard.print(" "); drawMorse(); delay(200); }
  } 
  else if (currentMode == 3) {
    if (digitalRead(B6) == LOW) { currentMode = 0; drawMenu(); delay(200); }
    if (digitalRead(B1) == LOW) { passLen++; if(passLen > 25) passLen = 25; drawGenerator(); delay(200); }
    if (digitalRead(B2) == LOW) { passLen--; if(passLen < 4) passLen = 4; drawGenerator(); delay(200); }
    if (digitalRead(B3) == LOW) { 
      currentPass = "";
      for(int i=0; i<passLen; i++) {
        currentPass += charset[random(0, strlen(charset))];
      }
      drawGenerator(); 
      delay(200); 
    }
    if (digitalRead(B4) == LOW) { Keyboard.print(currentPass); delay(200); }
  }
}

void drawMenu() {
  if(!displayOK) return;
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("ZEROPAD OS");
  display.println("----------------");
  for(int i = 0; i < 3; i++) {
    if(i == menuIndex) {
      display.print("-> ");
    } else {
      display.print("   ");
    }
    display.println(menuLabels[i]);
  }
  display.display();
}

void drawMode() {
  if(currentMode == 1) drawShortcuts();
  if(currentMode == 2) drawMorse();
  if(currentMode == 3) drawGenerator();
}

void drawShortcuts() {
  if(!displayOK) return;
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("SHORTCUTS");
  display.println("----------------");
  display.println("B1: Prev Track");
  display.println("B2: Play/Pause");
  display.println("B5: Win+Shift+S");
  display.println("B6: Zpet do Menu");
  display.display();
}

void drawMorse() {
  if(!displayOK) return;
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("MORSE CODE");
  display.println("----------------");
  display.print("Znak: "); 
  display.println(morseBuffer);
  display.print("Text: "); 
  display.println(textBuffer);
  display.display();
}

void drawGenerator() {
  if(!displayOK) return;
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("[ GENERATOR ]");
  display.print("Delka: ");
  display.println(passLen);
  display.println("Heslo:");
  display.println(currentPass);
  display.setCursor(0, 56);
  display.println("B1:len+ B2:len- B3:Gen");
  display.display();
}
