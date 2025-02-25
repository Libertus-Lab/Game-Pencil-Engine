/*
gpe_scene_particle_class.cpp
This file is part of:
GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 Game Pencil Engine contributors ( Contributors Page )

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
#include "gpe_scene_particle_class.h"

GPE_SceneParticleEmitter::GPE_SceneParticleEmitter( pawgui::widget_resource_container *pFolder )
{
    iconTexture = pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magic.png") ;
    branch_type_id = gpe::branch_type::PARTIClE_EMITTER;
    if( projectParentFolder!=nullptr)
    {
        emmitterInEditor = new pawgui::widget_drop_down_resource_menu( "Particle Emitter",projectParentFolder->find_resource_from_name( gpe::resource_type_names_plural[ gpe::resource_type_emitter]),-1,true);
        emmitterInEditor->set_width(192);
    }
    else
    {
        emmitterInEditor = nullptr;
    }
}

GPE_SceneParticleEmitter::~GPE_SceneParticleEmitter()
{

}

void GPE_SceneParticleEmitter::add_typed_elements()
{
    if( panel_inspector!=nullptr )
    {
        //panel_inspector->add_gui_element( light_is_active, true );
    }
}

bool GPE_SceneParticleEmitter::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount,  pawgui::widget_resource_container * localResTypeController )
{
    GPE_SceneBasicClass::build_intohtml5_file( fileTarget, leftTabAmount+1, localResTypeController);
    return true;
}

void GPE_SceneParticleEmitter::process_elements()
{
    GPE_SceneBasicClass::process_elements();
}

void GPE_SceneParticleEmitter::render_branch()
{

}

bool GPE_SceneParticleEmitter::save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=nullptr && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
        *fileTarget << nestedTabsStr+"   GPE_AmbientLight=";
        if( x_posField!=nullptr)
        {
            x_posField->make_valid_number(0);
            *fileTarget <<  x_posField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-0,";
        }
        if( y_posField!=nullptr)
        {
            y_posField->make_valid_number(0);
            *fileTarget << y_posField->get_held_number() << ",";
        }
        else
        {
            *fileTarget << "-0,";
        }
        if( branchColor!=nullptr)
        {
            *fileTarget << branchColor->get_hex_string() << ",";
        }
        else
        {
            *fileTarget << "#FFFF00,";
        }
        if( branchAlpha!=nullptr)
        {
            *fileTarget << stg_ex::int_to_string( branchAlpha->get_value() )<< ",";
        }
        else
        {
            *fileTarget << "255,";
        }
        *fileTarget << widget_name+",,\n";
        GPE_SceneBasicClass::save_branch_data( fileTarget, nestedFoldersIn+1 );
        return true;
    }
    return false;
}
