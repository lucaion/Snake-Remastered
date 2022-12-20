#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>

// matrix variables
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No.DRIVER

// lcd variables
const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 3;
const byte d6 = 5;
const byte d7 = 4;
const byte lcdBrightnessPin = 6;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
const byte lcdWidth = 16;
const byte lcdHeight = 2;


// joystick variables
byte swState = LOW;
int xValue = 0;
int yValue = 0;
const int pinSW = 2; 
const int pinX = A0; 
const int pinY = A1;

// variables for joystick moves
bool joyMoved = false;
const int minThreshold = 200;
const int maxThreshold = 750;
const int minDiagonalThreshold = 400;
const int maxDiagonalThreshold = 600;

// debounce variables
const int debounceDelay = 50;
unsigned long int lastDebounceTime = 0;
volatile byte lastReading = LOW;
byte reading = LOW;

// menu size variables
const byte mainMenuSize = 5;
const byte leaderboardMenuSize = 6;
const byte settingsMenuSize = 7;
const byte aboutMenuSize = 4;
const byte howToPlayMenuSize = 12;
const byte soundMenuSize = 3;
const byte difficultyMenuSize = 4;
const byte resetScoreMenuSize = 2;

// delay for welcome
const long delayWelcomeVariable = 1000;

// menu auxiliary variables
byte currentMenu = 0;
byte lastCursorState;
byte currentCursorState = 0;

bool playingGame = false;

// custom lcdCharacter
byte rightArrow[] = {
  B00000,
  B00100,
  B00110,
  B11111,
  B11111,
  B00110,
  B00100,
  B00000
};

byte downArrow[] = {
  B00000,
  B01110,
  B01110,
  B01110,
  B11111,
  B01110,
  B00100,
  B00000
};

byte upArrow[] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B01110,
  B01110,
  B01110,
  B00000
};

byte checkMark[] = {
  B00000,
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
  B00000,
  B00000
};

byte pBar[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte congrats[] = {
  B00000,
  B00000,
  B00000,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000
};

byte unfortunately[] = {
  B00000,
  B00000,
  B00000,
  B01010,
  B00000,
  B00000,
  B01110,
  B10001
};

byte matrix[matrixSize][matrixSize] = {
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};

// custom matrixes
const int matrixOn[matrixSize] = {
  B00000000,
  B00000000,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B00000000,
  B00000000
};

const int welcomeMatrix[matrixSize] = {
  B00000000,
  B10010111,
  B10010010,
  B10010010,
  B11110010,
  B10010010,
  B10010010,
  B10010111
};

const int startGameMatrix[matrixSize] = {
  B00100000,
  B00110000,
  B00111000,
  B00111100,
  B00111000,
  B00110000,
  B00100000,
  B00000000
};

const int leaderboardMatrix[matrixSize] = {
  B00000000,
  B00000000,
  B00011000,
  B01111000,
  B01111110,
  B01111110,
  B00000000,
  B00000000
};

const int settingsMatrix[matrixSize] = {
  B00000100,
  B00001000,
  B00001001,
  B00001110,
  B01110000,
  B10010000,
  B00010000,
  B00100000
};


const int difficultyMatrix[matrixSize] = {
  B00111100,
  B11111111,
  B11011011,
  B11111111,
  B01100110,
  B00111100,
  B00000000,
  B00111100
};

const int brightnessMatrix[matrixSize] = {
  B00000000,
  B00011000,
  B00110100,
  B00110100,
  B00011000,
  B00000010,
  B11100111,
  B00000010
};

const int lcdBrightnessMatrix1[matrixSize] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B01000000,
  B00000000
};

const int lcdBrightnessMatrix2[matrixSize] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00100000,
  B01100000,
  B00000000
};

const int lcdBrightnessMatrix3[matrixSize] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00010000,
  B00110000,
  B01110000,
  B00000000
};

const int lcdBrightnessMatrix4[matrixSize] = {
  B00000000,
  B00000000,
  B00000000,
  B00001000,
  B00011000,
  B00111000,
  B01111000,
  B00000000
};

const int lcdBrightnessMatrix5[matrixSize] = {
  B00000000,
  B00000000,
  B00000100,
  B00001100,
  B00011100,
  B00111100,
  B01111100,
  B00000000
};

const int lcdBrightnessMatrix6[matrixSize] = {
  B00000000,
  B00000010,
  B00000110,
  B00001110,
  B00011110,
  B00111110,
  B01111110,
  B00000000
};

const int soundOnMatrix[matrixSize] = {
  B00000000,
  B00010010,
  B00110100,
  B11010000,
  B10010111,
  B11010000,
  B00110100,
  B00010010
};

const int soundOffMatrix[matrixSize] = {
  B00000000,
  B00010000,
  B00110000,
  B11010101,
  B10010010,
  B11010101,
  B00110000,
  B00010000
};

const int backMatrix[matrixSize] = {
  B00000000,
  B00010000,
  B00100000,
  B01111100,
  B00100010,
  B00010001,
  B00000010,
  B01111100
};

const int enterNameMatrix[matrixSize] = {
  B00000000,
  B00011000,
  B00111100,
  B00111100,
  B00011000,
  B01111110,
  B11111111,
  B00000000
};

const int resetScoreMatrix[matrixSize] = {
  B00000000,
  B00101110,
  B01001100,
  B10001010,
  B10000010,
  B10100010,
  B01100100,
  B11101000
};

const int howToPlayMatrix[matrixSize] = {
  B00000000,
  B00111000,
  B01000100,
  B00001000,
  B00010000,
  B00010000,
  B00000000,
  B00010000
};

const int aboutMatrix[matrixSize] = {
  B00000000,
  B00010000,
  B00000000,
  B00110000,
  B00010000,
  B00010000,
  B00011000,
  B00000000
};

const int githubMatrix[matrixSize] = {
  B01000010,
  B01100110,
  B11111111,
  B11111111,
  B01111110,
  B00011000,
  B10111000,
  B01011000
};

const int firstMatrix[matrixSize] = {
  B00000000,
  B00001000,
  B00011000,
  B00101000,
  B00001000,
  B00001000,
  B00001000,
  B00111110
};

const int secondMatrix[matrixSize] = {
  B00000000,
  B00111100,
  B01000010,
  B00000100,
  B00001000,
  B00010000,
  B00100010,
  B01111110
};

