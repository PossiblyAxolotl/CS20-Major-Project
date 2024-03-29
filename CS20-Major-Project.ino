//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
// PossiblyAxolotl                                      \\
// CS20 Final Project - Generic Dungeon Game            //
//                                                      \\
// started Oct 24, 2023                                 //
// Last modified Jan 19, 2024                           \\
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

//// todo
// [X] make dynamic level thing
// [X] death
// [X] proper title screen and title
// [X] work with multiple skeles properly
// [X] shorten sword out time a bit
// [X] add sword recharge
// [X] draw pile of bones where skele died

// [X] add instructions
// [X] create overview and other info for log
// [X] upload finished build to github pages
// [X] submit to Mr. Schellenberg

#include <Arduboy2.h>
#include <Sprites.h>
#include "./art/player_art.h"
#include "./art/tile_art.h"
#include "./art/enemy_art.h"
#include "./art/misc_art.h"

Arduboy2 arduboy;
Sprites sprites;

// this code MUST be done in this order, completely ruins all my #include stuff up there
#define ARDBITMAP_SBUF arduboy.getBuffer()
#include <ArdBitmap.h>
ArdBitmap<WIDTH, HEIGHT> ardbitmap;

/// define constants
// player
#define MOVE_SPEED 1
#define PLAYER_ANIM_WAIT_TIME 8

#define PLAYER_MIN_X 9
#define PLAYER_MAX_X 119

#define PLAYER_MIN_Y 10
#define PLAYER_MAX_Y 56

// hitboxes
#define SWORD_HITBOX 6
#define PLAYER_HITBOX 4

// enemy spawns
#define ENEMY_SPAWN_MIN_X 20
#define ENEMY_SPAWN_MIN_Y 20

#define ENEMY_SPAWN_MAX_X WIDTH-20
#define ENEMY_SPAWN_MAX_Y HEIGHT-20

// skeleton
#define SKELE_MAX_MOVE_DISTANCE 9
#define SKELE_WAIT_TIME 30
#define SKELE_MOVE_SPEED 2

#define SKELE_MAX_AMOUNT 5

// define variables
float player_x = WIDTH/2;
float player_y = HEIGHT/2;
int player_health = 2;

int player_invincibility_frames = 0;

// skele stuff
int number_of_skeletons = 1;
int skeletons_left = 1;
float skele_health = 1.0f;

// player animation variables
int player_frame = 0;
int player_direction = 0;
int player_sword_time = 0;
int player_sword_recharge = 0;
int player_anim_timer = PLAYER_ANIM_WAIT_TIME;

//misc game variables
int state = 0; // 0 = title, 1 = game, 2 = transition
int transition_offset = 128;

int game_level = 1;

// declared now, defined after certain vars are created
void clearAll();

const int player_directions[3][3] = { 
  {2,2,2},
  {1,0,3},
  {0,0,0}
};

const int sword_positions[4][3] = { // x, y, mirror
  {0,10, MIRROR_NONE}, // down
  {-9,1, MIRROR_HORIZONTAL}, // left
  {0,-10, MIRROR_NONE}, // up
  {9,1, MIRROR_NONE}  // right
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
};

