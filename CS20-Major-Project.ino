#include <Arduboy2.h>

Arduboy2 arduboy;

int x = 0;
int dir = 1;

void setup() {
  arduboy.begin();
}

void loop() {
  if (x > 128 || x < 0) {
    dir *= -1;
  }

  arduboy.clear();
  arduboy.drawPixel(x, 5);
  x+= dir;
  arduboy.display();
}