const int thirdMatrix[matrixSize] = {
  B00000000,
  B00111000,
  B00000100,
  B00000100,
  B00011000,
  B00000100,
  B00000100,
  B00111000
};

const int fourthMatrix[matrixSize] = {
  B00000100,
  B00001100,
  B00010100,
  B00100100,
  B01111110,
  B00000100,
  B00000100,
  B00000100
};

const int fifthMatrix[matrixSize] = {
  B00000000,
  B00111100,
  B00100000,
  B00100000,
  B00111100,
  B00000010,
  B00000010,
  B00111100
};

const int happyFaceMatrix[matrixSize] = {
  B11000011,
  B10000001,
  B01100110,
  B01100110,
  B00000000,
  B01000010,
  B00111100,
  B00000000
};

const int sadFaceMatrix[matrixSize] = {
  B11000011,
  B10000001,
  B01100110,
  B01100110,
  B00000000,
  B00000000,
  B00111100,
  B01000010
};

const int medalMatrix[matrixSize] = {
  B10000001,
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00111100,
  B00111100,
  B00011000
};

// buzzer variables
const int buzzerPin = 13;
const int buzzerWrongMove = 370;
const int buzzerRightMove = 600;
const int buzzerButtonPressed = 105;
const int buzzerEatFood = 500;

// player variables
char defaultPlayer[7] = "Player";
char defeatedPlayer[7];

// settings variables
struct {
  char playerName[7];
  byte difficulty;
  byte lcdBrightness;
  byte matrixBrightness;
  byte sound;
} settings;

// variable for scrollText
long lastChanged;

// index variable for enterName
byte indexName;

// highscore variables
int highscores[5] = {0, 0, 0, 0, 0};
char highscoreNames[5][7];
int defeatedPosition;
bool highscoreDefeated = false;

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(lcdBrightnessPin, OUTPUT);

  lcd.begin(lcdWidth, lcdHeight);
  lcd.createChar(1, rightArrow);
  lcd.createChar(2, downArrow);
  lcd.createChar(3, upArrow);
  lcd.createChar(4, checkMark);
  lcd.createChar(5,  pBar);
  lcd.createChar(6, congrats);
  lcd.createChar(7, unfortunately);

  dataFromEEPROM();
  
  analogWrite(lcdBrightnessPin, settings.lcdBrightness);
  updateHighscores(0);

  lcd.begin(lcdWidth, lcdHeight);
  lc.shutdown(0, false);
  lc.setIntensity(0, settings.matrixBrightness);
  lc.clearDisplay(0);
  welcomeAnimation();
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  reading = digitalRead(pinSW);
  if (!playingGame) {
    handleMenu();
  }
  else {
    generateFood();    
    scanJoystick();  
    calculateSnake();  
    handleGameOver();
    menuInGame();
  }
}

void handleMenu() {
  buttonPressed();
  switch (currentMenu) {
    case 0:
      displayMainMenu();
      menuUpDownMoves(mainMenuSize);
      break;
    case 1:
      displayLeaderboard();
      menuUpDownMoves(leaderboardMenuSize);
      break;
    case 2:
      displaySettings();
      menuUpDownMoves(settingsMenuSize);
      break;
    case 3:
      displayAbout();
      break;
    case 4:
      displayHowToPlay();
      menuUpDownMoves(howToPlayMenuSize);
      break;
    case 5:
      displayEnterName();
      break;
    case 6:
      displayDifficulty();
      menuUpDownMoves(difficultyMenuSize);
      break;
    case 7:
      displayLCDBrightness();
      break;
    case 8:
      displayMatBrightness();
      break;
    case 9:
      displaySound();
      menuUpDownMoves(soundMenuSize);
      break;
    case 10:
      displayResetScore();
      menuUpDownMoves(resetScoreMenuSize);
      break;
  }
}

// function to print different drawings on matrix 
void showCustomMatrix(int customMatrix[matrixSize]) {
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, customMatrix[row]);
  } 
}

void displayMainMenu() {
  if (currentMenu == 0){
    switch (currentCursorState) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("Snake Remastered");

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Start Game");
        showCustomMatrix(startGameMatrix);
        lcd.setCursor(15, 1);
        lcd.write(2);

        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.write(1);
        lcd.setCursor(2, 0);
        lcd.print("Leaderboard");
        showCustomMatrix(leaderboardMatrix);
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(2, 1);
        lcd.print("Settings");
        lcd.setCursor(15, 1);
        lcd.write(2);

        break;
      case 2:
        lcd.setCursor(2, 0);
        lcd.print("Leaderboard");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Settings");
        showCustomMatrix(settingsMatrix);
        lcd.setCursor(15, 1);
        lcd.write(2);

        break;
      case 3:
        lcd.setCursor(0, 0);
        lcd.write(1);
        lcd.setCursor(2, 0);
        lcd.print("About");
        showCustomMatrix(aboutMatrix);
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(2, 1);
        lcd.print("How to play");
        lcd.setCursor(15, 1);
        lcd.write(2);

        break;
      case 4:
        lcd.setCursor(2, 0);
        lcd.print("About");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("How to play");
        showCustomMatrix(howToPlayMatrix);

        break;     
    }
  }
}

