#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS -1
#define TFT_DC D4
#define TFT_RST D5
Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);

#define Up D0
#define Right D1
#define Down D2
#define Left D3

bool gameOver = false;
int score = 0;

int playerX = 120;
int playerY = 210;

int blockY = 30;
int blockX = 0;

void drawScore(){
  tft.fillRect(0, 0, 240, 30, ST77XX_WHITE);
  tft.fillRect(0, 0, 120, 25, ST77XX_WHITE);
  tft.setCursor(10, 5);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(3);
  tft.print("Score: ");
  tft.print(score);
}
void startGame(){
  gameOver = false;
  score = 0;
  playerX = 120;
  playerY = 210;

  tft.fillScreen(ST77XX_BLACK);
  drawScore();
  tft.fillRect(playerX, playerY, 20, 20, ST77XX_YELLOW);

  spawnBlock();
}
void setup() {
  randomSeed(micros());

  pinMode(Up, INPUT_PULLUP);
  pinMode(Right, INPUT_PULLUP);
  pinMode(Down, INPUT_PULLUP);
  pinMode(Left, INPUT_PULLUP);

  tft.init(240, 240, SPI_MODE3);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);

  startGame();

}
void loop() {
  if (gameOver){
    if (digitalRead(Up) == LOW || digitalRead(Right) == LOW || digitalRead(Down) == LOW || digitalRead(Left) == LOW){
      startGame();
    }
    return;
  }
  moveBlock();
  ifs();

  if (blockY >= 240){
    score++;
    drawScore();
    spawnBlock();
  }
}

void spawnBlock(){
  blockX = random(0, 200);
  blockY = 30;
}
void moveBlock(){
  tft.fillRect(blockX, blockY, 40, 2, ST77XX_BLACK);
  blockY += 2;

  tft.fillRect(playerX, playerY, 20, 20, ST77XX_YELLOW);
  tft.fillRect(blockX, blockY, 40, 40, ST77XX_RED);
  if (score < 50){
    delay(5);
  }
  else if (score >= 50 && score <= 99){
    delay(4);
  }
  else if (score >= 100){
    delay(3);
  }

}

void ifs(){
  if (digitalRead(Up) == LOW && playerY > 30){
    tft.fillRect(playerX, playerY + 18, 20, 2, ST77XX_BLACK);
    playerY -= 2;
    tft.fillRect(playerX, playerY, 20, 20, ST77XX_YELLOW);
  }
  if (digitalRead(Down) == LOW && playerY < 220){
    tft.fillRect(playerX, playerY, 20, 2, ST77XX_BLACK);
    playerY += 2;
    tft.fillRect(playerX, playerY, 20, 20, ST77XX_YELLOW);
  }
  if (digitalRead(Left) == LOW && playerX > 0){
    tft.fillRect(playerX + 18, playerY, 2, 20, ST77XX_BLACK);
    playerX -= 2;
    tft.fillRect(playerX, playerY, 20, 20, ST77XX_YELLOW);
  }
  if (digitalRead(Right) == LOW && playerX < 220){
    tft.fillRect(playerX, playerY, 2, 20, ST77XX_BLACK);
    playerX += 2;
    tft.fillRect(playerX, playerY, 20, 20, ST77XX_YELLOW);
  }

  if (playerX + 20 > blockX && playerX < blockX + 40 && playerY + 20 > blockY && playerY < blockY + 40){
    endGame();
    return;
  }

}

void endGame(){
  gameOver = true;

  tft.fillScreen(ST77XX_RED);
  tft.setCursor(35, 80);
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_BLACK);
  tft.print("GAME OVER");

  tft.setCursor(70, 130);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("Score: ");
  tft.print(score);

  tft.setCursor(35, 200);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("press any button to restart");

  delay(700);
}