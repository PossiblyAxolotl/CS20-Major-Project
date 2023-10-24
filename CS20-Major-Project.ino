#include <Arduboy2.h>

Arduboy2 arduboy;

int x = 0;

void setup() {
  arduboy.begin();
}

void loop() {
  arduboy.drawPixel(x, 5);
  x++;
  arduboy.display();
}