void displayLeaderboard(){
  if(currentMenu == 1) {
    switch (currentCursorState) {
      case 0:
        lcd.setCursor(2, 0);
        lcd.print("Leaderboard");
        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.write("1. ");
        lcd.setCursor(5, 1);
        lcd.print(String(highscoreNames[0]));
        lcd.setCursor(11, 1);
        lcd.print(": ");
        lcd.setCursor(13, 1);
        lcd.print(String(highscores[0]));
        lcd.setCursor(15, 1);
        lcd.write(2);
        showCustomMatrix(firstMatrix);
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.write(1);
        lcd.setCursor(2, 0);
        lcd.write("2. ");
        lcd.setCursor(5, 0);
        lcd.print(String(highscoreNames[1]));
        lcd.setCursor(11, 0);
        lcd.print(": ");
        lcd.setCursor(13, 0);
        lcd.print(String(highscores[1]));
        lcd.setCursor(15, 0);
        lcd.write(3);
        showCustomMatrix(secondMatrix);
        lcd.setCursor(2, 1);
        lcd.write("3. ");
        lcd.setCursor(5, 1);
        lcd.print(String(highscoreNames[2]));
        lcd.setCursor(11, 1);
        lcd.print(": ");
        lcd.setCursor(13, 1);
        lcd.print(String(highscores[2]));
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 2:
        lcd.setCursor(2, 0);
        lcd.write("2. ");
        lcd.setCursor(5, 0);
        lcd.print(String(highscoreNames[1]));
        lcd.setCursor(11, 0);
        lcd.print(": ");
        lcd.setCursor(13, 0);
        lcd.print(String(highscores[1]));
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.write("3. ");
        lcd.setCursor(5, 1);
        lcd.print(String(highscoreNames[2]));
        lcd.setCursor(11, 1);
        lcd.print(": ");
        lcd.setCursor(13, 1);
        lcd.print(String(highscores[2]));
        lcd.setCursor(15, 1);
        lcd.write(2);
        showCustomMatrix(thirdMatrix);
        break;
      case 3:
        lcd.setCursor(0, 0);
        lcd.write(1);
        lcd.setCursor(2, 0);
        lcd.write("4. ");
        lcd.setCursor(5, 0);
        lcd.print(String(highscoreNames[3]));
        lcd.setCursor(11, 0);
        lcd.print(": ");
        lcd.setCursor(13, 0);
        lcd.print(String(highscores[3]));
        lcd.setCursor(15, 0);
        lcd.write(3);
        showCustomMatrix(fourthMatrix);
        lcd.setCursor(2, 1);
        lcd.write("5. ");
        lcd.setCursor(5, 1);
        lcd.print(String(highscoreNames[4]));
        lcd.setCursor(11, 1);
        lcd.print(": ");
        lcd.setCursor(13, 1);
        lcd.print(String(highscores[4]));
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 4:
        lcd.setCursor(2, 0);
        lcd.write("4. ");
        lcd.setCursor(5, 0);
        lcd.print(String(highscoreNames[3]));
        lcd.setCursor(11, 0);
        lcd.print(": ");
        lcd.setCursor(13, 0);
        lcd.print(String(highscores[3]));
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.write("5. ");
        lcd.setCursor(5, 1);
        lcd.print(String(highscoreNames[4]));
        lcd.setCursor(11, 1);
        lcd.print(": ");
        lcd.setCursor(13, 1);
        lcd.print(String(highscores[4]));
        lcd.setCursor(15, 1);
        lcd.write(2);
        showCustomMatrix(fifthMatrix);
        break;
      case 5:
        lcd.setCursor(2, 0);
        lcd.write("5. ");
        lcd.setCursor(5, 0);
        lcd.print(String(highscoreNames[4]));
        lcd.setCursor(11, 0);
        lcd.print(": ");
        lcd.setCursor(13, 0);
        lcd.print(String(highscores[4]));
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Back");
        showCustomMatrix(backMatrix);
        break;
    }
  }
}

void displaySettings(){
  if (currentMenu == 2){
    switch (currentCursorState) {
      case 0:
        lcd.setCursor(4, 0);
        lcd.print("Settings");

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Enter name");
        showCustomMatrix(enterNameMatrix);
        lcd.setCursor(15, 1);
        lcd.write(2);

        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.write(1);
        lcd.setCursor(2, 0);
        lcd.print("Difficulty");
        showCustomMatrix(difficultyMatrix);
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(2, 1);
        lcd.print("LCDBrightness");
        lcd.setCursor(15, 1);
        lcd.write(2);

        break;
      case 2:
        lcd.setCursor(2, 0);
        lcd.print("Difficulty");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);        
        lcd.setCursor(2, 1);
        lcd.print("LCDBrightness");
        lcd.setCursor(15, 1);
        lcd.write(2);
        showCustomMatrix(brightnessMatrix);

        break;
      case 3:
        lcd.setCursor(0, 0);
        lcd.write(1);
        lcd.setCursor(2, 0);
        lcd.print("MatBrightness");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(2, 1);
        lcd.print("Sound");
        lcd.setCursor(15, 1);
        lcd.write(2);
        showCustomMatrix(brightnessMatrix);

        break;
      case 4:
        lcd.setCursor(2, 0);
        lcd.print("MatBrightness");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);        
        lcd.setCursor(2, 1);
        lcd.print("Sound"); 
        lcd.setCursor(15, 1);
        lcd.write(2);        

        if (settings.sound == 1) {
          showCustomMatrix(soundOnMatrix);
        }
        else if (settings.sound == 0) {
          showCustomMatrix(soundOffMatrix);
        }

        break;     
      case 5:
        lcd.setCursor(0, 0);
        lcd.write(1); 
        lcd.setCursor(2, 0);
        lcd.print("Reset scores");
        showCustomMatrix(resetScoreMatrix);
        lcd.setCursor(15, 0);
        lcd.write(3);   

        lcd.setCursor(2, 1);
        lcd.print("Back");
        lcd.setCursor(15, 1);
        lcd.write(2);

        break;      
      case 6:
        lcd.setCursor(2, 0);
        lcd.print("Reset scores");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Back"); 
        showCustomMatrix(backMatrix);

        break;            
    }
  }
}

void displayEnterName() {
  int offset = (lcdWidth - 6) / 2;
  lcd.setCursor(offset - 1, 0);
  lcd.print('<');
  lcd.print(settings.playerName);
  lcd.setCursor(lcdWidth - offset, 0);
  lcd.print('>');
  lcd.setCursor(lcdWidth - offset + 1, 0);
  lcd.print(' ');
  lcd.setCursor(1, 1);
  lcd.print("Press to save!");
  lcd.setCursor(offset, 0);
  lcd.cursor();
  showCustomMatrix(enterNameMatrix);

  moveNameIndex();
  lcd.setCursor(indexName + offset, 0);
  if (indexName < 6) {
    moveNameLetter();
  }
  delay(50); // delay for lcd.cursor()
  lcd.noCursor();
}

void moveNameIndex() {
  if (yValue < minThreshold && joyMoved == false) {
    indexName = indexName > 0 ? indexName - 1 : 0;
    joyMoved = true;
  }

  if (yValue > maxThreshold && joyMoved == false) {
    indexName = indexName < 5 ? indexName + 1 : 5;
    joyMoved = true;
  }

  if (yValue > minThreshold && yValue < maxThreshold && xValue > minThreshold && xValue < maxThreshold) {
    joyMoved = false;
  }
}

