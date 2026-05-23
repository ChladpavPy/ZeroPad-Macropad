#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>

// So basically this code works like rubber ducky, it is a keyboard that can type much faster than a human

Adafruit_SSD1306 disp(128, 32, &Wire, -1);

// pins mapped from the kicad schematic
int b1 = 26;
int b2 = 27;
int b3 = 28;
int b4 = 0;
int b5 = 1;
int b6 = 29;

int led1 = 2; 
int led2 = 4;
int led3 = 3;

bool gamingMode = false; // i  will use this later maybe for layers

void setup() {
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4, INPUT_PULLUP);
  pinMode(b5, INPUT_PULLUP);
  pinMode(b6, INPUT_PULLUP);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  Keyboard.begin();

  
  // init display
  disp.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  disp.clearDisplay();
  disp.setTextColor(1);
  disp.setCursor(0,0);
  disp.print("zeropad booting...");
  disp.display();
  delay(1000); // give it a sec to breathe
  
  disp.clearDisplay();
  disp.setCursor(0,0);
  disp.print("Work Mode");
  disp.display();
}

// custom function to open apps acting like a super fast human
void openApp(String appName) {
  Keyboard.press(KEY_LEFT_GUI); // press windows key
  delay(50);
  Keyboard.releaseAll();
  delay(300); // wait for start menu to pop up, otherwise it eats the first letters
  Keyboard.print(appName);
  delay(300); // wait for windows search to find it
  Keyboard.press(KEY_RETURN); // smash enter
  delay(50);
  Keyboard.releaseAll();
}

void loop() {
  
  if (digitalRead(b1) == LOW) {
    digitalWrite(led1, HIGH);
    openApp("kicad"); // magically opens kicad
    delay(500); // desperately preventing double clicks
    digitalWrite(led1, LOW);
  }

  if (digitalRead(b2) == LOW) {
    digitalWrite(led2, HIGH);
    openApp("fusion 360");
    delay(500);
    digitalWrite(led2, LOW);
  }

  if (digitalRead(b3) == LOW) {
    digitalWrite(led3, HIGH);
    openApp("code"); // opens vs code
    delay(500);
    digitalWrite(led3, LOW);
  }

  if (digitalRead(b4) == LOW) {
    // just a classic ctrl c
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('c');
    delay(100);
    Keyboard.releaseAll();
    delay(300);
  }

  if (digitalRead(b5) == LOW) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('v');
    delay(100);
    Keyboard.releaseAll();
    delay(300);
  }

  if (digitalRead(b6) == LOW) {
    gamingMode = !gamingMode; // flip the boolean state
    disp.clearDisplay();
    disp.setCursor(0,0);
    
    if(gamingMode) {
       disp.print("Gaming Mode");
       // you could change the app shortcuts here based on this mode later
    } else {
       disp.print("Work Mode");
    }
    disp.display();
    delay(500); // bounce protection
  }
}