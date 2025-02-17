/*
path_resource.cpp
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

#include "path_resource.h"


gamePathResource::gamePathResource(pawgui::widget_resource_container * pFolder )
{
    bottomPaneList = new pawgui::widget_panel_list();
    projectParentFolder = pFolder;

    if( projectParentFolder!=nullptr)
    {
        sceneToPreview = new pawgui::widget_drop_down_resource_menu( "Previewed Scene",projectParentFolder->find_resource_from_name( gpe::resource_type_names_plural[ gpe::resource_type_scene]),-1,true );
        sceneToPreview->set_width(128);
    }
    else
    {
        sceneToPreview = nullptr;
    }
    sceneZoomLevel = new pawgui::widget_dropdown_menu( "",true);
    sceneZoomLevel->add_menu_option("10%","10",10,false);
    sceneZoomLevel->add_menu_option("25%","25",25,false);
    sceneZoomLevel->add_menu_option("33%","33",33,false);
    sceneZoomLevel->add_menu_option("50%","50",50,false);
    sceneZoomLevel->add_menu_option("67%","67",67,false);
    sceneZoomLevel->add_menu_option("75%","75",75,false);
    sceneZoomLevel->add_menu_option("80%","80",80,false);
    sceneZoomLevel->add_menu_option("90%","90",90,false);
    sceneZoomLevel->add_menu_option("100%","100",100,true);
    sceneZoomLevel->add_menu_option("110%","110",110,false);
    sceneZoomLevel->add_menu_option("125%","125",125,false);
    sceneZoomLevel->add_menu_option("150%","150",150,false);
    sceneZoomLevel->add_menu_option("175%","175",175,false);
    sceneZoomLevel->add_menu_option("200%","200",200,false);
    sceneZoomLevel->add_menu_option("250%","250",250,false);
    sceneZoomLevel->add_menu_option("300%","300",300,false);
    sceneZoomLevel->add_menu_option("400%","400",400,false);
    sceneZoomLevel->add_menu_option("500%","500",500,false);
    sceneZoomLevel->set_width(96);

    pathOptions = new pawgui::widget_selectbox( "Path Points");
    pathOptions->set_width( 224 );
    pathOptions->set_height(512);
    pathOptions->limit_height(512);

    point_settingsButtton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cogs.png","Path Settings",-1,32);
    pointRemove_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/remove.png", "Remove Point",-1,32 );
    pointMoveUpButtton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-up.png", "Move Point Up",-1,32);
    pointMoveDown_button = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-down.png","Move Point Down",-1,32 );

    pathTypeIsClosed = new pawgui::widget_checkbox("Closed Path?","Check to close the path", false );
    pathShapeType = new pawgui::widget_radio_button_controller("Path Shape");
    pathShapeType->add_menu_option("Lines","lines",0,true);
    pathShapeType->add_menu_option("Curves","curves",1,false);

    zoomValue = 1;
    editorMode = 0;
    sceneEditorView.x = 128;
    sceneEditorView.y = 32;
    sceneEditorView.w = 640;
    sceneEditorView.h = 480;

    scenePreviewRect.x = 0;
    scenePreviewRect.y = 0;
    scenePreviewRect.w = 640;
    scenePreviewRect.h = 480;

    pathLineColor = gpe::c_blue->duplicate_color();
    pathPointColor = gpe::c_orange->duplicate_color();
    sceneXScroll = new pawgui::widget_scrollbar_xaxis();
    update_rectangle(&sceneXScroll->contextRect,0,0,640,480);
    sceneYScroll = new pawgui::widget_scrollbar_yaxis();
    update_rectangle(&sceneYScroll->contextRect,0,0,640,480);
    areaIsScrollable = false;
    local_mouse_x = 0;
    local_mouse_y = 0;

    pointPos = 0;
    selectedPointPos = 0;
    selectedPathPoint = nullptr;
    currentPointX = new pawgui::widget_input_number("PointX",false,0);
    currentPointY = new pawgui::widget_input_number("PointY",false,0);
    currentPointSpeed = new pawgui::widget_input_number("PointSpeed",false,0);
}

gamePathResource::~gamePathResource()
{
    if( bottomPaneList!=nullptr)
    {
        delete bottomPaneList;
        bottomPaneList = nullptr;
    }
    if( sceneZoomLevel!=nullptr)
    {
        delete sceneZoomLevel;
        sceneZoomLevel = nullptr;
    }
    if( pathOptions!=nullptr)
    {
        delete pathOptions;
        pathOptions = nullptr;
    }
    if( point_settingsButtton!=nullptr)
    {
        delete point_settingsButtton;
        point_settingsButtton = nullptr;
    }

    if( pointRemove_button!=nullptr)
    {
        delete pointRemove_button;
        pointRemove_button = nullptr;
    }
    if( pointMoveUpButtton!=nullptr)
    {
        delete pointMoveUpButtton;
        pointMoveUpButtton = nullptr;
    }
    if( pointMoveDown_button!=nullptr)
    {
        delete pointMoveDown_button;
        pointMoveDown_button = nullptr;
    }

    if( sceneToPreview!=nullptr)
    {
        delete sceneToPreview;
        sceneToPreview = nullptr;
    }

    if( sceneXScroll!=nullptr)
    {
        delete sceneXScroll;
        sceneXScroll = nullptr;
    }
    if( sceneYScroll!=nullptr)
    {
        delete sceneYScroll;
        sceneYScroll = nullptr;
    }
    if( pathTypeIsClosed!=nullptr)
    {
        delete pathTypeIsClosed;
        pathTypeIsClosed = nullptr;
    }
    clear_points();
}

gpe::game_path_point2d * gamePathResource::add_point( int point_x, int point_y, float pointSpeed )
{
    if( pathOptions !=nullptr )
    {
        gpe::game_path_point2d * newPoint = new gpe::game_path_point2d(point_x, point_y, pointSpeed);
        if( pointPos >=0 && pointPos < (int)pathPoints.size() )
        {
            pathPoints.insert( pathPoints.begin()+pointPos+1,newPoint );
            pathOptions->insert_option(pointPos+1,"X:"+ stg_ex::int_to_string(point_x)+" Y:"+ stg_ex::int_to_string(point_y)+" Spd:"+ stg_ex::float_to_string(pointSpeed),nullptr,nullptr,-1, true );
            return pathPoints[(int) pathPoints.size()-1 ];
        }
        else
        {
            pathPoints.push_back( newPoint );
            if( pathOptions!=nullptr)
            {
                pathOptions->add_option("X:"+ stg_ex::int_to_string(point_x)+"Y:"+ stg_ex::int_to_string(point_y)+"Spd:"+ stg_ex::float_to_string(pointSpeed),pointPos+1,nullptr,nullptr,-1,true );
            }
        }
        pointPos =  pathOptions->get_selection();
        return pathPoints[ pointPos ];
    }
    return nullptr;
}

bool gamePathResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=nullptr && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( leftTabAmount  );
        std::string html5PathName = "_path_" + get_name();

        *fileTarget << nestedTabsStr << "var " + html5PathName + " =  GPE.add_path(";
        if( pathTypeIsClosed!=nullptr && pathTypeIsClosed->is_clicked() )
        {
            *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) +",true); \n";
        }
        else
        {
            *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) +",false); \n";
        }

        gpe::game_path_point2d * tempPoint = nullptr;
        int pointCount = (int)pathPoints.size();
        for( int pointI = 0; pointI < pointCount; pointI++)
        {
            tempPoint = pathPoints[pointI];
            if( tempPoint!=nullptr )
            {
                *fileTarget << nestedTabsStr+html5PathName+".add_original_point( ";
                *fileTarget << stg_ex::float_to_string(tempPoint->x_pos)+",";
                *fileTarget << stg_ex::float_to_string(tempPoint->y_pos)+",";
                *fileTarget << stg_ex::float_to_string(tempPoint->pointSpeed)+");\n";
            }
        }
        return true;
    }
    return false;
}

bool gamePathResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void gamePathResource::compile_cpp()
{

}

void gamePathResource::clear_points()
{
    selectedPointPos = -1;
    selectedPathPoint = nullptr;
    gpe::game_path_point2d * tempPoint = nullptr;
    for( int i = (int)pathPoints.size()-1; i >=0; i--)
    {
        tempPoint = pathPoints[i];
        if( tempPoint!=nullptr)
        {
            delete tempPoint;
            tempPoint = nullptr;
        }
    }
    pathPoints.clear();
    if( pathOptions!=nullptr)
    {
        pathOptions->clear_list();
    }
}

bool gamePathResource::export_and_play_native( bool launchProgram )
{
    return false;
}

bool gamePathResource::get_mouse_coords( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    local_mouse_x = 0;
    local_mouse_y = 0;
    if( view_space!=nullptr)
    {
        if( gpe::point_within( gpe::input->mouse_position_x, gpe::input->mouse_position_y,
                         sceneEditorView.x,
                         sceneEditorView.y,
                         sceneEditorView.x+sceneEditorView.w,
                         sceneEditorView.y+sceneEditorView.h ) )
        {
            local_mouse_x = ( gpe::input->mouse_position_x-sceneEditorView.x)/zoomValue +scenePreviewRect.x;
            local_mouse_y = ( gpe::input->mouse_position_y-sceneEditorView.y)/zoomValue +scenePreviewRect.y;
            if( spm!=nullptr)
            {
                spm->mouseInScene = true;
                spm->mouseXPos = local_mouse_x;
                spm->mouseYPos = local_mouse_y;
            }
            pawgui::main_overlay_system->update_tooltip( "Mouse( "+ stg_ex::int_to_string(local_mouse_x )+" , "+ stg_ex::int_to_string(local_mouse_y)+")" );
            return true;
        }
    }
    if( spm!=nullptr)
    {
        spm->mouseInScene = true;
        spm->mouseXPos = local_mouse_x;
        spm->mouseYPos = local_mouse_y;
    }
    return false;
}

void gamePathResource::handle_scrolling()
{
    bool editorPaneInUse = false;
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    if( panel_main_editor!=nullptr )
    {
        if( panel_main_editor->hasScrollControl )
        {
            return;
        }
        editorPaneInUse = panel_main_editor->is_inuse();
    }
    if( areaIsScrollable  )
    {
        if( gpe::input->check_kb_down(kb_ctrl) && sceneZoomLevel!=nullptr )
        {
            //Zoom In
            if( gpe::input->mouse_scrolling_down )
            {
                sceneZoomLevel->set_id(sceneZoomLevel->get_selected_id()-1 );
            }
            else if( gpe::input->mouse_scrolling_up)
            {
                //zoom out
                sceneZoomLevel->set_id(sceneZoomLevel->get_selected_id()+1 );
            }

        }
        else if( gpe::input->kb_shift_pressed)
        {
            if( gpe::input->mouse_scrolling_up > 0 )
            {
                xScrollHappened = true;
                scenePreviewRect.x-=(scenePreviewRect.w/16)*zoomValue;
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                //zoom out
                xScrollHappened = true;
                scenePreviewRect.x+=(scenePreviewRect.w/16)*zoomValue;
            }
        }
        else
        {
            if( gpe::input->mouse_scrolling_up )
            {
                yScrollHappened = true;
                scenePreviewRect.y-=(scenePreviewRect.h/16)*zoomValue;
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                yScrollHappened = true;
                scenePreviewRect.y+=(scenePreviewRect.h/16)*zoomValue;
            }
            else if( editorPaneInUse==false )
            {
                //arrow scrolling
                if( gpe::input->check_kb_down(kb_up) )
                {
                    yScrollHappened = true;
                    scenePreviewRect.y-=(scenePreviewRect.h/32)*zoomValue;
                }
                else if( gpe::input->check_kb_down(kb_down) )
                {
                    yScrollHappened = true;
                    scenePreviewRect.y+=(scenePreviewRect.h/32)*zoomValue;
                }
                if( gpe::input->check_kb_down(kb_left) )
                {
                    if( scenePreviewRect.x > (scenePreviewRect.w/32)*zoomValue )
                    {
                        xScrollHappened = true;
                        scenePreviewRect.x-=(scenePreviewRect.w/32)*zoomValue;
                    }
                    else
                    {
                        scenePreviewRect.x = 0;
                        xScrollHappened = true;
                    }
                }
                else if( gpe::input->check_kb_down(kb_right) )
                {
                    if( (scenePreviewRect.x +scenePreviewRect.w/32)*zoomValue < sceneRect.w*zoomValue )
                    {
                        xScrollHappened = true;
                        scenePreviewRect.x+=(scenePreviewRect.w/32)*zoomValue;
                    }
                }
            }
        }
    }

    if( scenePreviewRect.x+scenePreviewRect.w/zoomValue > sceneRect.w )
    {
        xScrollHappened = true;
        scenePreviewRect.x = sceneRect.w-scenePreviewRect.w/zoomValue;
    }

    if( scenePreviewRect.y+scenePreviewRect.h/zoomValue > sceneRect.h )
    {
        yScrollHappened = true;
        scenePreviewRect.y = sceneRect.h-scenePreviewRect.h/zoomValue;
    }

    if( scenePreviewRect.x <= 0)
    {
        xScrollHappened = true;
        scenePreviewRect.x = 0;
    }

    if( scenePreviewRect.y <= 0)
    {
        yScrollHappened = true;
        scenePreviewRect.y = 0;
    }

    if( xScrollHappened)
    {
        sceneXScroll->contextRect.x = scenePreviewRect.x;
        sceneXScroll->process_self(nullptr,nullptr);
    }
    if( yScrollHappened)
    {
        sceneXScroll->contextRect.y = scenePreviewRect.y;
        sceneYScroll->process_self(nullptr,nullptr);
    }
}

bool gamePathResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}


bool gamePathResource::is_build_ready()
{
    recentErrorMessage = "";
    if( (int)pathPoints.size() == 0)
    {
        recentErrorMessage = "NoPointsselected;";
        return false;
    }
    return true;
}

void gamePathResource::integrate_into_syntax()
{

}

void gamePathResource::open_code(int lineNumb, int colNumb, std::string codeTitle )
{

}

void gamePathResource::prerender_self( )
{

}

void gamePathResource::load_resource(std::string file_path )
{
    if( resourcePostProcessed ==false  || sff_ex::file_exists(file_path) )
    {
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->update_submessages( "Processing Path",resource_name );
        }
        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/paths/";
        if( sff_ex::file_exists(file_path) )
        {
            newFileIn = file_path;
            soughtDir = stg_ex::get_path_from_file(newFileIn);
        }
        else
        {
            newFileIn = soughtDir + resource_name+".gpf";
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        gpe::error_log->report("Loading Path - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
        {
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string key_string="";
                std::string valstring="";
                std::string subValstring="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                float foundFileVersion = 0;
                int foundX = 0;
                int foundY = 0;
                int foundSpeed = 0;
                int foundWaiver = 0;
                int foundR = 0, foundG = 0, foundB = 0;
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                    currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

                    if( foundFileVersion <=0)
                    {
                        //Empty Line skipping is only allowed at the top of the file
                        if(!currLineToBeProcessed.empty() )
                        {
                            //Comment skipping is only allowed at the top of the file
                            if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                            {
                                //searches for an equal character and parses through the variable
                                equalPos = currLineToBeProcessed.find_first_of("=");
                                if(equalPos!=(int)std::string::npos)
                                {
                                    //if the equalPos is present, then parse on through and carryon
                                    key_string = currLineToBeProcessed.substr(0,equalPos);
                                    valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( key_string=="Version")
                                    {
                                        foundFileVersion = stg_ex::string_to_float(valstring);
                                    }
                                }
                            }
                        }
                    }
                    else if( foundFileVersion <= 2)
                    {
                        //Begin processing the file.
                        if(!currLineToBeProcessed.empty() )
                        {
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                key_string = currLineToBeProcessed.substr(0,equalPos);
                                valstring = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                                if( key_string=="ResourceName")
                                {
                                    renameBox->set_string(valstring);
                                }
                                else if( key_string=="Point" || key_string=="point" )
                                {
                                    foundX = stg_ex::split_first_int(valstring,',');
                                    foundY = stg_ex::split_first_int(valstring,',');
                                    foundSpeed = stg_ex::split_first_int(valstring,',');
                                    foundWaiver = stg_ex::split_first_int(valstring,',');
                                    add_point( foundX, foundY, foundSpeed );
                                }
                                else if ( key_string == "PathIsClosed" && pathTypeIsClosed!=nullptr )
                                {
                                    pathTypeIsClosed->set_checked( stg_ex::string_to_bool(valstring) );
                                }
                                else if ( key_string == "pathType" && pathTypeIsClosed!=nullptr )
                                {
                                    //Processes old path type from previous version
                                    if( stg_ex::split_first_string(valstring, ",")=="Closed Path" )
                                    {
                                        pathTypeIsClosed->set_checked( true );
                                    }
                                    else
                                    {
                                        pathTypeIsClosed->set_checked( false);
                                    }
                                }
                                else if ( key_string == "LineColor"|| key_string=="PathLineColor")
                                {
                                    foundR = stg_ex::split_first_int(valstring, ',');
                                    foundG = stg_ex::split_first_int(valstring, ',');
                                    foundB = stg_ex::split_first_int(valstring, ',');
                                    if( pathLineColor!=nullptr)
                                    {
                                        pathLineColor->change_rgba(foundR,foundG, foundB);
                                    }
                                }

                                else if ( key_string == "PointColor"|| key_string=="PathPointColor")
                                {
                                    foundR = stg_ex::split_first_int(valstring, ',');
                                    foundG = stg_ex::split_first_int(valstring, ',');
                                    foundB = stg_ex::split_first_int(valstring, ',');
                                    if( pathPointColor!=nullptr)
                                    {
                                        pathPointColor->change_rgba(foundR,foundG, foundB);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        gpe::error_log->report("Invalid FoundFileVersion ="+ stg_ex::float_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
    }
}

void gamePathResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( spm !=nullptr )
    {
        spm->editMode = SCENE_MODE_NONE;
        spm->zoomValue = zoomValue;
    }
    if( projectParentFolder!=nullptr)
    {
        sceneEditorView.x = view_space->x;
        sceneEditorView.y = view_space->y;
        sceneEditorView.w = scenePreviewRect.w = (int)(view_space->w - sceneYScroll->get_box_width() )-16; //xcroll = 16px height
        sceneEditorView.h = scenePreviewRect.h = view_space->h-64; //Comment pane = 64, yscroll = 16 height

        sceneRect.x = 0;
        sceneRect.y = 0;
        sceneRect.w = 8192;
        sceneRect.h = 8192;

        editorCommentPane.x =0;
        editorCommentPane.y = sceneEditorView.h+16;
        editorCommentPane.w = view_space->w;
        editorCommentPane.h = view_space->h - editorCommentPane.y;

        pawgui::widget_resource_container * sceneTypeContainer =  sceneToPreview->get_selected_container();
        if( sceneTypeContainer!=nullptr && sceneTypeContainer->get_held_resource()!=nullptr )
        {
            pawgui::general_resource * sceneGenObject = sceneTypeContainer->get_held_resource();
            if( sceneGenObject!=nullptr)
            {
                gameSceneResource * foundSceneObject = (gameSceneResource *)sceneGenObject;
                if( foundSceneObject!=nullptr)
                {
                    sceneRect.w = foundSceneObject->sceneRect.w;
                    sceneRect.h = foundSceneObject->sceneRect.h;
                }
            }
        }


        if( pathOptions !=nullptr )
        {
            pathOptions->set_height( view_space->h - 160 );
            pathOptions->limit_height( view_space->h - 160 );
        }

        if(cam!=nullptr && view_space!=nullptr  )
        {
            //For bottom pane
            bottomPaneList->clear_list();
            bottomPaneList->set_coords( editorCommentPane.x, editorCommentPane.y );
            bottomPaneList->set_width( view_space->w );
            bottomPaneList->set_height( view_space->h - editorCommentPane.y );
            bottomPaneList->alignment_h = gpe::fa_left;
            bottomPaneList->alignment_v = gpe::fa_middle;
            bottomPaneList->barXPadding = pawgui::padding_default;
            bottomPaneList->barYPadding = 0;
            bottomPaneList->barXMargin  = pawgui::padding_default;
            bottomPaneList->barYMargin  = pawgui::padding_default;
            bottomPaneList->add_gui_element( sceneToPreview, false );
            bottomPaneList->add_gui_element( sceneZoomLevel, false );
            bottomPaneList->add_gui_element( pathTypeIsClosed, false );
            bottomPaneList->process_self( view_space, cam );

            if( sceneZoomLevel!=nullptr)
            {
                zoomValue = sceneZoomLevel->get_selected_value();
                if( zoomValue < 0)
                {
                    zoomValue = 1;
                    sceneZoomLevel->set_option_value(100);
                }
                zoomValue/=100;
            }
            else
            {
                zoomValue = 1;
            }

            if( panel_main_editor!=nullptr )
            {
                //Now back to the editor pane
                panel_main_editor->add_gui_element(renameBox,true);
                panel_main_editor->add_gui_element(pathOptions,true);
                if( selectedPathPoint!=nullptr)
                {
                    panel_main_editor->add_gui_element(currentPointX,true);
                    panel_main_editor->add_gui_element(currentPointY,true);
                    panel_main_editor->add_gui_element(currentPointSpeed,true);
                }
                panel_main_editor->add_gui_element(point_settingsButtton,false);
                panel_main_editor->add_gui_element(pointRemove_button,false);
                panel_main_editor->add_gui_element(pointMoveUpButtton,false);
                panel_main_editor->add_gui_element(pointMoveDown_button,true);
                //panel_main_editor->add_gui_element(pathShapeType,true);


                panel_main_editor->add_gui_element(confirmResource_button,true);
                panel_main_editor->add_gui_element(cancelResource_button,true);
                panel_main_editor->process_self(nullptr, nullptr);

                if( pathOptions!=nullptr)
                {
                    pointPos = pathOptions->get_selection();
                    if( selectedPointPos !=pointPos )
                    {
                        selectedPointPos = -1;
                        selectedPathPoint = nullptr;
                    }
                }

                if( point_settingsButtton!=nullptr && point_settingsButtton->is_clicked() )
                {
                    pawgui::context_menu_open(-1,-1,256);

                    pawgui::main_context_menu->add_menu_option("Change Points Color",1,nullptr,-1,nullptr,false,true);
                    pawgui::main_context_menu->add_menu_option("Change Lines Color",2,nullptr,-1,nullptr,false,true);
                    pawgui::main_context_menu->add_menu_option("Clear Point List",3,nullptr,-1,nullptr,false,true);
                    int menuSelection = pawgui::context_menu_process();
                    if( menuSelection ==1)
                    {
                        pawgui::get_color_from_popup("Change Points Color",pathPointColor);
                    }
                    else if( menuSelection ==2)
                    {
                        pawgui::get_color_from_popup("Change Points Color",pathLineColor);
                    }
                    else if( menuSelection ==3)
                    {
                        if( (int)pathPoints.size() > 0 )
                        {
                            if( (int)pathPoints.size() <=5)
                            {
                                clear_points();
                            }
                            else
                            {
                                if( pawgui::display_prompt_message("Path Clear","Are you sure you will like to clear this path?") ==pawgui::display_query_yes )
                                {
                                    clear_points();
                                }
                            }
                        }
                    }
                }
                else if( pointRemove_button!=nullptr && pointRemove_button->is_clicked() )
                {
                    remove_point( pointPos );
                    selectedPointPos = -1;
                    selectedPathPoint = nullptr;
                }
                else if( confirmResource_button->is_clicked() )
                {
                    save_resource();
                }
                else if( cancelResource_button->is_clicked() )
                {
                    if( pawgui::display_prompt_message("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
                    {
                        resourcePostProcessed = false;
                        load_resource();
                    }
                }
            }
        }


        //if( panel_main_editor->hasScrollControl==false && layerPaneList->hasScrollControl==false )
        {
            //Horizontal scrolling
            sceneXScroll->update_box( 0,sceneEditorView.h,sceneEditorView.w,16);
            update_rectangle(&sceneXScroll->fullRect,0,0,(float)sceneRect.w, (float)sceneRect.h );
            update_rectangle(&sceneXScroll->contextRect,(float)scenePreviewRect.x,(float)scenePreviewRect.y, (float)scenePreviewRect.w/zoomValue, (float)scenePreviewRect.h/zoomValue );
            sceneXScroll->process_self(view_space,cam );
            //if( sceneXScroll->has_moved() )
            {
                scenePreviewRect.x = (float)(sceneXScroll->contextRect.x);
            }

            //Vertical Scrolling
            sceneYScroll->update_box( sceneEditorView.w,0,16,sceneEditorView.h );
            update_rectangle(&sceneYScroll->fullRect,0,0,(float)sceneRect.w, (float)sceneRect.h );
            update_rectangle(&sceneYScroll->contextRect,(float)scenePreviewRect.x,(float)scenePreviewRect.y, (float)scenePreviewRect.w/zoomValue, (float)scenePreviewRect.h/zoomValue );
            //sceneYScroll->contextRect.h = sceneEditorView.h;
            sceneYScroll->process_self(view_space,cam );
            //if( sceneYScroll->has_moved() )
            {
                scenePreviewRect.y = float(sceneYScroll->contextRect.y);
            }
        }

        if( gpe::input->has_new_input() )
        {
            get_mouse_coords(view_space,cam);
        }
        //if( gpe::input->check_mouse_pressed(0) ||  gpe::input->check_mouse_pressed(1) || gpe::input->check_mouse_pressed(2) )
        {
            if( get_mouse_coords(view_space,cam) )
            {
                areaIsScrollable = true;
                if( panel_main_editor!=nullptr)
                {
                    panel_main_editor->hasScrollControl = false;
                }
            }
            else
            {
                areaIsScrollable = false;
            }
        }

        int pointSize = std::max( 1, (int)( 7*zoomValue) );
        gpe::game_path_point2d * tempPoint = nullptr;
        bool foundPoint  = false;
        int i = 0;
        if( gpe::input->check_mouse_down( mb_left ) )
        {
            if( get_mouse_coords(view_space,cam) )
            {
                if( local_mouse_x-scenePreviewRect.x < 0 )
                {
                    scenePreviewRect.x = 0;
                }

                if( local_mouse_y-scenePreviewRect.y < 0 )
                {
                    scenePreviewRect.y = 0;
                }
            }
            /*if(sceneYScroll->is_held() )
            {
                editorCameraRect.x+=16;
            }
            if(sceneXScroll->is_held() )
            {
                //editorCameraRect.y+=16;
            }*/
        }
        else if( gpe::input->check_mouse_released( mb_left))
        {
            //Adds, selects or moves a point
            if( get_mouse_coords(view_space,cam) )
            {
                if( selectedPathPoint!=nullptr)
                {
                    selectedPathPoint->x_pos = local_mouse_x;
                    selectedPathPoint->y_pos = local_mouse_y;
                    if( currentPointX !=nullptr)
                    {
                        currentPointX->set_number( local_mouse_x );
                    }
                    if( currentPointY !=nullptr)
                    {
                        currentPointY->set_number( local_mouse_y );
                    }
                    if( pathOptions!=nullptr)
                    {
                        pathOptions->rename_option( selectedPointPos, "X:"+ stg_ex::int_to_string(selectedPathPoint->x_pos)+" Y:"+ stg_ex::int_to_string(selectedPathPoint->y_pos)+" Spd:"+ stg_ex::float_to_string(selectedPathPoint->pointSpeed) );
                    }
                }
                else
                {
                    //searches if the mouse overlaps with a point on the screen.
                    for( i = (int)pathPoints.size()-1; i >=0 && !foundPoint; i--)
                    {
                        tempPoint = pathPoints[i];
                        if( tempPoint!=nullptr)
                        {
                            // if found change its coordinates
                            if( gpe::point_between( (int)local_mouse_x,(int)local_mouse_y, tempPoint->x_pos -pointSize,tempPoint->y_pos - pointSize,tempPoint->x_pos +pointSize,tempPoint->y_pos +pointSize) )
                            {
                                foundPoint = true;
                                selectedPathPoint = tempPoint;
                                if( currentPointX !=nullptr)
                                {
                                    currentPointX->set_number( selectedPathPoint->x_pos );
                                }
                                if( currentPointY !=nullptr)
                                {
                                    currentPointY->set_number( selectedPathPoint->y_pos );
                                }
                                if( currentPointSpeed !=nullptr)
                                {
                                    currentPointSpeed->set_number( selectedPathPoint->pointSpeed );
                                }
                                pointPos = i;
                                selectedPointPos = i;
                                if( pathOptions!=nullptr)
                                {
                                    pathOptions->set_selection( i);
                                }
                            }
                        }
                    }

                    if( selectedPathPoint == nullptr)
                    {
                        pointPos = pathOptions->get_selection();
                        gpe::game_path_point2d * newPoint = add_point(local_mouse_x, local_mouse_y, 1);
                    }
                }
            }
        }
        else if( gpe::input->check_mouse_released(1) )
        {
            //removes points
            if( get_mouse_coords(view_space,cam) )
            {
                for( i = (int)pathPoints.size()-1; i >=0 && !foundPoint; i--)
                {
                    tempPoint = pathPoints[i];
                    if( tempPoint!=nullptr)
                    {
                        if( gpe::point_between( (int)local_mouse_x,(int)local_mouse_y, tempPoint->x_pos -pointSize,tempPoint->y_pos - pointSize,tempPoint->x_pos +pointSize,tempPoint->y_pos +pointSize) )
                        {
                            foundPoint = true;
                            remove_point( i );
                        }
                    }
                }
            }
            selectedPathPoint = nullptr;
            selectedPointPos = -1;
        }
        else if( selectedPathPoint!=nullptr)
        {
            bool foundPointChange = false;
            float foundNumber = 0;
            if( currentPointX!=nullptr && currentPointX->is_valid() )
            {
                foundNumber = currentPointX->get_held_number();
                if( foundNumber >=0 && foundNumber!=selectedPathPoint->x_pos)
                {
                    selectedPathPoint->x_pos = foundNumber;
                    foundPointChange = true;
                }
            }

            if( currentPointX!=nullptr && currentPointX->is_valid() )
            {
                foundNumber = currentPointY->get_held_number();
                if( foundNumber >=0 && foundNumber!=selectedPathPoint->y_pos)
                {
                    selectedPathPoint->y_pos = foundNumber;
                    foundPointChange = true;
                }
            }

            if( pathOptions!=nullptr && foundPointChange)
            {
                pathOptions->rename_option( selectedPointPos, "X:"+ stg_ex::int_to_string(selectedPathPoint->x_pos)+" Y:"+ stg_ex::int_to_string(selectedPathPoint->y_pos)+" Spd:"+ stg_ex::float_to_string( selectedPathPoint->pointSpeed) );
            }
        }
        handle_scrolling();
    }
}

