//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
// PossiblyAxolotl                                      \\
// CS20 Final Project - Currently Unsure of End Goal    //
//                                                      \\
// started Oct 24, 2023                                 //
// Last modified Nov 18, 2023                           \\
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#include <Arduboy2.h>
#include "./art/player_art.h"

Arduboy2 arduboy;

// define constants
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define MOVE_SPEED 1

#define PLAYER_ANIM_WAIT_TIME 8

// define variables
float player_x = SCREEN_WIDTH/2;
float player_y = SCREEN_HEIGHT/2;

// player animation variables
int player_frame = 0;
int player_direction = 0;
int player_anim_timer = PLAYER_ANIM_WAIT_TIME;

const int player_directions[3][3] = { // temp values as not all art is made yet
  {2,2,2},
  {1,1,3},
  {0,0,0}
};

// set up game, run once
void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
}

// animate player based on direction
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
// didn't indent this as everything will be in here, keeps constant framerate
if (arduboy.nextFrame()) {
  arduboy.clear(); // clear screen every frame

  // get player input
  int x_input = (int)arduboy.pressed(RIGHT_BUTTON) - (int)arduboy.pressed(LEFT_BUTTON);
  int y_input = (int)arduboy.pressed(DOWN_BUTTON) - (int)arduboy.pressed(UP_BUTTON);
  
  if (x_input != 0 || y_input != 0) { // if moving
    // animation
    animate();

    player_direction = player_directions[y_input+1][x_input+1];

    // move player and collide
    player_x += x_input * MOVE_SPEED;
    player_y += y_input * MOVE_SPEED;

  } else if (player_frame != 0 && player_anim_timer != PLAYER_ANIM_WAIT_TIME) { // just stopped
    player_frame = 0;
    player_anim_timer = PLAYER_ANIM_WAIT_TIME;
  }

  // drawing
  arduboy.drawBitmap(player_x - PLAYER_SPRITE_WIDTH/2, player_y - PLAYER_SPRITE_HEIGHT/2, player_animation_frames[player_direction][player_frame], PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT);

  arduboy.display(); // update screen to display changes
}
}