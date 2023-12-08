#define TILEMAP_WIDTH 8
#define TILEMAP_HEIGHT 4

const int PROGMEM map_01[4][8] =   {
  {0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,0}
};

const int PROGMEM maps[2][4][8] = { // # of maps, height, width
  {
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0}
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0}
  }
};