void moveNameLetter() {
  char name[7];
  strcpy(name, settings.playerName);

  for (int i = strlen(name); i < 6; i++) {
    name[i] = ' ';
  }
  if (xValue < minThreshold && joyMoved == false) {
    if (name[indexName] == 'A') {
      name[indexName] = 'z';
      
    }
    else if (name[indexName] == ' ') {
      name[indexName] = 'Z';
    }
    else if (name[indexName] == 'a'){
      name[indexName] = ' ';
    }
    else {
      name[indexName] = (char)(name[indexName] - 1);
    }
    joyMoved = true;
  }

  if (xValue > maxThreshold && joyMoved == false) {
    if (name[indexName] == 'Z') {
      name[indexName] = ' ';
    }
    else if (name[indexName] == ' ') {
      name[indexName] = 'a';
    }
    else if (name[indexName] == 'z') {
      name[indexName] = 'A';
    }
    else {
      name[indexName] = (char)(name[indexName] + 1);
    }
    joyMoved = true;
  }

  if (xValue > minThreshold && xValue < maxThreshold && yValue > minThreshold && yValue < maxThreshold) {
    joyMoved = false;
  }

  // for (int i = 6; i > 0; --i) {
  //   if (name[i] != ' ') {
  //     name[i] = '\0';
  //     break;
  //   }
  // }

  strcpy(settings.playerName, name);
  saveSettingsInEEPROM();
}

void displayDifficulty() {
  if (currentMenu == 6) {
    switch (currentCursorState) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.write(1);
        lcd.setCursor(2, 0);
        lcd.print("Easy");
        showCustomMatrix(difficultyMatrix);

        lcd.setCursor(2, 1);
        lcd.print("Medium");
        lcd.setCursor(15, 1);
        lcd.write(2);

        if (settings.difficulty == 0) {
          lcd.setCursor(9, 0);
          lcd.write(4);
        }
        else if(settings.difficulty == 1) {
          lcd.setCursor(9, 1);
          lcd.write(4);
        }

        break;
      case 1:
        lcd.setCursor(2, 0);
        lcd.print("Easy");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Medium");
        showCustomMatrix(difficultyMatrix);
        lcd.setCursor(15, 1);
        lcd.write(2);

        if (settings.difficulty == 0) {
          lcd.setCursor(9, 0);
          lcd.write(4);
        }
        else if(settings.difficulty == 1) {
          lcd.setCursor(9, 1);
          lcd.write(4);
        }

        break;
      case 2:
        lcd.setCursor(2, 0);
        lcd.print("Medium");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Hard");
        showCustomMatrix(difficultyMatrix);

        if (settings.difficulty == 1) {
          lcd.setCursor(9, 0);
          lcd.write(4);
        }
        else if (settings.difficulty == 2) {
          lcd.setCursor(9, 2);
          lcd.write(4);
        }

        break;
      case 3:
        lcd.setCursor(2, 0);
        lcd.print("Hard");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Back");
        showCustomMatrix(backMatrix);

        if (settings.difficulty == 2) {
          lcd.setCursor(9, 0);
          lcd.write(4);
        }

        break;
    }
  }
}

void displayLCDBrightness() {
  if (currentMenu == 7) {
    lcd.setCursor(0, 0);
    lcd.print("-");
    lcd.setCursor(7, 0);
    lcd.print(settings.lcdBrightness);
    lcd.setCursor(15, 0);
    lcd.print("+");

    lcd.setCursor(1, 2);
    lcd.print("Press to save");
    analogWrite(lcdBrightnessPin, settings.lcdBrightness);
  
    if (settings.lcdBrightness > 0 && settings.lcdBrightness < 51) {
      showCustomMatrix(lcdBrightnessMatrix1);
    }

    if (settings.lcdBrightness >= 51 && settings.lcdBrightness < 102) {
      showCustomMatrix(lcdBrightnessMatrix2);
    }

    if (settings.lcdBrightness >= 102 && settings.lcdBrightness < 153) {
      showCustomMatrix(lcdBrightnessMatrix3);
    }

    if (settings.lcdBrightness >= 153 && settings.lcdBrightness < 204) {
      showCustomMatrix(lcdBrightnessMatrix4);
    }

    if (settings.lcdBrightness >= 204 && settings.lcdBrightness < 255) {
      showCustomMatrix(lcdBrightnessMatrix5);
    }

    if (settings.lcdBrightness == 255) {
      showCustomMatrix(lcdBrightnessMatrix6);
    }

    if (yValue < minThreshold && xValue < maxDiagonalThreshold && xValue > minDiagonalThreshold && joyMoved == false) {
      if (settings.lcdBrightness == 0) {  
        switch (settings.sound) {
          case 0:
            break;
          case 1:
            tone(buzzerPin, buzzerWrongMove, 30);
            break;
        }

        lc.clearDisplay(0);
      }
      else {
        settings.lcdBrightness -= 17;
        switch (settings.sound) {
          case 0:
            break;
          case 1:
            tone(buzzerPin, buzzerRightMove, 30);
            break;
        }

        lc.clearDisplay(0);
        lcd.clear();
      }

      joyMoved = true;
    }

    if (yValue > maxThreshold && xValue < maxDiagonalThreshold && xValue > minDiagonalThreshold && joyMoved == false) {
      if (settings.lcdBrightness == 255) {
        switch (settings.sound) {
          case 0:
            break;
          case 1:
            tone(buzzerPin, buzzerWrongMove, 30);
            break;
        }
        
        lc.clearDisplay(0);
      }
      else {
        settings.lcdBrightness += 17;
        switch (settings.sound) {
          case 0:
            break;
          case 1:
            tone(buzzerPin, buzzerRightMove, 30);
            break;
        }

        lc.clearDisplay(0);      
        lcd.clear();
      }
      joyMoved = true;
    }

    if (yValue > minThreshold && yValue < maxThreshold && xValue > minThreshold && xValue < maxThreshold) {
      joyMoved = false;
    }
  }

}

