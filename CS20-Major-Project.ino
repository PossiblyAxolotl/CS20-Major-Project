//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
// PossiblyAxolotl                                      \\
// CS20 Final Project - Currently Unsure of End Goal    //
//                                                      \\
// started Oct 24, 2023                                 //
// Last modified Dec 11, 2023                            \\
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#include <Arduboy2.h>
#include "./art/player_art.h"
#include "./art/tile_art.h"
#include "./art/enemy_art.h"

Arduboy2 arduboy;

// define constants
#define MOVE_SPEED 1
#define PLAYER_ANIM_WAIT_TIME 8

// define variables
float player_x = WIDTH/2;
float player_y = HEIGHT/2;

// player animation variables
int player_frame = 0;
int player_direction = 0;
int player_anim_timer = PLAYER_ANIM_WAIT_TIME;

int state = 2; // 1 = game
int transition_offset = 128;

const int player_directions[3][3] = { // temp values as not all art is made yet
  {2,2,2},
  {1,1,3},
  {0,0,0}
};

class Skeleton {
  private:
    int x = random(20,108);
    int y = random(20, 44);
    
    int wait_time = 30;
    int move_speed = 2;
    int move_distance = 6;
    int total_distance = 0;

    // wait for wait time, move by move speed until total distance travelled >= move_distance

  public:
    void update(int to_x, int to_y) {
      arduboy.drawPixel(x, y);
    }
};

Skeleton* skeletons;

// set up game, run once
void setup() {
  randomSeed(analogRead(0));

  skeletons = new Skeleton[2];

  arduboy.begin();
  arduboy.setFrameRate(60);
}

/// Player
// animate player based on direction
void animatePlayer() {
  player_anim_timer --;

  if (player_anim_timer < 1) {
    player_frame ++;

    if (player_frame > PLAYER_SPRITE_FRAME_AMOUNT) {
      player_frame  = 0;
    }

    player_anim_timer = PLAYER_ANIM_WAIT_TIME;
  }
}

// clamp player bounds
void clampPlayerToBounds() {
  // limit player x position
  if (player_x < 9) {
    player_x = 9;
  } else if (player_x > 119) {
    player_x = 119;
  }

  // limit player y position
  if (player_y < 16) {
    player_y = 16;
  } else if (player_y > 56) {
    player_y = 56;
  }
}

/// other drawing
void drawRoom(int offset = 0) {
  // draw outline (intentionally too tall)
  arduboy.drawRect(0+offset,0,128,65);

  // draw extra line that makes the back 2 thick
  arduboy.drawLine(1+offset,1,126+offset,1);

  // draw back rect
  arduboy.fillRect(2+offset,3,124,4);
}

// modes
void processGame() {
// didn't indent this as everything will be in here, keeps constant framerate
if (arduboy.nextFrame()) {
  arduboy.clear(); // clear screen every frame

  // get player input
  int x_input = (int)arduboy.pressed(RIGHT_BUTTON) - (int)arduboy.pressed(LEFT_BUTTON);
  int y_input = (int)arduboy.pressed(DOWN_BUTTON) - (int)arduboy.pressed(UP_BUTTON);

  if (x_input != 0 || y_input != 0) { // if moving
    // player animation
    animatePlayer();

    player_direction = player_directions[y_input+1][x_input+1];

    // move player and collide
    player_x += x_input * MOVE_SPEED;
    player_y += y_input * MOVE_SPEED;

  } else if (player_frame != 0 && player_anim_timer != PLAYER_ANIM_WAIT_TIME) { // just stopped
    player_frame = 0;
    player_anim_timer = PLAYER_ANIM_WAIT_TIME;
  }

  clampPlayerToBounds();

  /// drawing
  drawRoom();

  skeletons[1].update(player_x, player_y);

  // player draw
  arduboy.drawBitmap(player_x - PLAYER_SPRITE_WIDTH/2, player_y - PLAYER_SPRITE_HEIGHT/2, player_animation_frames[player_direction][player_frame], PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT);

  arduboy.display(); // update screen to display changes
}
}

void processTransition() {
if (arduboy.nextFrame()) {
  // clear screen
  arduboy.clear();

  // draw the room twice for scrolling animation
  drawRoom(transition_offset);
  drawRoom(transition_offset-128);

  // scroll until the screen is in the right place, then go to game mode
  if (transition_offset > 0) {
    transition_offset -= 1;
  } else {
    state = 1;
  }

  arduboy.display();
}
}

// mainloop
void loop() {
  if (state == 1) {
    processGame();
  } else if (state == 2) {
    processTransition();
  }
}