// PossiblyAxolotl
// CS20 Final Project - Currently Unsure of End Goal
// started Oct 24, 2023
// Last modified Oct 25, 2023

// 128x64 screen

#include <Arduboy2.h>

Arduboy2 arduboy;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class Bubble {
  private:
    int x = random(0,SCREEN_WIDTH);
    int y = random(0,SCREEN_HEIGHT);
    int size = random(2,5);
    float speed = ((float)random(1, 30)) / 100;
  public:

    void update() {
      y -= speed;
      if (y < 1) {
        y = SCREEN_HEIGHT;
      }
    };

    void draw() {
      arduboy.drawCircle(x, y, size, WHITE);
    };
};

Bubble bubbles[30];

void setup() {
  arduboy.begin();
}

void loop() {
  arduboy.clear();

  for (int i = 0; i < 30; i ++) {
    Bubble bub = bubbles[i];

    bub.update();
    bub.draw();

    bubbles[i] = bub;
  }

  arduboy.display();
}
