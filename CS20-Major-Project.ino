//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
// PossiblyAxolotl                                      \\
// CS20 Final Project - Currently Unsure of End Goal    //
//                                                      \\
// started Oct 24, 2023                                 //
// Last modified Nov 17, 2023                           \\
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#include <Arduboy2.h>
#include "player_art.h"

Arduboy2 arduboy;

// define constants
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define MOVE_SPEED 0.1f

#define PLAYER_ANIM_WAIT_TIME 60

// define variables
float player_x = SCREEN_WIDTH/2;
float player_y = SCREEN_HEIGHT/2;

// player animation variables
int player_frame = 0;
int player_direction = 0;
int player_anim_timer = PLAYER_ANIM_WAIT_TIME;

const int player_directions[3][3] PROGMEM = { // temp values as not all art is made yet
  {1,1,1},
  {1,1,1},
  {0,0,0}
};

// set up game, run once
void setup() {
  arduboy.begin();
}

void animate() {
  player_anim_timer --;

  if (player_anim_timer < 1) {
    player_frame ++;

    if (player_frame > PLAYER_SPRITE_FRAME_AMOUNT) {
      player_frame  = 0;
    }

    player_anim_timer = PLAYER_ANIM_WAIT_TIME;
  }
}

// mainloop
void loop() {
  arduboy.clear(); // clear screen every frame

  int x_input = (int)arduboy.pressed(RIGHT_BUTTON) - (int)arduboy.pressed(LEFT_BUTTON);
  int y_input = (int)arduboy.pressed(DOWN_BUTTON) - (int)arduboy.pressed(UP_BUTTON);

  if (x_input != 0 || y_input != 0) { // moving
    animate();

    player_direction = player_directions[y_input+1][x_input+1];

    player_x += x_input * MOVE_SPEED;
    player_y += y_input * MOVE_SPEED;
  } else if (player_frame != 0 && player_anim_timer != PLAYER_ANIM_WAIT_TIME) { // just stopped
    player_frame = 0;
    player_anim_timer = PLAYER_ANIM_WAIT_TIME;
  }

  // DRAWING
  arduboy.drawBitmap(player_x - PLAYER_SPRITE_WIDTH/2, player_y - PLAYER_SPRITE_HEIGHT/2, player_animation_frames[player_direction][player_frame], 18, 16);

  arduboy.display(); // update screen to display changes
}