// skeleton
class Skeleton : private Enemy {
  private:
    void setup() {
      wait_time = random(10,50);
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
          wait_time = SKELE_WAIT_TIME - game_level;
          wait_time = max(wait_time, 10);

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

        int skele_topleft[2]  = {x - SKELE_SPRITE_WIDTH / 2, y - SKELE_SPRITE_HEIGHT / 2};
        int skele_botright[2] = {x + SKELE_SPRITE_WIDTH / 2, y + SKELE_SPRITE_HEIGHT / 2};

        if (player_sword_time > 0) { // player is attacking
          int sword_topleft[2]  = {player_x + sword_positions[player_direction][0] - SWORD_HITBOX, player_y + sword_positions[player_direction][1] - SWORD_HITBOX};
          int sword_botright[2] = {player_x + sword_positions[player_direction][0] + SWORD_HITBOX, player_y + sword_positions[player_direction][1] + SWORD_HITBOX};

          // detect if rects overlap
          if ((skele_topleft[1] < sword_botright[1]) && (skele_topleft[0] < sword_botright[0])) {
            if ((skele_botright[1] > sword_topleft[1]) && (skele_botright[0] > sword_topleft[0])) {
              alive = false;
              skeletons_left --;
            }
          }
        } else if (player_invincibility_frames <= 0) { // player can't get hurt if attacking, otherwise do:
          int player_topleft[2]  = {player_x - PLAYER_HITBOX/2, player_y - PLAYER_HITBOX/2};
          int player_botright[2] = {player_x + PLAYER_HITBOX/2, player_y + PLAYER_HITBOX/2};

          // detect if rects overlap
          if ((skele_topleft[1] < player_botright[1]) && (skele_topleft[0] < player_botright[0])) {
            if ((skele_botright[1] > player_topleft[1]) && (skele_botright[0] > player_topleft[0])) {
              player_health--;
              player_invincibility_frames = 30;

              if (player_health < 1) {
                clearAll();
              }
            }
          }
        }
      } else {
        ardbitmap.drawBitmap(x, y, dead_skele, SKELE_SPRITE_WIDTH, SKELE_SPRITE_HEIGHT, WHITE, ALIGN_CENTER, mirror);
      }
    }
};

class Trapdoor {
  private:
    int x = random(ENEMY_SPAWN_MIN_X, ENEMY_SPAWN_MAX_X);
    int y = random(ENEMY_SPAWN_MIN_Y, ENEMY_SPAWN_MAX_Y);

  public:
    void draw() {
      arduboy.drawBitmap(x, y, trapdoor_frames[(int)(skeletons_left > 0)], TRAPDOOR_SPRITE_WIDTH, TRAPDOOR_SPRITE_HEIGHT);

      if (skeletons_left < 1) {
        int door_topleft[2]  = {x, y};
        int door_botright[2] = {x + TRAPDOOR_SPRITE_WIDTH, y + TRAPDOOR_SPRITE_HEIGHT};

        int player_topleft[2]  = {player_x - PLAYER_SPRITE_WIDTH/2, player_y - PLAYER_SPRITE_HEIGHT/2};
        int player_botright[2] = {player_x + PLAYER_SPRITE_WIDTH/2, player_y + PLAYER_SPRITE_HEIGHT/2};

        // detect if rects overlap
        if ((door_topleft[1] < player_botright[1]) && (door_topleft[0] < player_botright[0])) {
          if ((door_botright[1] > player_topleft[1]) && (door_botright[0] > player_topleft[0])) { // if player is on open door
            state = 2;
            transition_offset = 128;
            reset();
          }
        }
      }
    }

    void reset() {
      x = random(ENEMY_SPAWN_MIN_X, ENEMY_SPAWN_MAX_X);
      y = random(ENEMY_SPAWN_MIN_Y, ENEMY_SPAWN_MAX_Y);
    }
};

// create a list of skeletons as a pointer so I can change the length and stuff later
Skeleton* skeletons;

Trapdoor trapdoor;

// set up next level (trapdoor does automatically)
void nextLevel() {
  delete[] skeletons;

  game_level++;

  // SET NUM BASED ON LEVEL
  number_of_skeletons = ceil(game_level / 4 + 0.1);
  if (number_of_skeletons > SKELE_MAX_AMOUNT) {
    number_of_skeletons = SKELE_MAX_AMOUNT;
  }

  skeletons_left = number_of_skeletons;

  skeletons = new Skeleton[number_of_skeletons];

  player_invincibility_frames = 60;
  player_sword_time = 0;
  player_sword_recharge = 30;
}

// die 
void clearAll() {
  delete[] skeletons;
  state = 0;
}

bool inputButton() {
  return arduboy.anyPressed(A_BUTTON | B_BUTTON);
}

// set up game, run once
void setup() {
  randomSeed(analogRead(A0)); // randomize the game on arduboy, since computers don't have pins it's always the same

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
}

