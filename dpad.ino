#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SNAKE_SIZE 4
#define FOOD_SIZE 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button pins
#define BTN_UP    2
#define BTN_DOWN  3
#define BTN_LEFT  4
#define BTN_RIGHT 5
#define RED_LED 16
#define BLUE_LED 15
#define GREEN_LED 14

// Snake data
int snakeX[64] = {40, 36, 32, 28}; 
int snakeY[64] = {32, 32, 32, 32};
int snakeLength = 4;

// Food
int test_foodx[1] = {0};
int test_foody[1] = {0};

// LED logic
int ledlogic = 0;

// Snake direction
enum Direction {UP, DOWN, LEFT, RIGHT};
Direction dir = RIGHT;

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 failed");
    while (true);
  }

  display.clearDisplay();
  display.display();

  // Seed random number generator
  randomSeed(analogRead(A0));

  // Initial food position (aligned to grid)
  test_foodx[0] = random(0, SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;
  test_foody[0] = random(0, SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;
}

void loop() {
  move();
  draw();
  food();
  
  leds();
  delay(150);
}

void leds() {
  if (ledlogic == 0) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  } else if (ledlogic == 1) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
  } else if (ledlogic == 2) {
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    ledlogic = -1;
  }
  ledlogic++;
}

void move() {
  if (digitalRead(BTN_UP) == LOW && dir != DOWN) dir = UP;
  if (digitalRead(BTN_DOWN) == LOW && dir != UP) dir = DOWN;
  if (digitalRead(BTN_LEFT) == LOW && dir != RIGHT) dir = LEFT;
  if (digitalRead(BTN_RIGHT) == LOW && dir != LEFT) dir = RIGHT;

  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  switch (dir) {
    case UP:    snakeY[0] -= SNAKE_SIZE; break;
    case DOWN:  snakeY[0] += SNAKE_SIZE; break;
    case LEFT:  snakeX[0] -= SNAKE_SIZE; break;
    case RIGHT: snakeX[0] += SNAKE_SIZE; break;
  }

  // Wrap around screen
  if (snakeX[0] < 0) snakeX[0] = SCREEN_WIDTH - SNAKE_SIZE;
  if (snakeX[0] >= SCREEN_WIDTH) snakeX[0] = 0;
  if (snakeY[0] < 0) snakeY[0] = SCREEN_HEIGHT - SNAKE_SIZE;
  if (snakeY[0] >= SCREEN_HEIGHT) snakeY[0] = 0;
}

void draw() {
  display.clearDisplay();

  for (int i = 0; i < snakeLength; i++) {
    display.fillRect(snakeX[i], snakeY[i], SNAKE_SIZE, SNAKE_SIZE, SSD1306_WHITE);
  }

  display.fillRect(test_foodx[0], test_foody[0], FOOD_SIZE, FOOD_SIZE, SSD1306_WHITE);
  display.display();
}

void food() {
  if (snakeX[0] == test_foodx[0] && snakeY[0] == test_foody[0]) {
    // Grow snake
    snakeLength++;

    // Move food to new random location (aligned to grid)
    test_foodx[0] = random(0, SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;
    test_foody[0] = random(0, SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;
  }
}
