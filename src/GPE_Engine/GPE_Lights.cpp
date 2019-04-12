/*
GPE_Lights.cpp
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

#include "GPE_Lights.h"

GPE_BasicLight::GPE_BasicLight()
{
    lightX = 0;
    lightY = 0;
    lightIntensity = 255;
    lightIsActive = true;
    lightActiveOnStart = true;
    lightColor = new GPE_Color("Light",255,255 );
}

GPE_BasicLight::~GPE_BasicLight()
{
    if( lightColor!=NULL )
    {
        delete lightColor;
        lightColor = NULL;
    }
}

void GPE_BasicLight::render_light( double scale, GPE_Rect * cam )
{
    cam = GPE_find_camera( cam );
    gcanvas->render_rectangle(0,0, cam->w, cam->h, lightColor, false, lightIntensity );
}

void GPE_BasicLight::render_light_at( double x, double y, double scale, GPE_Rect * cam )
{
    cam = GPE_find_camera( cam );
    gcanvas->render_rect( cam, lightColor, false, lightIntensity );
}

void GPE_BasicLight::setup_light( int x, int y, int intensity, bool activeNow, bool activeOnLoad )
{
    lightX = x;
    lightY = y;
    lightIntensity = intensity;
    lightIsActive = activeNow;
    lightActiveOnStart = activeOnLoad;
}

GPE_Directionight::GPE_Directionight()
{
    lightX2 = lightY2 = 0;
    lightWidth = 64;
    lightLength = 128;
    lightDirection = 0;
    lightTexture = NULL;
}

GPE_Directionight::~GPE_Directionight()
{

}

double GPE_Directionight::get_direction()
{
    return lightDirection;
}

void GPE_Directionight::render_light( double scale, GPE_Rect * cam )
{
    render_light_at(lightX, lightY, scale, cam );
}

void GPE_Directionight::render_light_at( double x, double y, double scale, GPE_Rect * cam )
{
    cam = GPE_find_camera( cam );
    if( lightTexture!=NULL )
    {
        lightTexture->render_tex_rotated( x - cam->x, y - cam->y,lightDirection, lightColor, NULL, lightIntensity );
    }
    else
    {
        lightX2 = x + gpe->lengthdir_x( lightLength*scale,lightDirection ) - cam->x;
        lightY2 = y + gpe->lengthdir_y( lightLength*scale,lightDirection ) - cam->y;
        //gcanvas->render_semi_circle(x,y,lightWidth,lightDirection, false );
        gcanvas->render_line_width_color(x- cam->x, y - cam->y, lightX2, lightY2, lightWidth, lightColor, lightIntensity );
    }
}

void GPE_Directionight::set_direction( double newDir )
{
    while( newDir < -360.d  )
    {
        newDir += 360.d ;
    }
    while( newDir >= 360.d )
    {
        newDir -= 360.d;
    }
    lightDirection = newDir;
}

GPE_PointLight::GPE_PointLight()
{
    lightRadius = 32;
    secondRadius = 16;
    thirdRadius = 0;
    useFlicker = true;
    currentFlicker = 0;
    lightFlickerTime = 0;
    lightFlickerTimeGoal = 250;
    lightFlickerRange = 2;
    secondRadiusIntensity = 255;
    thirdRadiusIntensity = 0;
}

GPE_PointLight::~GPE_PointLight()
{

}

void GPE_PointLight::disable_flicker()
{
    useFlicker = false;
    lightFlickerTime = 0;
    currentFlicker = 0;
}

void GPE_PointLight::enable_flicker()
{
    useFlicker = true;
}

void GPE_PointLight::render_light( double scale, GPE_Rect * cam )
{
    render_light_at(lightX, lightY, scale, cam );
}

void GPE_PointLight::render_light_at( double x, double y, double scale, GPE_Rect * cam )
{
    cam = GPE_find_camera( cam );
    gcanvas->render_circle_color( x, y, (lightRadius+currentFlicker)*scale,lightColor, lightIntensity );
    if( secondRadius > 0 )
    {
        gcanvas->render_circle_color( x, y, (secondRadius+currentFlicker)*scale, lightColor, secondRadiusIntensity );
    }
    if( thirdRadius > 0 )
    {
        gcanvas->render_circle_color( x, y, (thirdRadius+currentFlicker)*scale, lightColor, thirdRadiusIntensity  );
    }
}

void GPE_PointLight::setup_flicker( int fTime, int fRange )
{
    if( fTime > 0 && fRange > 0 )
    {
        enable_flicker();
        lightFlickerTimeGoal = fTime;
        lightFlickerRange = fRange;
        if( currentFlicker < -fRange)
        {
            currentFlicker = -fRange;
        }
        else if( currentFlicker > fRange )
        {
            currentFlicker = fRange;
        }
    }
    else
    {
        disable_flicker();
    }
}

void GPE_PointLight::update_light( int delta )
{
    if( useFlicker )
    {
        lightFlickerTime+=delta;
        if( lightFlickerTime >= lightFlickerTimeGoal )
        {
            currentFlicker = random( -lightFlickerRange, lightFlickerRange );
            lightFlickerTime = 0;
        }
    }
}
