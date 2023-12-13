//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
// PossiblyAxolotl                                      \\
// CS20 Final Project - Currently Unsure of End Goal    //
//                                                      \\
// started Oct 24, 2023                                 //
// Last modified Dec 13, 2023                            \\
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#include <Arduboy2.h>
#include <Sprites.h>
#include "./art/player_art.h"
#include "./art/tile_art.h"
#include "./art/enemy_art.h"

Arduboy2 arduboy;
Sprites sprites;

// this code MUST be done in this order, completely ruins all my #include s up there
#define ARDBITMAP_SBUF arduboy.getBuffer()
#include <ArdBitmap.h>
ArdBitmap<WIDTH, HEIGHT> ardbitmap;

/// define constants
// player
#define MOVE_SPEED 1
#define PLAYER_ANIM_WAIT_TIME 8

#define PLAYER_MIN_X 9
#define PLAYER_MAX_X 119

#define PLAYER_MIN_Y 16
#define PLAYER_MAX_Y 56

// enemy spawns
#define ENEMY_SPAWN_MIN_X 20
#define ENEMY_SPAWN_MIN_Y 20

#define ENEMY_SPAWN_MAX_X WIDTH-20
#define ENEMY_SPAWN_MAX_Y HEIGHT-20

// skeleton
#define SKELE_MAX_MOVE_DISTANCE 9
#define SKELE_WAIT_TIME 30
#define SKELE_MOVE_SPEED 2

// define variables
float player_x = WIDTH/2;
float player_y = HEIGHT/2;

// player animation variables
int player_frame = 0;
int player_direction = 0;
int player_anim_timer = PLAYER_ANIM_WAIT_TIME;

//misc game variables
int state = 2; // 1 = game
int transition_offset = 128;

const int player_directions[3][3] = { 
  {2,2,2},
  {1,1,3},
  {0,0,0}
};

/// define enemy classes
// base enemy class
class Enemy {
  public:
    int x = random(ENEMY_SPAWN_MIN_X,ENEMY_SPAWN_MAX_X);
    int y = random(ENEMY_SPAWN_MIN_Y, ENEMY_SPAWN_MAX_Y);
    
    int wait_time;
    int frame = 0;
    int mirror = MIRROR_NONE;

    bool alive = true;

    // wait for wait time, move by move speed until total distance travelled >= move_distance
};

// skeleton
class Skeleton : private Enemy {
  private:
    void setup() {
      wait_time = random(10,50);
      Serial.println("setup");
    }
  public:
    Skeleton() {
      setup();
    }
    
    void update(int to_x, int to_y) {
      if (alive) {
        ardbitmap.drawBitmap(x, y, skele_frames[frame], SKELE_SPRITE_WIDTH, SKELE_SPRITE_HEIGHT, WHITE, ALIGN_CENTER, mirror);

        wait_time --;

        if (wait_time < 0) {
          wait_time = SKELE_WAIT_TIME;

          // animate skeleton
          frame ++;
          if (frame > SKELE_SPRITE_FRAME_AMOUNT) {
            frame = 0;
          }

          // move on x
          if (to_x > x) {
            x += SKELE_MOVE_SPEED;
            mirror = MIRROR_HORIZONTAL;
          } else if (to_x < x) {
            x -= SKELE_MOVE_SPEED;
            mirror = MIRROR_NONE;
          }

          // move on y
          if (to_y > y) {
            y += SKELE_MOVE_SPEED;
          } else if (to_y < y) {
            y -= SKELE_MOVE_SPEED;
          }
        }
      } else {
        // draw pile of bones in spot
      }
    }
};

class Trapdoor {
  private:
    int x = random(ENEMY_SPAWN_MIN_X, ENEMY_SPAWN_MAX_X);
    int y = random(ENEMY_SPAWN_MIN_Y, ENEMY_SPAWN_MAX_Y);

  public:
    bool closed = true;

    void draw() {
      arduboy.drawBitmap(x, y, trapdoor_frames[(int)closed], TRAPDOOR_SPRITE_WIDTH, TRAPDOOR_SPRITE_HEIGHT);
    }

    void reset() {
      x = random(ENEMY_SPAWN_MIN_X, ENEMY_SPAWN_MAX_X);
      y = random(ENEMY_SPAWN_MIN_Y, ENEMY_SPAWN_MAX_Y);
      closed = true;
    }
};

// create a list of skeletons as a pointer so I can change the length and stuff later
Skeleton* skeletons;

Trapdoor trapdoor;

// set up game, run once
void setup() {
  randomSeed(analogRead(0)); // randomize the game on arduboy, since computers don't have pins it's always the same

  skeletons = new Skeleton[number_of_skeletons];

  Serial.begin(9600);

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
  if (player_x < PLAYER_MIN_X) {
    player_x = PLAYER_MIN_X;
  } else if (player_x > PLAYER_MAX_X) {
    player_x = PLAYER_MAX_X;
  }

  // limit player y position
  if (player_y < PLAYER_MIN_Y) {
    player_y = PLAYER_MIN_Y;
  } else if (player_y > PLAYER_MAX_Y) {
    player_y = PLAYER_MAX_Y;
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
  trapdoor.draw();

  sprites.drawOverwrite(player_x - PLAYER_SPRITE_WIDTH/2, player_y - PLAYER_SPRITE_HEIGHT/2, player_frames[player_direction], player_frame);
  
  drawRoom();

  for (int i = 0; i < number_of_skeletons; i++) {
    skeletons[i].update(player_x, player_y);
  }
  
  arduboy.drawPixel(20,20);

  arduboy.drawPixel(108,44);

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