/*
  Created by Fabrizio Di Vittorio (fdivitto2013@gmail.com) - <http://www.fabgl.com>
  Copyright (c) 2019-2022 Fabrizio Di Vittorio.
  All rights reserved.


* Please contact fdivitto2013@gmail.com if you need a commercial license.


* This library and related software is available under GPL v3.

  FabGL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FabGL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FabGL.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "fabgl.h"

#include <math.h>


fabgl::VGAController DisplayController;



// Mask, 1 bit per pixel. One byte contains 8 pixels. 0 = transparent, 1 = foreground color (in this case it is "RGB888(255, 0, 0)".
const uint8_t bitmap1_data[] = {
  0x07, 0xe0, 0x1f, 0xf8, 0x3f, 0xfc, 0x6d, 0xb6, 0xff, 0xff, 0x39, 0x9c, 0x10, 0x08,
};
Bitmap bitmap1 = Bitmap(16, 7, bitmap1_data, PixelFormat::Mask, RGB888(255, 255, 0));


// ABGR, 2 bits per channel. One byte contains one pixel.
const uint8_t bitmap2_data[] = {
  0x00, 0x00, 0xd5, 0xd5, 0xd9, 0xd9, 0xd9, 0xd9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe9, 0xea, 0xc0, 0x00, 0x00, 0xd4, 0xd4, 0xd4, 0x00, 0x00, 0x00,
  0xd5, 0xd5, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xc0, 0xd4, 0xd5, 0xd9, 0xd9, 0xd9, 0xd5, 0x00,
  0xd9, 0xd9, 0xd9, 0xd9, 0xd9, 0xd9, 0xc0, 0xc0, 0xd4, 0xd5, 0xd9, 0xe9, 0xe9, 0xe9, 0xd9, 0xd5,
  0x00, 0xd4, 0xe9, 0xe9, 0xea, 0xea, 0xc0, 0xd4, 0xd5, 0xe9, 0xe9, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd4, 0xd5, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xe9, 0xe9, 0xc0, 0xe9, 0xc0, 0xe9, 0xe9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xea, 0xea, 0xea, 0xc0, 0xea, 0xc0, 0xea, 0xea, 0xd9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xd9, 0xd9, 0xd9, 0xc0, 0xd9, 0xc0, 0xd9, 0xd9, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xd5, 0xd5, 0xc0, 0xd5, 0xc0, 0xd5, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd4, 0xd5, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xd4, 0xd9, 0xd9, 0xe9, 0xe9, 0xc0, 0xd4, 0xd5, 0xe9, 0xe9, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xc0, 0xc0, 0xd4, 0xd5, 0xd9, 0xe9, 0xe9, 0xe9, 0xd9, 0xd5,
  0xd4, 0xd4, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xc0, 0xd4, 0xd5, 0xd9, 0xd9, 0xd5, 0xd5, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xd9, 0xe9, 0xc0, 0x00, 0x00, 0xd4, 0xd5, 0xd5, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xd4, 0xd4, 0xd5, 0xd5, 0xd5, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
Bitmap bitmap2 = Bitmap(16, 16, &bitmap2_data[0], PixelFormat::RGBA2222);


// RGBA, 8 bits per channel. Four bytes contains one pixel.
const uint8_t bitmap3_data[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0x00, 0x00, 0x00, 0xff, 0x80, 0x5e, 0x4d, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x5e, 0x4d, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x5e, 0x4d, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0x00, 0x00, 0x00, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x5e, 0x4d, 0xff, 0x3f, 0x2c, 0x16, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x3f, 0x2c, 0x16, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0x9c, 0x9a, 0xba, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x5e, 0x4d, 0xff, 0x3f, 0x2c, 0x16, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0xb7, 0xb5, 0xcc, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x5e, 0x4d, 0xff, 0x3f, 0x2c, 0x16, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x80, 0x5e, 0x4d, 0xff, 0x3f, 0x2c, 0x16, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xdc, 0xdb, 0xe6, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x80, 0x5e, 0x4d, 0xff, 0x80, 0x5e, 0x4d, 0xff, 0x3f, 0x2c, 0x16, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x3f, 0x2c, 0x16, 0xff, 0x3f, 0x2c, 0x16, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
Bitmap bitmap3 = Bitmap(16, 15, &bitmap3_data[0], PixelFormat::RGBA8888);


Sprite sprites[3];


void setup()
{
  //Serial.begin(115200); delay(500); Serial.write("\n\n\n"); // DEBUG ONLY

  DisplayController.begin(GPIO_NUM_22, GPIO_NUM_21, GPIO_NUM_19, GPIO_NUM_18, GPIO_NUM_5, GPIO_NUM_4, GPIO_NUM_23, GPIO_NUM_15);
  DisplayController.begin();

  DisplayController.setResolution(VGA_320x200_75Hz);

  // draw background
  Canvas cv(&DisplayController);
  cv.setBrushColor(RGB888(0, 0, 64));
  cv.clear();
  cv.setPenColor(RGB888(64, 64, 0));
  for (int i = 0; i < cv.getWidth(); i += 10)
    cv.drawLine(i, 0, i, cv.getHeight() - 1);
  for (int i = 0; i < cv.getHeight(); i += 10)
    cv.drawLine(0, i, cv.getWidth() - 1, i);

  // adjust this to center screen in your monitor
  //DisplayController.moveScreen(-6, 0);

  // associates bitmaps to sprites
  sprites[0].addBitmap(&bitmap1);
  sprites[1].addBitmap(&bitmap2);
  sprites[2].addBitmap(&bitmap3);

  // sets initial position
  sprites[0].moveTo(0, 50);
  sprites[1].moveTo(0, 100);
  sprites[2].moveTo(0, 150);

  // makes sprites visible
  sprites[0].visible = true;
  sprites[1].visible = true;
  sprites[2].visible = true;

  // add sprites to display controller
  DisplayController.setSprites(sprites, 3);
}



void loop()
{
  static double count = 0;

  // set position of sprite 0
  sprites[0].x += 1;
  if (sprites[0].x >= DisplayController.getViewPortWidth())
    sprites[0].x = 0;
  sprites[0].y = 50 + 35.0 * sin(count);

  // set position of sprite 1
  sprites[1].x += 1;
  if (sprites[1].x >= DisplayController.getViewPortWidth())
    sprites[1].x = 0;
  sprites[1].y = 100 + 35.0 * cos(count);

  // set position of sprite 2
  sprites[2].x += 1;
  if (sprites[2].x >= DisplayController.getViewPortWidth())
    sprites[2].x = 0;
  sprites[2].y = 150 + 35.0 * sin(count);

  // update sprites positions
  DisplayController.refreshSprites();

  count += 0.10;

  delay(std::max(10, 50 - (int)count / 2));
}