void displayMatBrightness() {
  lcd.setCursor(0, 0);
  lcd.print("-");
  lcd.setCursor(7, 0);
  lcd.print(settings.matrixBrightness);
  lcd.setCursor(15, 0);
  lcd.print("+");

  lcd.setCursor(1, 2);
  lcd.print("Press to save");
  lc.setIntensity(0, settings.matrixBrightness);

  showCustomMatrix(matrixOn);

  if (yValue < minThreshold && xValue < maxDiagonalThreshold && xValue > minDiagonalThreshold && joyMoved == false) {
    if (settings.matrixBrightness == 0) {  
      switch (settings.sound) {
        case 0:
          break;
        case 1:
          tone(buzzerPin, buzzerWrongMove, 30);
          break;
      }
    }
    else {
      settings.matrixBrightness--;
      switch (settings.sound) {
        case 0:
          break;
        case 1:
          tone(buzzerPin, buzzerRightMove, 30);
          break;
      }

      lcd.clear();
    }

    joyMoved = true;
  }

  if (yValue > maxThreshold && xValue < maxDiagonalThreshold && xValue > minDiagonalThreshold && joyMoved == false) {
    if (settings.matrixBrightness == 15) {
      switch (settings.sound) {
        case 0:
          break;
        case 1:
          tone(buzzerPin, buzzerWrongMove, 30);
          break;
      }
    }
    else {
      settings.matrixBrightness++;
      switch (settings.sound) {
        case 0:
          break;
        case 1:
          tone(buzzerPin, buzzerRightMove, 30);
          break;
      }

      lcd.clear();
    }
    joyMoved = true;
  }

  if (yValue > minThreshold && yValue < maxThreshold && xValue > minThreshold && xValue < maxThreshold) {
    joyMoved = false;
  }  
}

void displaySound () {
  if (currentMenu == 9) {
    switch (currentCursorState) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.write(1);
        lcd.setCursor(2, 0);
        lcd.print("On");
        showCustomMatrix(soundOnMatrix);

        lcd.setCursor(2, 1);
        lcd.print("Off");
        lcd.setCursor(15, 1);
        lcd.write(2);

        if (settings.sound == 1) {
          lcd.setCursor(6, 0);
          lcd.write(4);
        }
        else if(settings.sound == 0) {
          lcd.setCursor(6, 1);
          lcd.write(4);
        }

        break;
      case 1:
        lcd.setCursor(2, 0);
        lcd.print("On");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Off");
        showCustomMatrix(soundOffMatrix);
        lcd.setCursor(15, 1);
        lcd.write(2);

        if (settings.sound == 1) {
          lcd.setCursor(6, 0);
          lcd.write(4);
        }
        else if(settings.sound == 0) {
          lcd.setCursor(6, 1);
          lcd.write(4);
        }

        break;
      case 2:
        lcd.setCursor(2, 0);
        lcd.print("Off");
        lcd.setCursor(15, 0);
        lcd.write(3);

        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Back");
        showCustomMatrix(backMatrix);

        if (settings.sound == 0) {
          lcd.setCursor(6, 0);
          lcd.write(4);
        }

        break;
    }
  }
}

void displayResetScore() {
  if (currentMenu == 10) {
    switch (currentCursorState) {
      case 0:
        lcd.setCursor(2, 0);
        lcd.print("Are you sure?");
        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("Yes");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 1: 
        lcd.setCursor(2, 0);
        lcd.print("Yes");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(0, 1);
        lcd.write(1);
        lcd.setCursor(2, 1);
        lcd.print("No");
        break;
    }
    showCustomMatrix(resetScoreMatrix);
  }
}

void displayAbout(){
  if (currentMenu == 3) {
    lcd.setCursor(0, 0);
    lcd.print("-- About page --");
    char *text = "   Title: Snake Remastered      By: Luca-Mihail Ion     Github: https://github.com/lucaion      Press to go back";
    
    if (millis() - lastChanged < 5000) {
      for (int letter = 0; letter <= strlen(text) - 16; letter++) {
        showCustomMatrix(githubMatrix);
        scrollText(0, letter);
        
      }
      
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print("Press to go back");
      
      showCustomMatrix(backMatrix);
    }
  }
}

void displayHowToPlay(){
  if (currentMenu == 4) {
    switch (currentCursorState) {
      case 0:
        lcd.setCursor(3, 0);
        lcd.print("This is a");
        lcd.setCursor(2, 1);
        lcd.print("snake game");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 1:
        lcd.setCursor(2, 0);
        lcd.print("remastered.");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(1, 1);
        lcd.print("Just move the");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 2:
        lcd.setCursor(1, 0);
        lcd.print("snake using");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(0, 1);
        lcd.print("given joystick");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 3:
        lcd.setCursor(1, 0);
        lcd.print("and eat the");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(1, 1);
        lcd.print("food. On easy");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 4:
        lcd.setCursor(2, 0);
        lcd.print("it is just");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(2, 1);
        lcd.print("a simple");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 5:
        lcd.setCursor(1, 0);
        lcd.print("snake game.");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(1, 1);
        lcd.print("On medium the");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 6:
        lcd.setCursor(0, 0);
        lcd.print("speed increases");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(0, 1);
        lcd.print("and walls will");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 7:
        lcd.setCursor(2, 0);
        lcd.print("appear at");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(2, 1);
        lcd.print("score = 5.");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 8:
        lcd.setCursor(1, 0);
        lcd.print("On hard the");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(0, 1);
        lcd.print("speed increases");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 9:
        lcd.setCursor(1, 0);
        lcd.print("everytime you");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(1, 1);
        lcd.print("eat the food");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 10:
        lcd.setCursor(0, 0);
        lcd.print("and walls will");
        lcd.setCursor(15, 0);
        lcd.write(3);
        lcd.setCursor(2, 1);
        lcd.print("appear at");
        lcd.setCursor(15, 1);
        lcd.write(2);
        break;
      case 11:
        lcd.setCursor(2, 0);
        lcd.print("score = 3.");
        lcd.setCursor(0, 1);
        lcd.print("Press to go back");
        lcd.setCursor(15, 0);
        lcd.write(3);
        break;
      }

    showCustomMatrix(howToPlayMatrix);
  }
}


