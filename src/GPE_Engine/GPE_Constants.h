/*
GPE_Constants.h
This file is part of:
GAME PENCIL ENGINE
https://create.pawbyte.com
Copyright (c) 2014-2019 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2019 PawByte LLC.
Copyright (c) 2014-2019 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://create.pawbyte.com>


*/
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "GPE_CIncludes.h"

//Version INFO VARIABLES

//Game Pencil Runtime
const int GPR_MAJOR_VERSION = 1;
const int GPR_MINOR_VERSION =3;
const int GPR_PATCHLEVEL = 1;

//Game Pencil Engine Editor
const double GPE_VERSION_NUMBER = 1;
const double GPE_VERSION_SUB_NUMBER = 3;
const double GPE_VERSION_UPDATE_NUMBER = 1;
const double GPE_VERSION_DOUBLE_NUMBER = GPE_VERSION_NUMBER + GPE_VERSION_SUB_NUMBER/10 + GPE_VERSION_UPDATE_NUMBER/100;

const bool GPE_DRAW_OBJECT_BOXES = false;
const double PI = 3.141592653589793;
const double pi = 3.141592653589793;
const double pie = 3.141592653589793;
const double GPE_TO_RADIANS = 180.d/PI;
const double degreesMultiple = 180.d/-PI;
const double radiansMultiple = PI/-180.d;

const int IS_NULL = -1;
const int isNULL = -1;

const int FA_LEFT = -2;
const int FA_CENTER = -1;
const int FA_RIGHT = 0;

const int FA_ERROR = -1;

const int FA_TOP = -2;
const int FA_MIDDLE = -1;
const int FA_BOTTOM = 0;

//Flip flags
const int FLIP_VERTICAL = 1;
const int FLIP_HORIZONTAL = 2;

const int Native_None = 0;
const int Native_GamePencil = 1;
const int Native_GamePencil_CPP = 2;
const int Native_Electron = 3;
const int Native_NWF = 4;

const int GPE_BUILD_HTML5 = 0;
const int GPE_BUILD_DESKTOP = 1;
const int GPE_BUILD_WINDOWS = 1;
const int GPE_BUILD_MAC = 2;
const int GPE_BUILD_LINUX = 3;
const int GPE_BUILD_WIIU = 4;
const int GPE_BUILD_SWITCH = 5;
const int GPE_BUILD_XBOXONE = 6;
const int GPE_BUILD_PS4 = 7;
const int GPE_BUILD_PSVITA = 8;
const int GPE_BUILD_ANDROID = 9;
const int GPE_BUILD_IOS = 10;
const int GPE_BUILD_WINDOWS_PHONE = 11;

const int GPE_BUILD_OPTIONS = 20;

const int GPE_IDE_WINDOWS = 1;
const int GPE_IDE_MAC = 2;
const int GPE_IDE_LINUX = 3;

const int GENERAL_GPE_PADDING = 8;
const int GPE_AVERAGE_LINE_HEIGHT = 20;
const int GPE_AVERAGE_LINE_HEIGHT_WITH_PADDING = 32;
const int GPE_TITLE_BPADDING = 16;
const int TOOLKEY_SHORTCUT_WIDTH = 64;
const int GENERAL_ICON_WIDTH = 24;
const int GENERAL_THUMBNAIL_SIZE = 48;
const int GENERAL_ICON_WIDTH_AND_PADDING = 32;
const int GENERAL_PLUSMINUX_ICON_SIZE = 8;
const int TOOLKEY_OPTION_PADDING = 16;
const int TOOLKEY_ICON_X_PADDING = 4;
const int GENERAL_NEAR_ICON_YPADDING = 8;
const int RESOURCE_AREA_HEIGHT = 14;
const int GPE_MAX_RECENT_PROJECT_LIST_SIZE = 15;

const int sprFrameCount = 0;
const int sprFrameWidth = 1;
const int sprFrameHeight = 2;
const int sprHPixelOffset = 3;
const int sprVPixelOffset = 4;
const int sprHPixelPadding = 5;
const int sprVPixelPadding = 6;

const int blend_mode_add = 1;
const int blend_mode_blend = 2;
const int blend_mode_mod = 3;
const int blend_mode_none = 4;

const int RESOURCE_TYPE_PROJECT_SETTINGS = 1;
const int RESOURCE_TYPE_ANIMATION = 2;
const int RESOURCE_TYPE_SPRITE = 2;
const int RESOURCE_TYPE_TEXTURE = 3;
const int RESOURCE_TYPE_TILESHEET = 4;
const int RESOURCE_TYPE_AUDIO = 5;
const int RESOURCE_TYPE_VIDEO = 6;
const int RESOURCE_TYPE_FUNCTION = 7;
const int RESOURCE_TYPE_OBJECT = 8;
const int RESOURCE_TYPE_SCENE = 9;
const int RESOURCE_TYPE_CLASS = 10;
const int RESOURCE_TYPE_PATH = 11;
const int RESOURCE_TYPE_FONT = 12;

const int RESOURCE_TYPE_EMITTER = 13;
const int RESOURCE_TYPE_LIGHT = 14;

//15 thru 20 not looked yet
const int RESOURCE_TYPE_PLUGIN = 15;
const int RESOURCE_TYPE_DICTIONARY = 16;
const int RESOURCE_TYPE_SPREADSHEET = 17;
const int RESOURCE_TYPE_EVENT = 18;
const int RESOURCE_TYPE_QUEST = 19;
const int RESOURCE_TYPE_ACHIEVEMENT = 20;
const int RESOURCE_TYPE_SHADER = 21;

//room constants
//const int ROOM_PAWBYTE_LOGO = 1;
//const double PI =3.14159265;
//The Basic colors we use

#endif
