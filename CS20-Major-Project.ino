#include <Arduboy2.h>

Arduboy2 arduboy;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int x = SCREEN_WIDTH / 2;
int y = SCREEN_HEIGHT / 2;

void setup() {
  arduboy.begin();
}

void loop() {
  arduboy.clear();

  if (arduboy.pressed(LEFT_BUTTON)) {
    x--;
  } else if (arduboy.pressed(RIGHT_BUTTON)) {
    x++;
  }

  if (arduboy.pressed(UP_BUTTON)) {
    y--;
  } else if (arduboy.pressed(DOWN_BUTTON)) {
    y++;
  }

  if (x < 0) {
    x = SCREEN_WIDTH;
  } else if (x > SCREEN_WIDTH) {
    x = 0;
  }

  if (y < 0) {
    y = SCREEN_HEIGHT;
  } else if (y > SCREEN_HEIGHT) {
    y = 0;
  }

  arduboy.drawRect(x,y,4,4,WHITE);

  arduboy.display();
}