void menuUpDownMoves(byte menuSize) {
  if (xValue < minThreshold && yValue < maxDiagonalThreshold && yValue > minDiagonalThreshold && joyMoved == false) {
    if (currentCursorState == menuSize - 1) {
      switch (settings.sound) {
        case 0:
          break;
        case 1:
          tone(buzzerPin, buzzerWrongMove, 30);
          break;
      }
    }
    else {
      currentCursorState++;
      switch (settings.sound) {
        case 0:
          break;
        case 1:
          tone(buzzerPin, buzzerRightMove, 30);
          break;
      }

      lcd.clear();
      lc.clearDisplay(0);
      displayMainMenu();
      displaySettings();
      displayAbout();
      displayHowToPlay();
      displayDifficulty();
      displaySound();
    }

    joyMoved = true;
  }    
  if (xValue > maxThreshold && yValue < maxDiagonalThreshold && yValue > minDiagonalThreshold && joyMoved == false) {
    if (currentCursorState == 0) {
      switch (settings.sound) {
        case 0:
          break;
        case 1:
          tone(buzzerPin, buzzerWrongMove, 30);
          break;
      }
    }
    else {
      currentCursorState--; 
      switch (settings.sound) {
        case 0:
          break;
        case 1:
          tone(buzzerPin, buzzerRightMove, 30);
          break;
      }

      lcd.clear();
      lc.clearDisplay(0);
      displayMainMenu();
      displaySettings();
      displayAbout();
      displayHowToPlay();
      displayDifficulty();
      displaySound();
    }

    joyMoved = true;
  }

  if (yValue > minThreshold && yValue < maxThreshold && xValue > minThreshold && xValue < maxThreshold) {
    joyMoved = false;
  }  
}


void buttonPressed() {
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > debounceDelay) {
    if (reading != swState) {
      swState = reading;

      if (swState == LOW) {
        lastChanged = millis();
        switch (currentMenu) {
          case 0: //mainMenu
            switch (currentCursorState) {
              case 0:
                if (!playingGame) {
                  currentMenu = 11;                
                  playingGame = true;
                  switch (settings.sound) {
                    case 0:
                      break;
                    case 1:
                      tone(buzzerPin, buzzerButtonPressed, 30);
                      break;
                  }
                }
                break;
              case 1: case 2: case 3: case 4:
                currentMenu = currentCursorState;
                lastCursorState = currentCursorState;
                currentCursorState = 0;
                switch (settings.sound) {
                  case 0:
                    break;
                  case 1:
                    tone(buzzerPin, buzzerButtonPressed, 30);
                    break;
                }
                break;
            }
            break;
          case 1: // leaderboardMenu
            if (currentCursorState == leaderboardMenuSize - 1) {
              currentMenu = 0;
              currentCursorState = lastCursorState;
            }

            break;
          case 2: // settingsMenu
            switch (settings.sound) {
              case 0:
                break;
              case 1:
                tone(buzzerPin, buzzerButtonPressed, 30);
                break;
            }
            if (currentCursorState == 0) { // enterName
              currentMenu = 5;
            }

            if (currentCursorState == 1) { // difficulty
              currentMenu = 6;
              currentCursorState = 0;
            }

            if (currentCursorState == 2) { // lcdBrightness
              currentMenu = 7;
            }

            if (currentCursorState == 3) { // matBrightness
              currentMenu = 8;
            } 

            if (currentCursorState == 4) { // sound
              currentMenu = 9;
              currentCursorState = 0;
            }

            if (currentCursorState == 5) { // resetScore
              currentMenu = 10;
              currentCursorState = 0;
            }

            if (currentCursorState == settingsMenuSize - 1) {
              currentMenu = 0;
              currentCursorState = lastCursorState;
            }

            break;
          case 3: // aboutMenu
            currentMenu = 0;
            currentCursorState = lastCursorState;

            break;
          case 4: // howToPlayMenu
            if (currentCursorState == howToPlayMenuSize - 1) {
              currentMenu = 0;
              currentCursorState = lastCursorState;
            }

            break;
          case 5: //enterNameMenu
            switch (settings.sound) {
              case 0:
                break;
              case 1:
                tone(buzzerPin, buzzerButtonPressed, 30);
                break;
            }
            currentMenu = 2;
            break;
          case 6: // difficultyMenu
            switch (settings.sound) {
              case 0:
                break;
              case 1:
                tone(buzzerPin, buzzerButtonPressed, 30);
                break;
            }
            if (currentCursorState == 0) {
              settings.difficulty = 0;
              saveSettingsInEEPROM();
            }

            if (currentCursorState == 1) {
              settings.difficulty = 1;
              saveSettingsInEEPROM();
            }
            
            if (currentCursorState == 2) {
              settings.difficulty = 2;
              saveSettingsInEEPROM();
            }

            if (currentCursorState == 3) {
              currentMenu = 2;
              currentCursorState = 1;
            }

            break;
          case 7: // lcdBrightnessMenu
            switch (settings.sound) {
              case 0:
                break;
              case 1:
                tone(buzzerPin, buzzerButtonPressed, 30);
                break;
            }
            currentMenu = 2;
            saveSettingsInEEPROM();
            break;
          case 8: // matrixBrightnessMenu
            switch (settings.sound) {
              case 0:
                break;
              case 1:
                tone(buzzerPin, buzzerButtonPressed, 30);
                break;
            }
            currentMenu = 2;
            saveSettingsInEEPROM();
            break;
          case 9: // soundMenu
            switch (settings.sound) {
              case 0:
                break;
              case 1:
                tone(buzzerPin, buzzerButtonPressed, 30);
                break;
            }
            if (currentCursorState == 0) {
              settings.sound = 1;
              saveSettingsInEEPROM();
            }
            
            if (currentCursorState == 1) {
              settings.sound = 0;
              saveSettingsInEEPROM();
            }

            if (currentCursorState == 2) {
              currentMenu = 2;
              currentCursorState = 4;
            }

            break;
          case 10: // resetScoreMenu
            switch (settings.sound) {
              case 0:
                break;
              case 1:
                tone(buzzerPin, buzzerButtonPressed, 30);
                break;
            }

            if (currentCursorState == 0) {
              highscores[0] = 0;
              highscores[1] = 0;
              highscores[2] = 0;
              highscores[3] = 0;
              highscores[4] = 0;

              strcpy(highscoreNames[0], defaultPlayer);
              strcpy(highscoreNames[1], defaultPlayer);
              strcpy(highscoreNames[2], defaultPlayer);
              strcpy(highscoreNames[3], defaultPlayer);
              strcpy(highscoreNames[4], defaultPlayer);

              saveHighscoreInEEPROM();
              currentMenu = 2;
              currentCursorState = 5;
            }

            if (currentCursorState == resetScoreMenuSize - 1) {
              currentMenu = 2;
              currentCursorState = 5;
            }
            break;
        }

        lcd.clear();
        lc.clearDisplay(0);
      }
    }
  }

  lastReading = reading;
}