bool gamePathResource::remove_point( int pointId )
{
    if( pointId >=0 && pointId < (int)pathPoints.size() )
    {
        gpe::game_path_point2d * tempPoint = pathPoints[pointId];
        if( tempPoint!=nullptr )
        {
            delete tempPoint;
            tempPoint = nullptr;
        }
        pathPoints.erase(pathPoints.begin()+ pointId );
        if( pathOptions!=nullptr )
        {
            pathOptions->remove_option( pointId );
        }

        if( pointPos >=pointId)
        {
            pointId-=1;
        }
        pointPos--;
        if( pointPos < 0)
        {
            pointPos = 0;
        }

        selectedPathPoint = nullptr;
        selectedPointPos = -1;
        return true;

    }
    return false;
}

void gamePathResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    gpe::renderer_main->reset_viewpoint(  );
    gpe::renderer_main->set_viewpoint( &sceneEditorView );
    if( cam!=nullptr && view_space!=nullptr)
    {
        pawgui::widget_resource_container * sceneTypeContainer =  sceneToPreview->get_selected_container();
        if( sceneTypeContainer!=nullptr && sceneTypeContainer->get_held_resource()!=nullptr )
        {
            pawgui::general_resource * sceneGenObject = sceneTypeContainer->get_held_resource();
            if( sceneGenObject!=nullptr)
            {
                gameSceneResource * foundSceneObject = (gameSceneResource *)sceneGenObject;
                if( foundSceneObject!=nullptr)
                {

                    foundSceneObject->render_scene_layers( view_space, cam, &scenePreviewRect,zoomValue,false, false);
                }
            }
        }

        //Draws the points and lines
        gpe::game_path_point2d * tempPoint = nullptr;
        int tempXPoint = 0, tempYPoint = 0;
        int tempX2Point = 0, tempY2Point = 0;
        int pointI = 0;
        int pointCount = (int)pathPoints.size();
        for( pointI = 0; pointI < pointCount; pointI++)
        {
            tempPoint = pathPoints[pointI];
            if( tempPoint!=nullptr )
            {
                tempXPoint = floor( tempPoint->x_pos*zoomValue - scenePreviewRect.x*zoomValue );
                tempYPoint = floor( tempPoint->y_pos*zoomValue - scenePreviewRect.y*zoomValue );
                if( pointI >=1)
                {
                    gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathLineColor, 255);
                    gpe::gcanvas->render_bezier_curve_width_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point,4, pathLineColor, 255);
                }
                tempX2Point = tempXPoint;
                tempY2Point = tempYPoint;
            }
        }

        if( pathTypeIsClosed!=nullptr && pointCount >2)
        {
            tempPoint = pathPoints[0];
            if( tempPoint!=nullptr && pathTypeIsClosed->is_clicked() )
            {
                tempXPoint = floor( tempPoint->x_pos*zoomValue - scenePreviewRect.x*zoomValue );
                tempYPoint = floor( tempPoint->y_pos*zoomValue - scenePreviewRect.y*zoomValue );
                gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathLineColor, 255);
                gpe::gcanvas->render_bezier_curve_width_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, 4,pathLineColor, 255);
            }
        }

        for( pointI = 0; pointI < (int)pathPoints.size(); pointI++)
        {
            tempPoint = pathPoints[pointI];
            if( tempPoint!=nullptr )
            {
                tempXPoint = floor( tempPoint->x_pos*zoomValue - scenePreviewRect.x*zoomValue );
                tempYPoint = floor( tempPoint->y_pos*zoomValue - scenePreviewRect.y*zoomValue );
                if( selectedPointPos == pointI)
                {
                    gpe::gcanvas->render_circle_filled_color( tempXPoint,tempYPoint, std::max( 1, (int)( 7*zoomValue) ),pathLineColor, 255 );
                }
                else
                {
                    gpe::gcanvas->render_circle_filled_color( tempXPoint,tempYPoint, std::max( 1, (int)( 7*zoomValue) ),pathPointColor, 255 );
                }
                //gpe::gcanvas->render_rectangle( tempXPoint, tempYPoint, tempXPoint+4, tempYPoint+4,pathPointColor, false, 255);
                //gpe::error_log->report("Rendering point ["+ stg_ex::int_to_string(pointI)+"] at ("+ stg_ex::int_to_string(tempXPoint)+","+ stg_ex::int_to_string(tempYPoint)+")");
            }
        }

        if( selectedPathPoint!=nullptr)
        {
            tempXPoint = floor( local_mouse_x*zoomValue - scenePreviewRect.x*zoomValue );
            tempYPoint = floor( local_mouse_y*zoomValue - scenePreviewRect.y*zoomValue );
            gpe::gcanvas->render_circle_filled_color( tempXPoint,tempYPoint, std::max( 1, (int)( 7*zoomValue) ),pathLineColor, 128 );

            if( selectedPointPos >=0 && selectedPointPos < (int)pathPoints.size() && pointCount >=2 )
            {
                if( selectedPointPos == 0)
                {
                    tempPoint = pathPoints[1];
                    if( tempPoint!=nullptr )
                    {
                        tempX2Point = floor( tempPoint->x_pos*zoomValue - scenePreviewRect.x*zoomValue );
                        tempY2Point = floor( tempPoint->y_pos*zoomValue - scenePreviewRect.y*zoomValue );
                        gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                    }

                    if( pathTypeIsClosed!=nullptr && pathTypeIsClosed->is_clicked()  && pointCount >=3 )
                    {
                        tempPoint = pathPoints[ pointCount - 1];
                        if( tempPoint!=nullptr)
                        {
                            tempX2Point = floor( tempPoint->x_pos*zoomValue - scenePreviewRect.x*zoomValue );
                            tempY2Point = floor( tempPoint->y_pos*zoomValue - scenePreviewRect.y*zoomValue );
                            gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                        }
                    }
                }
                else if( selectedPointPos == pointCount-1 )
                {
                    if( pointCount >2 )
                    {
                        tempPoint = pathPoints[ selectedPointPos -1];
                        if( tempPoint!=nullptr )
                        {
                            tempX2Point = floor( tempPoint->x_pos*zoomValue - scenePreviewRect.x*zoomValue );
                            tempY2Point = floor( tempPoint->y_pos*zoomValue - scenePreviewRect.y*zoomValue );
                            gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                        }

                        if( pathTypeIsClosed!=nullptr && pathTypeIsClosed->is_clicked()  && pointCount >=3 )
                        {
                            tempPoint = pathPoints[ 0];
                            if( tempPoint!=nullptr)
                            {
                                tempX2Point = floor( tempPoint->x_pos*zoomValue - scenePreviewRect.x*zoomValue );
                                tempY2Point = floor( tempPoint->y_pos*zoomValue - scenePreviewRect.y*zoomValue );
                                gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                            }
                        }
                    }
                }
                else
                {
                    tempPoint = pathPoints[ selectedPointPos-1];
                    if( tempPoint!=nullptr)
                    {
                        tempX2Point = floor( tempPoint->x_pos*zoomValue - scenePreviewRect.x*zoomValue );
                        tempY2Point = floor( tempPoint->y_pos*zoomValue - scenePreviewRect.y*zoomValue );
                        gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                    }

                    tempPoint = pathPoints[ selectedPointPos+1];
                    if( tempPoint!=nullptr)
                    {
                        tempX2Point = floor( tempPoint->x_pos*zoomValue - scenePreviewRect.x*zoomValue );
                        tempY2Point = floor( tempPoint->y_pos*zoomValue - scenePreviewRect.y*zoomValue );
                        gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                    }
                }
            }
        }
        //"Editor View:("+ stg_ex::int_to_string(editorCameraRect.x)+","+ stg_ex::int_to_string(editorCameraRect.y)+","+ stg_ex::int_to_string(editorCameraRect.w)+","+ stg_ex::int_to_string(editorCameraRect.h)+")",

        gpe::renderer_main->reset_viewpoint(  );
        gpe::renderer_main->set_viewpoint( view_space );
        gpe::gcanvas->render_rect( &editorCommentPane,pawgui::theme_main->program_color,false);
        //gpe::gcanvas->render_rectangle( sceneYScroll->widget_box.x+sceneYScroll->widget_box.w,0,view_space->w,view_space->h,pawgui::theme_main->program_color,false);
        //gpe::gcanvas->render_rectangle( sceneXScroll->widget_box.x,sceneXScroll->widget_box.y,view_space->w,view_space->h,pawgui::theme_main->program_color,false);
        if( sceneXScroll!=nullptr)
        {
            sceneXScroll->render_self( view_space,cam);
        }
        if( sceneYScroll!=nullptr)
        {
            sceneYScroll->render_self( view_space,cam);
        }
        //gpe::gcanvas->render_rect( &editorPane,pawgui::theme_main->program_color,false);
    }

    if( bottomPaneList!=nullptr )
    {
        bottomPaneList->render_self(  view_space, cam );
    }
}

