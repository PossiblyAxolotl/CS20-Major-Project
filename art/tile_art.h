// 11x12
#define TRAPDOOR_SPRITE_WIDTH 11
#define TRAPDOOR_SPRITE_HEIGHT 12

const uint8_t PROGMEM trapdoor_closed[] = {
0xf0, 0xf0, 0xd0, 0xd0, 0x50, 0xd0, 0xd0, 0xd0, 0xd0, 0x90, 0xf0, 
0x0f, 0x0a, 0x0e, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0a, 0x0a, 0x0f
};

const uint8_t PROGMEM trapdoor_open[] = {
0xff, 0x15, 0xd7, 0xd5, 0x11, 0x95, 0x95, 0x15, 0x1d, 0x15, 0xff, 
0x0f, 0x08, 0x0b, 0x0b, 0x08, 0x0b, 0x0b, 0x08, 0x0b, 0x08, 0x0f
};

const unsigned char* trapdoor_frames[2] {
    trapdoor_open,
    trapdoor_closed
};