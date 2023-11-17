// PossiblyAxolotl
// CS20 Final Project - Currently Unsure of End Goal
// started Oct 24, 2023
// Last modified Nov 17, 2023

// 128x64 screen

#include <Arduboy2.h>

Arduboy2 arduboy;

// define constants
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define MOVE_SPEED 0.1f

// define variables
float player_x = SCREEN_WIDTH/2;
float player_y = SCREEN_HEIGHT/2;

// set up game, run once
void setup() {
  arduboy.begin();
}

// mainloop
void loop() {
  arduboy.clear(); // clear screen every frame

  int x_input = (int)arduboy.pressed(RIGHT_BUTTON) - (int)arduboy.pressed(LEFT_BUTTON);
  int y_input = (int)arduboy.pressed(DOWN_BUTTON) - (int)arduboy.pressed(UP_BUTTON);

  player_x += x_input * MOVE_SPEED;
  player_y += y_input * MOVE_SPEED;

  arduboy.fillRect(player_x, player_y, 2, 2, WHITE);

  arduboy.display(); // update screen to display changes
}