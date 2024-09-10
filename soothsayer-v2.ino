
// this version was tested using an arduino nano every

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

// on nano every: D11 (COPI), D12 (CIPO), D13 (SCK). Use any GPIO for Chip Select (CS).
#define TFT_CS 10
#define TFT_RST 9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 8
// this display Adafruit 1.54" 240x240 tft https://www.adafruit.com/product/3787
// or this display Adafruit 1.3" 240x240 tft https://www.adafruit.com/product/4313 

// our other important hardware pins
#define BUTTON_PIN 2
#define LED_1_PIN 4
#define LED_2_PIN 3

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// keep track of the button's state and previous state for debouncing
int buttonState = 0;
int prevState = 0;

void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello!");

  tft.init(240, 240); // Init ST7789 240x240 display
  tft.setSPISpeed(40000000); // for some reason it doesn't work without this

  Serial.println("Initialized");

  drawText("Waking up...", ST77XX_GREEN);

  tft.fillScreen(ST77XX_BLACK);

  // initialize the LED pins as an output:
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(BUTTON_PIN, INPUT);

  drawText("Waiting...", ST77XX_WHITE);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(BUTTON_PIN);
  if (prevState != buttonState) {
    prevState = buttonState;
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (prevState == HIGH) {
      // turn LED on:
      digitalWrite(LED_1_PIN, HIGH);
      digitalWrite(LED_2_PIN, HIGH);
      tft.fillScreen(ST77XX_BLACK);
      drawText("Yes?", ST77XX_RED); // waiting for them to let go of the button
    } else {
      // button has been let go...
      // turn LED off:
      digitalWrite(LED_1_PIN, LOW);
      digitalWrite(LED_2_PIN, LOW);
      tft.fillScreen(ST77XX_BLACK);
      // pick random decision....
      int diceRoll = random(1, 4);
      if (diceRoll == 1) {
        drawText("Yes!", ST77XX_GREEN);
      } else if (diceRoll == 2) {
        drawText("No.", ST77XX_RED);
      } else if (diceRoll == 3) {
        drawText("Maybe", ST77XX_BLUE);
      }
      // delay....
      delay(2000);
      tft.fillScreen(ST77XX_BLACK);
      drawText("Waiting...", ST77XX_WHITE);
    }
  }
}

void drawText(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setTextSize(4);
  tft.print(text);
}
