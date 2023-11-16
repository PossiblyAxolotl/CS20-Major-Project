// PossiblyAxolotl
// CS20 Final Project - Currently Unsure of End Goal
// started Oct 24, 2023
// Last modified Nov 16, 2023

// 128x64 screen

#include <Arduboy2.h>

Arduboy2 arduboy;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int player_x = SCREEN_WIDTH/2;
int player_y = SCREEN_HEIGHT/2;

void setup() {
  arduboy.begin();
}

void loop() {
  arduboy.clear();

  if (arduboy.pressed(RIGHT_BUTTON)) {
    player_x += 1;
  } else if (arduboy.pressed(LEFT_BUTTON)) {
    player_x -= 1;
  }

  if (arduboy.pressed(DOWN_BUTTON)) {
    player_y += 1;
  } else if (arduboy.pressed(UP_BUTTON)) {
    player_y -= 1;
  }


  arduboy.fillRect(player_x, player_y, 2, 2, WHITE);

  arduboy.display();
}
