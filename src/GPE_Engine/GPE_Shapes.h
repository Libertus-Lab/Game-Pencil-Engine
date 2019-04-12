/*
GPE_Shapes.h
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

#ifndef GPE_SHAPES_H
#define GPE_SHAPES_H
#include <stdio.h>
#include <stdlib.h>

class GPE_Rect
{
    public:
        double x;
        double y;
        double xCenter;
        double x2Pos;
        double yMiddle;
        double y2Pos;
        double w;
        double h;
        GPE_Rect( double rX= 0, double rY= 0, double rW= 0, double rH = 0);
        ~GPE_Rect();
        void add_x( double xPlus);
        void add_y( double yPlus);
        void copy_rect( GPE_Rect * otherRect );
        double get_center();
        double get_height();
        double get_middle();
        double get_width();
        double get_x();
        double get_x2();
        double get_y();
        double get_y2();
        void set_x( double newX );
        void set_y( double newY );
        void set_w( double newW );
        void set_h( double newH );
        void update_position( double xNew, double yNew);
        void update_box( double xNew, double yNew, double widNew, double hgtNew);
        void update_shape( double xNew, double yNew, double widNew, double hgtNew);
        void update_size( double widNew, double hgtNew);
};

#endif
