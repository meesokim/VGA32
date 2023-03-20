class MC6847
{
    static const int height = 240, width = 320;
    uint8_t VRAM[0x2000] = {0};
    uint8_t framebuf[width*height];
    enum {
        ATTR_INV = 0x1, // white
        ATTR_CSS = 0x2, // cyan blue
        ATTR_SEM = 0x4,
        ATTR_EXT = 0x8};
    int rept, repb, repl, repr;    
    uint8_t cMap[18] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
  
    uint8_t semiGrFont0[16*12] = {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xf0,
        0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
        0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x0f,0x0f,0x0f,
        0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xf0,0xf0,
        0xf0,0xf0,0xf0,0xf0,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xff,0xff,0xff,0xff,0xff,0xff,
        0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xf0,0xf0,0xf0,
        0xf0,0xf0,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
        0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
        0xff,0xff,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xff,0xff,0xff,0xff,0xff,0xff,0xf0,0xf0,
        0xf0,0xf0,0xf0,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,};
    uint8_t semiGrFont1[64*12]; 
    uint8_t CGROM[12*(256-32)] =  {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //32
        0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x14, 0x14, 0x3E, 0x14, 0x3E, 0x14, 0x14, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x1E, 0x28, 0x1C, 0x0A, 0x3C, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x32, 0x32, 0x04, 0x08, 0x10, 0x26, 0x26, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x10, 0x28, 0x10, 0x28, 0x26, 0x24, 0x1A, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x10, 0x10, 0x08, 0x04, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x10, 0x08, 0x04, 0x04, 0x04, 0x08, 0x10, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x3E, 0x1C, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x0C, 0x12, 0x12, 0x12, 0x12, 0x12, 0x0C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x18, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x02, 0x1C, 0x20, 0x20, 0x3E, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x02, 0x0C, 0x02, 0x22, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x04, 0x0C, 0x14, 0x24, 0x3E, 0x04, 0x04, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x20, 0x3C, 0x02, 0x02, 0x22, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x3C, 0x22, 0x22, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x02, 0x02, 0x04, 0x08, 0x08, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x1E, 0x02, 0x22, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x02, 0x04, 0x08, 0x00, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x06, 0x0A, 0x0A, 0x06, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x3E, 0x22, 0x22, 0x22, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x3C, 0x22, 0x22, 0x3C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x20, 0x20, 0x22, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3C, 0x12, 0x12, 0x12, 0x12, 0x12, 0x3C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x20, 0x20, 0x3C, 0x20, 0x20, 0x3E, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x20, 0x20, 0x3C, 0x20, 0x20, 0x20, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x26, 0x22, 0x22, 0x1E, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x22, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x0E, 0x04, 0x04, 0x04, 0x04, 0x24, 0x18, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x24, 0x28, 0x30, 0x28, 0x24, 0x22, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3E, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x36, 0x2A, 0x2A, 0x22, 0x22, 0x22, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x32, 0x2A, 0x26, 0x22, 0x22, 0x22, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x22, 0x22, 0x22, 0x22, 0x22, 0x3E, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x3C, 0x20, 0x20, 0x20, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x22, 0x2A, 0x26, 0x1E, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x3C, 0x28, 0x24, 0x22, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x1C, 0x02, 0x22, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x2A, 0x2A, 0x36, 0x22, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x22, 0x14, 0x08, 0x14, 0x22, 0x22, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x02, 0x04, 0x08, 0x10, 0x20, 0x3E, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x1C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x10, 0x20, 0x7F, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //64
        0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3C, 0x7E, 0x7E, 0x7E, 0x7E, 0x3C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x2A, 0x1C, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x04, 0x7E, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x00, 0x72, 0x8A, 0x8A, 0x72, 0x02, 0x3E, 0x02, 0x3E, 0x20, 0x3E, 0x00,
        0x00, 0x72, 0x8A, 0x72, 0xFA, 0x2E, 0x42, 0x3E, 0x3E, 0x20, 0x3E, 0x00,
        0x00, 0x22, 0x22, 0xFA, 0x02, 0x72, 0x8B, 0x8A, 0x72, 0x22, 0xFA, 0x00,
        0x00, 0x10, 0x10, 0x28, 0x44, 0x82, 0x00, 0xFE, 0x10, 0x10, 0x10, 0x10,
        0x00, 0x7C, 0x44, 0x7C, 0x10, 0xFE, 0x00, 0x7C, 0x04, 0x04, 0x04, 0x00,
        0x00, 0x7C, 0x04, 0x04, 0x00, 0xFE, 0x00, 0x7C, 0x44, 0x44, 0x7C, 0x00,
        0x00, 0x7C, 0x40, 0x78, 0x40, 0x40, 0x7C, 0x10, 0x10, 0x10, 0xFE, 0x00,
        0x00, 0x82, 0x8E, 0x82, 0x8E, 0x82, 0xFA, 0x02, 0x40, 0x40, 0x7E, 0x00,
        0x00, 0x02, 0x22, 0x22, 0x22, 0x52, 0x52, 0x8A, 0x8A, 0x02, 0x02, 0x00,
        0x00, 0x44, 0x7C, 0x44, 0x7C, 0x00, 0xFE, 0x10, 0x50, 0x40, 0x7C, 0x00,
        0x00, 0x10, 0x10, 0xFE, 0x28, 0x44, 0x82, 0x10, 0x10, 0x10, 0xFE, 0x00,
        0x00, 0x01, 0x05, 0xF5, 0x15, 0x15, 0x17, 0x25, 0x45, 0x85, 0x05, 0x00,
        0x00, 0x01, 0x05, 0xF5, 0x85, 0x85, 0x87, 0x85, 0xF5, 0x05, 0x05, 0x00,
        0x00, 0x02, 0x72, 0x8A, 0x8A, 0x8A, 0x72, 0x02, 0x42, 0x40, 0x7E, 0x00,
        0x00, 0x00, 0x7C, 0x40, 0x40, 0x40, 0x7C, 0x10, 0x10, 0x10, 0xFE, 0x00,
        0x00, 0x02, 0x72, 0x8A, 0x72, 0xFA, 0x2E, 0x42, 0x22, 0x20, 0x3E, 0x00,

        0x00, 0x00, 0x00, 0x3E, 0x22, 0x3E, 0x22, 0x3E, 0x00, 0x00, 0x00, 0x00, // 128
        0x00, 0x00, 0x3E, 0x22, 0x3E, 0x22, 0x3E, 0x22, 0x42, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x54, 0x54, 0x10, 0x28, 0x44, 0x82, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x12, 0xFC, 0x38, 0x34, 0x52, 0x91, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x10, 0xFE, 0x10, 0x38, 0x54, 0x92, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x28, 0x7C, 0x92, 0x7C, 0x54, 0xFE, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x10, 0x10, 0x7C, 0x10, 0x10, 0xFE, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x20, 0x7E, 0x80, 0x7C, 0x50, 0xFE, 0x10, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x08, 0xFC, 0xA8, 0xFE, 0xA4, 0xFE, 0x14, 0x04, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x28, 0x44, 0xFE, 0x14, 0x24, 0x48, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x74, 0x24, 0xF5, 0x65, 0xB2, 0xA4, 0x28, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x10, 0x10, 0x54, 0x92, 0x30, 0x10, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x10, 0xFE, 0x10, 0x28, 0x44, 0x82, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x28, 0x44, 0x82, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x28, 0x7C, 0x82, 0x7C, 0x44, 0x7C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x3C, 0x44, 0xA8, 0x10, 0x3E, 0xE2, 0x3E, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x7F, 0x08, 0x7F, 0x08, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x06, 0x18, 0x20, 0x18, 0x06, 0x00, 0x3E, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x30, 0x0C, 0x02, 0x0C, 0x30, 0x00, 0x3E, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x3E, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x22, 0x14, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x49, 0x7F, 0x49, 0x3E, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x7F, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7F, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
        0x0C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x60,
        0x00, 0x0F, 0x08, 0x08, 0x08, 0x48, 0xA8, 0x18, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x7E, 0x20, 0x10, 0x20, 0x7E, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3E, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00,

        0x00, 0x60, 0x90, 0x90, 0x90, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 160
        0x00, 0x20, 0x60, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x60, 0x90, 0x20, 0x40, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x60, 0x90, 0x20, 0x90, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x60, 0xA0, 0xA0, 0xF0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0xF0, 0x80, 0xF0, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x60, 0x80, 0xF0, 0x90, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0xF0, 0x10, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x60, 0x90, 0x60, 0x90, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x60, 0x90, 0xF0, 0x10, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x02, 0x34, 0x48, 0x48, 0x36, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x18, 0x24, 0x38, 0x24, 0x24, 0x38, 0x20, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x4E, 0x30, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x18, 0x24, 0x24, 0x3C, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x1C, 0x20, 0x20, 0x18, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x1C, 0x2A, 0x2A, 0x1C, 0x08, 0x00, 0x00, 0x00,
        0x80, 0x40, 0x40, 0x20, 0x10, 0x10, 0x08, 0x04, 0x04, 0x02, 0x01, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x7C, 0x00, 0xFE, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xFE, 0xAA, 0xAA, 0xAA, 0xFE, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x7C, 0x10, 0x7C, 0x14, 0x14, 0xFE, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x10, 0xFE, 0x00, 0x28, 0x44, 0x82, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x20, 0x20, 0x20, 0xFE, 0x20, 0x20, 0x3E, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x44, 0x82, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x20, 0x20, 0xFC, 0x24, 0x24, 0x44, 0x86, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x36, 0x49, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x18, 0x20, 0x18, 0x24, 0x18, 0x04, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x08, 0x22, 0x14, 0x49, 0x14, 0x22, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x38, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
        0x00, 0x60, 0x90, 0x6E, 0x11, 0x10, 0x10, 0x11, 0x0E, 0x00, 0x00, 0x00,
        0x01, 0x02, 0x02, 0x04, 0x08, 0x08, 0x10, 0x20, 0x20, 0x40, 0x80, 0x80,

        0x00, 0x00, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 192
        0x00, 0x00, 0x00, 0x00, 0x3C, 0x02, 0x1E, 0x22, 0x1F, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x20, 0x20, 0x2C, 0x32, 0x22, 0x32, 0x2C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x22, 0x1C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x02, 0x02, 0x1A, 0x26, 0x22, 0x26, 0x1A, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x3E, 0x20, 0x1E, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x0C, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1A, 0x26, 0x22, 0x26, 0x1A, 0x02, 0x1C, 0x00,
        0x00, 0x00, 0x20, 0x20, 0x2C, 0x32, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x08, 0x00, 0x18, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x04, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x24, 0x18, 0x00, 0x00,
        0x00, 0x00, 0x20, 0x20, 0x22, 0x24, 0x28, 0x34, 0x22, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x76, 0x49, 0x49, 0x49, 0x49, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x2C, 0x32, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x22, 0x1C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x2C, 0x32, 0x22, 0x32, 0x2C, 0x20, 0x20, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1A, 0x26, 0x22, 0x26, 0x1A, 0x02, 0x02, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x2E, 0x30, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1E, 0x20, 0x1C, 0x02, 0x3C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x10, 0x38, 0x10, 0x10, 0x12, 0x0C, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x26, 0x1A, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x49, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x22, 0x14, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x26, 0x1A, 0x02, 0x1C, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x3E, 0x04, 0x08, 0x10, 0x3E, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x08, 0x10, 0x10, 0x20, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x54, 0xFE, 0x54, 0xFE, 0x54, 0x28, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x08, 0x08, 0x04, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x3E, 0x54, 0x14, 0x14, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 224
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };        
public:
    uint8_t *fb;
    char border;
    enum colorNum {
        COLOR_BLACK, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE,
        COLOR_RED, COLOR_BUFF, COLOR_CYAN, COLOR_MAGENTA,
        COLOR_ORANGE, COLOR_CYANBLUE, COLOR_LGREEN, COLOR_DGREEN,COLOR_WHITE };           
    uint8_t GMODE;
    MC6847() {
        fb = framebuf;
    };
    void write(uint16_t address, uint8_t uint8_t)
    {
        VRAM[address&0x1fff] = uint8_t;
    }
    uint8_t read(uint16_t address)
    {
        return VRAM[address&0x1fff];
    }
    const void * update(void *fb, int pitch)
    {
        int pos = 0;
        uint8_t *data = this->fb = (uint8_t *)fb;
        uint8_t _gm0, _gm1, _ag, _css;
        uint16_t _page, y, h, x, mask;
        uint8_t attr, ch, b, cix, c;
        _gm0 = BIT(GMODE, 2);
        _gm1 = BIT(GMODE, 1);
        _ag = BIT(GMODE, 3);
        _css = BIT(GMODE, 7);
        _page = GMODE >> 4 & 0x3;
        PIXEL bg, fg;
        rept = (height - 192)/2 * pitch;
        repl = (width - 256)/2;
        repr = (width - 256)/2;
        repb = (height - 192)/2 * pitch;
        border = cMap[0];
        b = 0;
        if (_ag == 0)
        {
            FILL(data, rept, border);
            for(y=0; y < 16; y++)
            {
                for(h=0; h < 12; h++)
                {
                    FILL(data, repl, border);
                    for(x=0; x < 32; x++)
                    {
                        attr = VRAM[x + y * 32 + SCREEN_ATTR_START + _page * 0x200];
                        ch = VRAM[x + y * 32 + SCREEN_TEXT_START + _page * 0x200];
                        if ((attr & ATTR_SEM) != 0)
                        {
                            bg = cMap[0];
                            if ((attr & ATTR_EXT) != 0)
                            {
                                fg = cMap[(((attr & ATTR_CSS) << 1) | ((ch & 0xc0) >> 6)) + 1];
                                b = semiGrFont1[(ch & 0x3f) * 12 + h];	
                            } 
                            else 
                            {
                                fg = cMap[((ch & 0x70)>> 4) + 1];
                                //printf("fg=%d,%d\n", ch, ((ch & 0x70)>> 4) + 1);
                                b = semiGrFont0[(ch & 0x0f) * 12 + h];
                            }
                        }
                        else // ASCII
                        {
                            cix = (attr & ATTR_CSS) >> 1; 
                            if ((attr & ATTR_INV) == 0)
                            {
                                bg = cMap[11 + cix * 2];
                                fg = cMap[11 + cix * 2 + 1];
                            }
                            else
                            {
                                fg = cMap[11 + cix * 2];
                                bg = cMap[11 + cix * 2 + 1];
                            }
                            if (ch < 32 && ((attr & ATTR_EXT) == 0))
                                ch = 32;
                            if (((attr & ATTR_EXT) != 0) && (ch < 96))
                                ch += 128;
                            if (ch >= 96 && ch < 128)
                                b = VRAM[0x1600+(ch-96)*16+h];
                            else if (ch >= 128 && ch < 224)
                                b = VRAM[0x1000+(ch-128)*16+h];
                                else if (ch >= 32)
                                b = CGROM[(ch-32)*12+h];
                        }
                        for(mask = 0x80; mask != 0; mask >>=1)
                        {
                            *data++ = (((b & mask) != 0) ? fg : bg);
                        }
                    }
                    FILL(data, repr, border);
                }
            }
        FILL(data, repb, border);
        }
        else
        {
            bg = cMap[0];
            border = fg = (_css ? cMap[10] : cMap[9]);
            //border = (_css ? cMap[10] : cMap[9]);
            FILL(data, rept, border);
            for(y = 0; y < 192; y++)
            {
                FILL(data, repl, border);
                for(x = 0; x < 32; x++)
                {
                    b = VRAM[y * 32 + x];
                    if (_gm1)
                    {
                        if (_gm0)
                        { 
                            for(mask = 0x80; mask != 0; mask >>=1)
                            {	
                                *data++ = (b & mask) != 0 ? fg : bg;
                            }
                        }
                        else // _gm0 == 0
                        {
                            for(c = 6; c < 8 && c >= 0; c-=2)
                            {
                                *data++ = cMap[((b & (0x3 << c)) >> c) + (_css ? 5 : 1)];
                                *data++ = cMap[((b & (0x3 << c)) >> c) + (_css ? 5 : 1)];
                            }
                        }
                    }
                    else
                    {
                        if (_gm0)
                        {
                            for(mask = 0x80; mask != 0; mask >>=1)
                            {	
                                *data++ = (b & mask) != 0 ? fg : bg;
                                *data++ = (b & mask) != 0 ? fg : bg;
                            }
                        }
                        else
                        {
                            for(c = 6; c > 0; c-=2)
                            {
                            *(data + 256 + 1) = *(data + 256) = *(data+1) = *data = cMap[((b & (0x3 << c)) >> c) + (_css ? 5 : 1)];
                            data+=2;
                            }
                        }
                    } 
                }
                FILL(data, repr, border);
        //		if (_gm1 + _gm0 == 0)
        //			data += 256 + repr + repl;
            }
            FILL(data, repb, border);
        }
        // memcpy(framebuf, fb, width * height * 1);
        return (const void *)data;
    }
    void* getFrameBuffer() {
        return (void*) framebuf;
    }
    void SetPixel(UG_S16 x, UG_S16 y, UG_COLOR c) {
        fb[x+y*width]=c;
    }
    int GetPixel(int x, int y) {
        return fb[x + y * 320];
    }
};