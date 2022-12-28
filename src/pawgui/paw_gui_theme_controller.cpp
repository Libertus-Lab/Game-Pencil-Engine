/*
paw_gui_theme_controller.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

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

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#include "paw_gui_theme_controller.h"

themes_controller::themes_controller()
{

}

themes_controller::~themes_controller()
{

}

GPE_Theme * themes_controller::add_theme(std::string theme_name, bool customTheme )
{
    GPE_Theme * newTemplate = new GPE_Theme(theme_name, customTheme);
    theme_list.push_back( newTemplate);
    return newTemplate;
}

GPE_Theme * themes_controller::find_theme(std::string theme_name )
{
    if( (int)theme_name.size() > 0 )
    {
        for( int i = (int)theme_list.size()-1; i >=0; i-- )
        {
            if( theme_list[i]->theme_name == theme_name)
            {
                return theme_list[i];
            }
        }
    }
    return NULL;
}