void gamePathResource::save_resource(std::string file_path, int backupId )
{
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->update_submessages( "Saving Path",resource_name );
    }

    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = stg_ex::get_path_from_file(file_path);
    if( sff_ex::path_exists(soughtDir) )
    {
        newFileOut = file_path;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/paths/";
        newFileOut = soughtDir + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the font file could be saved
    if( !newSaveDataFile.fail() && newSaveDataFile.is_open() )
    {
        write_header_on_file(&newSaveDataFile);
        newSaveDataFile << "#     \n";
        if( sceneToPreview!=nullptr)
        {
            newSaveDataFile << "RelevantScene=" << sceneToPreview->get_selected_name() << "\n";
        }
        else
        {
            newSaveDataFile << "RelevantScene=-1\n";
        }

        if( pathTypeIsClosed!=nullptr)
        {
            newSaveDataFile << "PathIsClosed="+ stg_ex::int_to_string( pathTypeIsClosed->is_clicked() )+"\n";
        }

        if( pathLineColor!=nullptr)
        {
            newSaveDataFile << "LineColor="+ stg_ex::int_to_string( (int)pathLineColor->get_r() )+","+ stg_ex::int_to_string( (int)pathLineColor->get_g() )+","+ stg_ex::int_to_string( (int)pathLineColor->get_b() )+",\n";
        }

        if( pathPointColor!=nullptr)
        {
            newSaveDataFile << "PointColor="+ stg_ex::int_to_string( (int)pathPointColor->get_r() )+","+ stg_ex::int_to_string( (int)pathPointColor->get_g() )+","+ stg_ex::int_to_string( (int)pathPointColor->get_b() )+",\n";
        }
        std::string resfile_location = "";
        std::string resFileCopySrc;
        std::string resFileCopyDest;
        gpe::game_path_point2d * tempPoint = nullptr;
        for( int i = 0; i < (int)pathPoints.size(); i++)
        {
            tempPoint = pathPoints[i];
            if( tempPoint!=nullptr )
            {
                newSaveDataFile << "Point="+ stg_ex::int_to_string( tempPoint->x_pos )+","+ stg_ex::int_to_string( tempPoint->y_pos )+","+ stg_ex::float_to_string( tempPoint->pointSpeed )+",\n";
            }
        }

        newSaveDataFile.close();
        if( !usingAltSaveSource)
        {
            isModified = false;
        }
        if( main_gpe_splash_page != nullptr )
        {
            main_gpe_splash_page->increment_and_update( "Path Successfully Saved!",resource_name );
        }
        return;
    }

    main_editor_log->log_general_error("Unable to save to file ["+newFileOut+"]");
    if( main_gpe_splash_page != nullptr )
    {
        main_gpe_splash_page->increment_and_update( "Unable to save to file",resource_name );
    }
}


int gamePathResource::search_for_string(std::string needle)
{
    return 0;
}

int gamePathResource::search_and_replace_string(std::string needle, std::string newStr )
{
    return 0;
}

void gamePathResource::update_project_layers()
{

}

bool gamePathResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=nullptr)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Path=" << resource_name << "," << get_global_rid() << ",";

            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}
