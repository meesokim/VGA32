/*
  Created by Miso Kim (meeso.kim@gmail.com) - <http://spc1000.github.io>
  Copyright (c) 2019-2022 Miso Kim.
  All rights reserved.

* Please contact meeso.kim@gmail.com if you need a commercial license.

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


/*
 * Optional SD Card connections:
 *   MISO => GPIO 16  (2 for PICO-D4)
 *   MOSI => GPIO 17  (12 for PICO-D4)
 *   CLK  => GPIO 14
 *   CS   => GPIO 13
 *
 * To change above assignment fill other paramaters of FileBrowser::mountSDCard().
 */

#pragma message "This sketch requires Tools->Partition Scheme = Huge APP"

#include "fabgl.h"
#include "fabutils.h"

#include "src/machine.h"