#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ==== Pini butoane ====
#define BTN_UP     2
#define BTN_DOWN   4
#define BTN_LEFT   16
#define BTN_RIGHT  17

// ==== Buzzer ====
#define BUZZER 15

// ==== Setări joc ====
#define CELL_SIZE 4
#define MAX_SNAKE 200

int snakeX[MAX_SNAKE];
int snakeY[MAX_SNAKE];
int snakeLength;
int direction; // 0=dreapta, 1=jos, 2=stanga, 3=sus

int foodX, foodY;
int score;
bool gameRunning = true;

unsigned long lastMove = 0;
int gameSpeed = 120;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  randomSeed(analogRead(34));
  initGame();
}

void loop() {

  if (!gameRunning) {
    if (digitalRead(BTN_UP) == LOW) {
      initGame();
    }
    return;
  }

  handleInput();

  if (millis() - lastMove > gameSpeed) {
    lastMove = millis();
    moveSnake();
    checkCollision();
    checkFood();
    drawGame();
  }
}

void initGame() {
  snakeLength = 3;
  direction = 0;
  score = 0;
  gameRunning = true;

  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = 40 - i * CELL_SIZE;
    snakeY[i] = 30;
  }

  generateFood();
  tone(BUZZER, 1000, 100);
}

void generateFood() {
  foodX = random(0, SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
  foodY = random(0, SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
}

void handleInput() {

  if (digitalRead(BTN_UP) == LOW && direction != 1) {
    direction = 3;
  }
  else if (digitalRead(BTN_DOWN) == LOW && direction != 3) {
    direction = 1;
  }
  else if (digitalRead(BTN_LEFT) == LOW && direction != 0) {
    direction = 2;
  }
  else if (digitalRead(BTN_RIGHT) == LOW && direction != 2) {
    direction = 0;
  }
}

void moveSnake() {

  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  switch (direction) {
    case 0: snakeX[0] += CELL_SIZE; break;
    case 1: snakeY[0] += CELL_SIZE; break;
    case 2: snakeX[0] -= CELL_SIZE; break;
    case 3: snakeY[0] -= CELL_SIZE; break;
  }
}

void checkCollision() {

  // margini
  if (snakeX[0] < 0 || snakeX[0] >= SCREEN_WIDTH ||
      snakeY[0] < 0 || snakeY[0] >= SCREEN_HEIGHT) {
    gameOver();
  }

  // corp
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameOver();
    }
  }
}

void checkFood() {

  if (snakeX[0] == foodX && snakeY[0] == foodY) {

    if (snakeLength < MAX_SNAKE)
      snakeLength++;

    score += 5;
    generateFood();
    tone(BUZZER, 1500, 80);
  }
}

void drawGame() {

  display.clearDisplay();

  // snake
  for (int i = 0; i < snakeLength; i++) {
    display.fillRect(snakeX[i], snakeY[i], CELL_SIZE, CELL_SIZE, WHITE);
  }

  // food
  display.fillRect(foodX, foodY, CELL_SIZE, CELL_SIZE, WHITE);

  // scor
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Scor:");
  display.print(score);

  display.display();
}

void gameOver() {

  tone(BUZZER, 300, 300);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 20);
  display.print("GAME OVER");
  display.setTextSize(1);
  display.setCursor(30, 45);
  display.print("Scor: ");
  display.print(score);
  display.display();

  gameRunning = false;
}
