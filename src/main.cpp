#include <Arduino.h>
#include "OneButton.h"
#include <LiquidCrystal.h>
#define  INTERVAL  300UL

byte bukva_Mz[8]  = {B10000,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква "Ь"
byte bukva_Y[8]   = {B10001,B10001,B10001,B01010,B00100,B01000,B10000,B00000,}; // Буква "У"
byte bukva_Shch[8]= {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00001,}; // Буква "Щ"
byte bukva_Ya[8]  = {B01111,B10001,B10001,B01111,B00101,B01001,B10001,B00000,}; // Буква "Я"


// motor setup
const int dirPin = 13;
const int stepPin = 10;
const int enablPin = 12;
// direction pins
const int leftDirectionPin = 8;
const int rightDirectionPin = 9;
static unsigned long previousMillis = 0;

const int delayEngineTimeConst = 3700;

// logic const
const int minSpeed = 1;

//  display const
const String speedText = "CKOPOCT\1:";
const String currentSpeedText = "TEK\2\3A\4: ";
const String forceSpeedTex = "\2CKOPEHHA\4: ";
const String emptyString = "              ";

// display setup
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int displayMode = 0;
// button setup
OneButton displayModeButton(A0, true);
OneButton addSpeedButton(A1, true);
OneButton decreaseSpeedButton(A2, true);
OneButton forceSpeedButton(A5, true);

int normalSpeedMode = 10;
int forcedSpeedMode = 200;

bool isEngineStarted = false;
bool isLeftDirection = false;
bool isRightDirection = false;

int normalSpeedDelay = 3700;
int forceCurrentSpeedDelay = 185;
int currentDelay = 3700;

int maxNormalSpeedMode = 500;
int maxForcedSpeedMode = 500;

int direction = 0;
bool isEngineLocked = false;
bool isForceMode = false;

void showData()
{
  lcd.setCursor(0, 0);
  lcd.print(speedText);
  lcd.setCursor(0, 1);

  if (displayMode == 0)
  {
    lcd.print(currentSpeedText + normalSpeedMode + emptyString);
  }
  else
  {
    lcd.print(forceSpeedTex + forcedSpeedMode + emptyString);
  }
}

void stopEngine()
{
  if (!isEngineLocked){
      digitalWrite(enablPin, HIGH);
      isEngineLocked = true;
  }
}

void startEngine()
{
  if (isEngineLocked){
    digitalWrite(enablPin, LOW);
    isEngineLocked = false;
  }
}

void forceEngineSpeed()
{
  currentDelay = forceCurrentSpeedDelay;
  isForceMode = true;
}

void normalizeSpeed()
{
  currentDelay = normalSpeedDelay;
  isForceMode = false;
}

void processCurrentDelay(){
  if (isForceMode)
  {
    currentDelay = forceCurrentSpeedDelay;
  }
  else
  {
    currentDelay = normalSpeedDelay;
  }
}

void addSpeed()
{
  if (displayMode == 0 && normalSpeedMode < maxNormalSpeedMode)
  {
    normalSpeedMode++;
    normalSpeedDelay = delayEngineTimeConst / (normalSpeedMode / 10);
    showData();
    processCurrentDelay();
  }
  else if (displayMode == 1 && forcedSpeedMode < maxForcedSpeedMode)
  {
    forcedSpeedMode++;
    forceCurrentSpeedDelay = delayEngineTimeConst / (forcedSpeedMode / 10);
    showData();
    processCurrentDelay();
  }

}

void descreaseSpeed()
{
  if (displayMode == 0 && normalSpeedMode > minSpeed)
  {
    normalSpeedMode--;
    normalSpeedDelay = delayEngineTimeConst / (normalSpeedMode / 10);
    showData();
    processCurrentDelay();
  }
  else if (displayMode == 1 && forcedSpeedMode > minSpeed)
  {
    forcedSpeedMode--;
    forceCurrentSpeedDelay = delayEngineTimeConst / (forcedSpeedMode / 10);
    showData();
    processCurrentDelay();
  }
}

void descreaseSpeedWithDelay()
{
  if (millis() - previousMillis > INTERVAL)
  {
    previousMillis = millis();
    descreaseSpeed();
  }
}

void addSpeedWithDelay()
{
  if (millis() - previousMillis > INTERVAL)
  {
    previousMillis = millis();
    addSpeed();
  }
}

void changeDisplayMode()
{
  if (displayMode == 0)
  {
    displayMode = 1;
  }
  else
  {
    displayMode = 0;
  }

  showData();
}

void setup()
{
  // motor setup
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablPin, OUTPUT);

  // input on/off setup 
  pinMode(leftDirectionPin, INPUT_PULLUP);
  pinMode(rightDirectionPin, INPUT_PULLUP);
  stopEngine();

  // button setup
  addSpeedButton.attachClick(addSpeed);
  decreaseSpeedButton.attachClick(descreaseSpeed);

  addSpeedButton.attachDuringLongPress(addSpeedWithDelay);
  decreaseSpeedButton.attachDuringLongPress(descreaseSpeedWithDelay);
  
  forceSpeedButton.attachLongPressStart(forceEngineSpeed);
  forceSpeedButton.attachLongPressStop(normalizeSpeed);
  displayModeButton.attachClick(changeDisplayMode);

  lcd.begin(16, 2); // Инициализирует LCD 16x2
  lcd.createChar(1, bukva_Mz); 
  lcd.createChar(2, bukva_Y); 
  lcd.createChar(3, bukva_Shch); 
  lcd.createChar(4, bukva_Ya);
  showData();
}

void processEngineSpeed()
{
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(currentDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(currentDelay);
}

void  validateIsEngineStatus(){
  isLeftDirection = !digitalRead(leftDirectionPin);
  isRightDirection = !digitalRead(rightDirectionPin);
  isEngineStarted = (isLeftDirection && !isRightDirection) || (!isLeftDirection && isRightDirection);
}

void validateDirection(){
    if (isLeftDirection && direction == 1){
      direction = 0;
      digitalWrite(dirPin, direction);
    }
    else if (isRightDirection && direction == 0){
      direction = 1;
      digitalWrite(dirPin, direction);
    }
}

void loop()
{
  validateIsEngineStatus();
  
  if (isEngineStarted)
  {
    startEngine();
    validateDirection();
    processEngineSpeed();
    forceSpeedButton.tick();
  }
  else 
  {
    stopEngine();
  }

  addSpeedButton.tick();
  decreaseSpeedButton.tick();
  displayModeButton.tick();
}
