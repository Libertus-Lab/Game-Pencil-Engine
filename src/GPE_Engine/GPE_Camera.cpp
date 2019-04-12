/*
GPE_Camera.h
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

#include "GPE_Camera.h"
#include "GPE_Constants.h"

GPE_SceneCamera::GPE_SceneCamera( double cameraX, double cameraY, double cameraW, double cameraH, int cameraIdNumber )
{
    cameraRect = new GPE_Rect();
    cameraBoundaries = new GPE_Rect();
    cameraRect->update_shape(cameraX, cameraY, cameraW, cameraH);

    renderRect = new GPE_Rect();
    scaledRect = new GPE_Rect();
    scaledRect->update_shape(cameraX, cameraY, cameraW, cameraH);
    viewTarget = IS_NULL;
    horiBorder = IS_NULL;
    vertBorder = IS_NULL;

    hSpeed = 0;
    vSpeed = 0;

    isVisible = true;

    maxXTilesInView = 0;
    maxYTilesInView = 0;

    cSceneWidth = 0;
    cSceneHeight = 0;
}

GPE_SceneCamera::~GPE_SceneCamera()
{

}

void GPE_SceneCamera::center_camera ( double centerX, double centerY)
{
    cameraRect->update_position(centerX-cameraRect->get_width()/2,centerY-cameraRect->get_height()/2);
    if( cameraRect->get_x2() > cameraBoundaries->get_width() )
    {
        cameraRect->set_x(cameraBoundaries->get_width()-cameraRect->get_width() );
    }
    if( cameraRect->get_y2() > cameraBoundaries->get_height() )
    {
        cameraRect->set_y( cameraBoundaries->get_height()-cameraRect->get_height() );
    }

    if( cameraRect->get_x() < 0 )
    {
        cameraRect->set_x(0);
        //console.log("Setting cam X to 0 for protection...");
    }
    if( cameraRect->get_y() < 0)
    {
        cameraRect->set_y(0);
        //console.log("Setting cam Y to 0 for protection...");

    }
    //console.log( "New Camera Pos:	("+cameraRect.get_x()+" , "+cameraRect.get_y()+")" );

}

void GPE_SceneCamera::move_hori ( double movSpace )
{
    if (movSpace < 0 )
    {
        if( cameraBoundaries->get_x() < cameraRect->get_x() +movSpace )
        {
            cameraRect->add_x( movSpace);
        }
        else
        {
            cameraRect->set_x(0);
        }
    }
    else
    {
        if( cameraBoundaries->get_x2() > cameraRect->get_x2() +movSpace )
        {
            cameraRect->add_x( movSpace);
        }
    }
}

void GPE_SceneCamera::move_vert ( double movSpace )
{
    if (movSpace < 0 )
    {
        if( cameraBoundaries->get_y() < cameraRect->get_y() +movSpace )
        {
            cameraRect->add_y( movSpace);
        }
        else
        {
            cameraRect->set_y(0);
        }
    }
    else
    {
        if( cameraBoundaries->get_y2() > cameraRect->get_y2() +movSpace )
        {
            cameraRect->add_y( movSpace);
        }
    }
}

void GPE_SceneCamera::set_visible( bool visiblility )
{
    isVisible = visiblility;
}

void GPE_SceneCamera::setup_camera (double cameraX,double cameraY, double cameraW, double cameraH)
{
    isVisible = true;
    //console.log("Updaing Camera with ("+cameraX+", "+cameraY+", "+ cameraW+", "+cameraH+") dimensions...");
    renderRect->update_shape(cameraX, cameraY, cameraW, cameraH);
    cameraRect->update_shape(cameraX, cameraY, cameraW, cameraH);
}

void GPE_SceneCamera::update_tile_size (int newTX, int newTY)
{
    maxXTilesInView = newTX;
    maxYTilesInView = newTY;
}

void GPE_SceneCamera::update_screen_space ( double cameraX, double cameraY, double cameraW, double cameraH)
{
    renderRect->update_shape(cameraX, cameraY, cameraW, cameraH);
}

void GPE_SceneCamera::update_view_space ( double cameraX, double cameraY, double cameraW, double cameraH)
{
    cameraRect->update_shape(cameraX, cameraY, cameraW, cameraH);
}