void scrollText(int printStart, int startLetter) {
  char *text = "   Title: Snake Remastered      By: Luca-Mihail Ion     Github: https://github.com/lucaion      Press to go back      ";
  lcd.setCursor(printStart, 1);

  for (int letter = startLetter; letter <= startLetter + 15; letter++) {
    lcd.print(text[letter]);
  }

  lcd.print(" ");
  delay(300);

}

void welcomeAnimation() {
  for (int i = 0; i < lcdWidth; i++) {
    lcd.setCursor(i, 0);
    lcd.write(5);
    delay(15);
  }

  for (int i = lcdWidth; i > -1; i--) {
    lcd.setCursor(i, 1);
    lcd.write(5);
    delay(15);
  }


  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, true); // turns on LED at col, row
      delay(12);
    }
  }

  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false); // turns off LED at col, row
      delay(12);
    }
  }

  delay(250);

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("WELCOME TO");
  
  lcd.setCursor(0, 1);
  lcd.print("Snake Remastered");
  showCustomMatrix(welcomeMatrix);

  delay(delayWelcomeVariable);
  lcd.clear();
}


struct Point {
  int row = 0, col = 0;
  Point(int row = 0, int col = 0): row(row), col(col) {}
};

bool gameOver = false;
const short initialSnakeLength = 3;
int currentScore = 0;

Point snake;
Point food(-1, -1);
Point wall(-1, -1);

int snakeLength = initialSnakeLength;
int snakeSpeed = 1;
int snakeDirection = 1;

// direction constants
const short up     = 1;
const short right  = 2;
const short down   = 3; // 'down - 2' must be 'up'
const short left   = 4; // 'left - 2' must be 'right'

int gameboard[8][8] = {};

void generateFood() {
  if (food.row == -1 || food.col == -1) {
    if (snakeLength >= 64) {
      gameOver = true;
      return;
    }

    // generate food until it is in the right position
    do {
      food.col = random(8);
      food.row = random(8);
    } while (gameboard[food.row][food.col] > 0);
  }
}

void generateWall() {
  if (wall.row == -1 || wall.col == -1) {
    if (snakeLength >= 64) {
      gameOver = true;
      return;
    }

    do {
      wall.col = random(8);
      wall.row = random(8);
    } while (gameboard[wall.row][wall.col] > 0 && gameboard[wall.row + 1][wall.col + 1] > 0 && wall.col != food.col && wall.row != food.row);
  }
}

void scanJoystick() {
  int previousDirection = snakeDirection; // save the last direction
  long timestamp = millis();

  while (millis() < timestamp + snakeSpeed) {
    if (settings.difficulty == 0) {
      snakeSpeed = 400;
    }
    else if (settings.difficulty == 1) {
      snakeSpeed = 290;
      if (snakeLength >= initialSnakeLength + 5) {
        generateWall();
      }
    }
    else if (settings.difficulty == 2) {
      snakeSpeed = 270;
      if (snakeLength >= initialSnakeLength + 3) {
        generateWall();
      }
    }
    

    // determine the direction of the snake
    yValue > maxThreshold ? snakeDirection = right : 0;
    yValue < minThreshold ? snakeDirection = left : 0;
    xValue > maxThreshold ? snakeDirection = up : 0;
    xValue < minThreshold ? snakeDirection = down : 0;

    // ignore directional change by 180 degrees
    snakeDirection + 2 == previousDirection && previousDirection != 0 ? snakeDirection = previousDirection : 0;
    snakeDirection - 2 == previousDirection && previousDirection != 0 ? snakeDirection = previousDirection : 0;

    // create blinking food
    lc.setLed(0, food.row, food.col, millis() % 100 < 50 ? 1 : 0);
    
    // create wall
    lc.setLed(0, wall.row, wall.col, 1);

    if (wall.row == 7) {
      lc.setLed(0, wall.row - 1, wall.col, 1);
    }
    else {
      lc.setLed(0, wall.row + 1, wall.col, 1);
    }
  }
}

// calculate snake movement
void calculateSnake() {
  switch (snakeDirection) {
    case up:
      snake.row--;
      wrapEdges();
      lc.setLed(0, snake.row, snake.col, 1);
      break;

    case right:
      snake.col++;
      wrapEdges();
      lc.setLed(0, snake.row, snake.col, 1);
      break;

    case down:
      snake.row++;
      wrapEdges();
      lc.setLed(0, snake.row, snake.col, 1);
      break;

    case left:
      snake.col--;
      wrapEdges();
      lc.setLed(0, snake.row, snake.col, 1);
      break;

    default: // if snake not moving => exit
      return;
  }

  // if snake hits his body then end of game
  if (gameboard[snake.row][snake.col] > 1 && snakeDirection != 0) {
    gameOver = true;
    return;
  }

  // check if the food was eaten; if yes food and wall reset
  if (snake.row == food.row && snake.col == food.col) {
    food.row = -1;
    food.col = -1;
    wall.row = -1;
    wall.col = -1;

    // increment snake length and change currentScore
    snakeLength++;
    currentScore = snakeLength - initialSnakeLength;
    if (settings.difficulty == 2) {
      snakeSpeed -= 15;
    }
    
    switch (settings.sound) {
      case 0:
        break;
      case 1:
        tone(buzzerPin, buzzerEatFood, 50);
        break;
    }

    // increment all the snake body segments
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (gameboard[row][col] > 0 ) {
          gameboard[row][col]++;
        }
      }
    }
  }

  else if (snake.row == wall.row && snake.col == wall.col) {
    gameOver = true;
    return;
  }

  // add new segment at the snake head location
  gameboard[snake.row][snake.col] = snakeLength + 1;

  // decrement all the snake body segments, if segment is 0, turn the corresponding led off
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      // if body segment => decrement it's value
      if (gameboard[row][col] > 0 ) {
        gameboard[row][col]--;
      }

      // display the current pixel
      lc.setLed(0, row, col, gameboard[row][col] == 0 ? 0 : 1);
    }
  }
}