void drawRoomWall(int offset = 0) {
    // draw back for room
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

  if (player_sword_time < 1) { // if you're not attacking
    // only run if moving
    if (x_input != 0 || y_input != 0) { 
      // player animation
      animatePlayer();

      player_direction = player_directions[y_input+1][x_input+1];

      // move player and collide
      player_x += x_input * MOVE_SPEED;
      player_y += y_input * MOVE_SPEED;

    // if you JUST stopped moving
    } else if (player_frame != 0 && player_anim_timer != PLAYER_ANIM_WAIT_TIME) {
      player_frame = 0;
      player_anim_timer = PLAYER_ANIM_WAIT_TIME;
    }

    // check if player tries to attack
    if (player_sword_recharge <= 0) {
      if (inputButton()) {
        player_sword_time = 15;
        player_sword_recharge = 30;
      }
    } else {
      player_sword_recharge--;
    }
    
  } else { // if you are attacking
    player_sword_time--;
  }

  if (player_invincibility_frames > 0) {
    player_invincibility_frames --;
  }

  clampPlayerToBounds();

  /// drawing
  trapdoor.draw();

  // draw back wall first since player can overlap
  drawRoomWall();

  // draw player
  sprites.drawOverwrite(player_x - PLAYER_SPRITE_WIDTH/2, player_y - PLAYER_SPRITE_HEIGHT/2, player_frames[player_direction], player_frame);

  // draw room outline
  drawRoom();

  // draw enemies
  for (int i = 0; i < number_of_skeletons; i++) {
    skeletons[i].update(player_x, player_y);
  }

  // sword
  if (player_sword_time > 0) {
    if (player_direction == 1 || player_direction == 3) { // left right
      ardbitmap.drawBitmap(player_x+sword_positions[player_direction][0], player_y+sword_positions[player_direction][1], sword_frames[1], SWORD_HORI_WIDTH, SWORD_HORI_HEIGHT, WHITE, ALIGN_CENTER, sword_positions[player_direction][2]);
    } else if (player_direction == 0) { // up down
      ardbitmap.drawBitmap(player_x+sword_positions[player_direction][0], player_y+sword_positions[player_direction][1], sword_frames[2], SWORD_VERTI_WIDTH, SWORD_VERTI_HEIGHT, WHITE, ALIGN_CENTER, MIRROR_NONE);
    } else {
      ardbitmap.drawBitmap(player_x+sword_positions[player_direction][0], player_y+sword_positions[player_direction][1], sword_frames[0], SWORD_VERTI_WIDTH, SWORD_VERTI_HEIGHT, WHITE, ALIGN_CENTER, MIRROR_NONE);
    }
  }
  // heart background
  arduboy.fillRect(0,0,27,9,BLACK);

  // full hearts
  for (int i = 0; i < player_health; i++) {
    arduboy.drawBitmap(i * 9, 0, heart_frames[0], HEART_SPRITE_SIZE, HEART_SPRITE_SIZE);
  }

  // empty hearts
  for (int i = 2; i > player_health-1; i--) {
    arduboy.drawBitmap(i * 9, 0, heart_frames[1], HEART_SPRITE_SIZE, HEART_SPRITE_SIZE);
  }

  arduboy.display(); // update screen to display changes
}

}

void processTransition() {
if (arduboy.nextFrame()) {
  // clear screen
  arduboy.clear();

  // draw the room twice for scrolling animation
  drawRoom(transition_offset);
  drawRoomWall(transition_offset);

  drawRoom(transition_offset-128);
  drawRoomWall(transition_offset-128);

  // scroll until the screen is in the right place, then go to game mode
  if (transition_offset > 0) {
    transition_offset -= 1;
  } else {
    state = 1;

    nextLevel();
  }

  arduboy.display();
}

}

void initialStartGame() {
  player_health = 3;
  game_level = 1;

  transition_offset = 128;
}

void processMenu() {
  // start game on button press
  if (inputButton()) {
    initialStartGame();
    state = 2;
  }

  // clear screen
  arduboy.clear();
  
  // draw title
  arduboy.drawBitmap(0,0,title_logo,128,64);

  // update screen
  arduboy.display();
}

// mainloop
void loop() {
  if (state == 0) {
    processMenu();
  } else if (state == 1) {
    processGame();
  } else if (state == 2) {
    processTransition();
  }
}