// make the snake appear on the other side of the matrix if it gets out of the edge
void wrapEdges() {
  snake.col < 0 ? snake.col += 8 : 0;
  snake.col > 7 ? snake.col -= 8 : 0;
  snake.row < 0 ? snake.row += 8 : 0;
  snake.row > 7 ? snake.row -= 8 : 0;
}


void handleGameOver() {
  if (gameOver) {
    currentMenu = 0;
    currentCursorState = 0;

    lcd.clear(); 
    
    updateHighscores(currentScore);
    showGameOver();

    // re-init the game
    gameOver = false;
    highscoreDefeated = false;
    snake.row = random(8);
    snake.col = random(8);
    food.row = -1;
    food.col = -1;
    wall.row = -1;
    wall.col = -1;
    snakeLength = initialSnakeLength;
    snakeDirection = 1;
    currentScore = 0;
    memset(gameboard, 0, sizeof(gameboard[0][0]) * 8 * 8);
    lc.clearDisplay(0);
  }
}

void menuInGame(){
  lcd.setCursor(0, 0);
  lcd.print("Snake Remastered");
  lcd.setCursor(1, 1);
  lcd.print(String(settings.playerName));

  lcd.setCursor(9, 1);
  lcd.print(currentScore);
  buttonPressed();
  if (!playingGame) {
    lcd.clear();
  }
}

void showGameOver() {
  if (highscoreDefeated) {
    showCustomMatrix(happyFaceMatrix);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Congratulations!");
    for (int i = 0; i < 16; i++) {
      lcd.setCursor(i, 1);
      lcd.write(6);
    }
    delay(5000);
    lcd.clear();
    lc.clearDisplay(0);

    showCustomMatrix(medalMatrix);
    lcd.setCursor(1, 0);
    lcd.print("You crushed");
    lcd.setCursor(1, 1);
    lcd.print(String(defeatedPlayer));
    lcd.setCursor(9, 1);
    lcd.print("record");
    delay(5000);
    lcd.clear();

    lcd.setCursor(1, 0);
    lcd.print("You are now");
    lcd.setCursor(0, 1);
    lcd.print(String(defeatedPosition));
    lcd.setCursor(2, 1);
    lcd.print("on leaderboard");
    delay(5000);
    lcd.clear();
  }
  else {
    showCustomMatrix(sadFaceMatrix);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Unfortunately");
    for (int i = 0; i < 16; i++) {
      lcd.setCursor(i, 1);
      lcd.write(7);
    }
    delay(5000);
    lcd.clear();

    lcd.setCursor(1, 0);
    lcd.print("You didn't pass");
    lcd.setCursor(5, 1);
    lcd.print("anyone");
    delay(3000);
    lcd.clear();

    lcd.setCursor(3, 0);
    lcd.print("Try again!");
    delay(3000);
    lcd.clear();
  }
  playingGame = false;
}

void updateHighscores(int score) {
  if (score > highscores[0]) {
    highscores[4] = highscores[3];
    highscores[3] = highscores[2];
    highscores[2] = highscores[1];
    highscores[1] = highscores[0];
    highscores[0] = score;

    strcpy(highscoreNames[4], highscoreNames[3]);
    strcpy(highscoreNames[3], highscoreNames[2]);
    strcpy(highscoreNames[2], highscoreNames[1]);
    strcpy(highscoreNames[1], highscoreNames[0]);
    strcpy(defeatedPlayer, highscoreNames[0]);
    strcpy(highscoreNames[0], settings.playerName);
    defeatedPosition = 1;
    highscoreDefeated = true;
  }
  else if (score > highscores[1]) {
    highscores[4] = highscores[3];
    highscores[3] = highscores[2];
    highscores[2] = highscores[1];
    highscores[1] = score;

    strcpy(highscoreNames[4], highscoreNames[3]);
    strcpy(highscoreNames[3], highscoreNames[2]);
    strcpy(highscoreNames[2], highscoreNames[1]);
    strcpy(defeatedPlayer, highscoreNames[1]);
    strcpy(highscoreNames[1], settings.playerName);
    defeatedPosition = 2;
    highscoreDefeated = true;
  }
  else if (score > highscores[2]) {
    highscores[4] = highscores[3];
    highscores[3] = highscores[2];
    highscores[2] = score;

    strcpy(highscoreNames[4], highscoreNames[3]);
    strcpy(highscoreNames[3], highscoreNames[2]);
    strcpy(defeatedPlayer, highscoreNames[2]);
    strcpy(highscoreNames[2], settings.playerName);
    defeatedPosition = 3;
    highscoreDefeated = true;
  }
  else if (score > highscores[3]) {
    highscores[4] = highscores[3];
    highscores[3] = score;

    strcpy(highscoreNames[4], highscoreNames[3]);
    strcpy(defeatedPlayer, highscoreNames[3]);
    strcpy(highscoreNames[3], settings.playerName);
    defeatedPosition = 4;
    highscoreDefeated = true;
  }

  else if (score > highscores[4]) {
    highscores[4] = score;
    strcpy(defeatedPlayer, highscoreNames[4]);
    strcpy(highscoreNames[4], settings.playerName);
    defeatedPosition = 5;
    highscoreDefeated = true;
  }

  saveHighscoreInEEPROM();
}

void saveSettingsInEEPROM() {
  EEPROM.put(0, settings);
}

void saveHighscoreInEEPROM() {
  unsigned int eepromOffset = sizeof(settings);
  EEPROM.put(eepromOffset, highscores);
  eepromOffset += sizeof(highscores);
  EEPROM.put(eepromOffset, highscoreNames);
}

void dataFromEEPROM() {
  unsigned int eepromOffset = 0;
  EEPROM.get(eepromOffset, settings);
  eepromOffset += sizeof(settings);
  EEPROM.get(eepromOffset, highscores);
  eepromOffset += sizeof(highscores);
  EEPROM.get(eepromOffset, highscoreNames);
}