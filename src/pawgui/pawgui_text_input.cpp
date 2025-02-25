/*
pawgui_text_input.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2023 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2023 PawByte LLC.
Copyright (c) 2014-2023 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#include "pawgui_text_input.h"

namespace pawgui
{
    widget_input_text::widget_input_text( std::string startName,std::string placeHolderText)
    {
        inputFieldPadding = 8;
        inputIsPassword = false;
        resubmissionAllowed = false;
        widget_name = "";
        lastfloatClickAction = 0;
        widget_type = "inputtext";
        cursorPos = 0;
        prevCursorPos =0;
        cursorHistory_pos = -1;
        showPlaceHolder = true;
        if( (int)startName.size()>512)
        {
            startName = startName.substr(0,512);
        }
        textInputstring = startName;
        cursorPos = (int) textInputstring.size();
        showLabel = false;
        inputLabel = "";
        if( (int)placeHolderText.size()>512)
        {
            placeHolderText = placeHolderText.substr(0,512);
        }
        placeHolderstring = placeHolderText;
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 192;
        widget_box.h = 24;

        font_textinput->get_metrics("A",&font_sizeW,&font_sizeH);
        if( font_sizeW<=0)
        {
            font_sizeW = 12;
        }
        maxCharactersInView = widget_box.w/font_sizeW;

        inputFieldPos = 0;
        hasValidInput = true;
        showCursor = false;
        cursorTimer = 0;
        startXPos = 0;
        upArrowDelay = 0;
        downArrowDelay = 0;
        leftArrowDelay = 0;
        rightArrowDelay = 0;
        enterDelay = 0;
        bscDelay = 0;
        delDelay = 0;
        selectionCursorPos = selectionEndCursorPos = 0;
        fieldElementBox.x = widget_box.x;
        fieldElementBox.y = widget_box.y;
        prevwidget_boxW = fieldElementBox.w = widget_box.w;
        prevwidget_boxH = fieldElementBox.h = widget_box.h;
    }

    widget_input_text::~widget_input_text()
    {

    }

    std::string widget_input_text::get_data()
    {
        return widget_type+":"+widget_name+"==|||=="+textInputstring;
    }

    void widget_input_text::load_data(std::string datastring)
    {
        textInputstring = datastring;
    }

    void widget_input_text::copy_selection()
    {
        if( selectionCursorPos >=0 && selectionCursorPos < (int)textInputstring.size() && selectionEndCursorPos >=0 && selectionEndCursorPos <= (int)textInputstring.size() )
        {
            int minHighlightPos = std::min(selectionCursorPos,selectionEndCursorPos);
            int maxHighlightPos = std::max(selectionCursorPos,selectionEndCursorPos);
            std::string strToCopy = stg_ex::get_substring(textInputstring,minHighlightPos,maxHighlightPos-minHighlightPos);
            gpe::input->clipboard_set( strToCopy );
            //main_overlay_system->update_temporary_message("Selection copied","","",0.25);
        }
    }

    void widget_input_text::cut_selection()
    {
        copy_selection();
        delete_selection();
    }

    void widget_input_text::delete_selection()
    {
        if(  (int)textInputstring.size() > 0 && selectionCursorPos!=selectionEndCursorPos )
        {
            int minHighlightPos = std::min(selectionCursorPos,selectionEndCursorPos);
            int maxHighlightPos = std::max(selectionCursorPos,selectionEndCursorPos);
            if(minHighlightPos >=0 && minHighlightPos < (int)textInputstring.size() && maxHighlightPos >0 && maxHighlightPos <= (int)textInputstring.size() )
            {
                std::string leftSidestring = stg_ex::get_substring(textInputstring,0,minHighlightPos);
                std::string rightSidestring = stg_ex::get_substring(textInputstring,maxHighlightPos);
                textInputstring = leftSidestring+rightSidestring;
                selectionCursorPos = 0;
                selectionEndCursorPos = 0;
                cursorPos = minHighlightPos;
                if( startXPos >minHighlightPos)
                {
                    startXPos = minHighlightPos;
                }
            }
        }
    }

    void widget_input_text::get_cursor_from_mouse( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        cursorPos = floor( startXPos+(gpe::input->mouse_position_x-view_space->x-fieldElementBox.x)/font_sizeW);
        if( cursorPos < 0)
        {
            cursorPos = 0;
        }
        if( cursorPos > (int)textInputstring.size() )
        {
            cursorPos = (int)textInputstring.size();
        }
    }

    std::string widget_input_text::get_plain_string()
    {
        if( stg_ex::is_alnum(textInputstring,true,true) )
        {
            return textInputstring;
        }
        else
        {
            return "''";
        }
    }

    std::string widget_input_text::get_label()
    {
        return inputLabel;
    }

    std::string widget_input_text::get_string()
    {
        return textInputstring;
    }

    bool widget_input_text::has_content()
    {
        return ( (int)textInputstring.size() > 0);
    }

    bool widget_input_text::is_valid()
    {
        return hasValidInput;
    }

    void widget_input_text::move_left(int xToMove )
    {
        if( xToMove > 0)
        {
            startXPos-=xToMove;
            if( startXPos < 0 )
            {
                startXPos = 0;
            }
        }
    }

    void widget_input_text::move_right(int xToMove )
    {
        int currStrSize = (int)textInputstring.size();
        if( xToMove > 0 && currStrSize > maxCharactersInView )
        {
            startXPos+=xToMove;
            if( startXPos >= currStrSize-maxCharactersInView )
            {
                startXPos = currStrSize-maxCharactersInView;
            }
            if( startXPos >= currStrSize )
            {
                startXPos = currStrSize-1;
            }
            if( startXPos < 0 )
            {
                startXPos = 0;
            }
        }
    }

    void widget_input_text::process_self( gpe::shape_rect * view_space, gpe::shape_rect *cam)
    {
        font_textinput->get_metrics("A",&font_sizeW,&font_sizeH);
        if( font_sizeW<=0)
        {
            font_sizeW = 12;
        }
        prevCursorPos = cursorPos;
        inputSubmitted = false;
        bool pasteCommandGiven = false;
        view_space = gpe::camera_find(view_space);
        maxCharactersInView = widget_box.w/font_sizeW;
        cam = gpe::camera_find(cam);
        fieldElementBox.x = widget_box.x;
        fieldElementBox.y = widget_box.y;
        fieldElementBox.w = prevwidget_boxW;
        fieldElementBox.h = prevwidget_boxH;
        if( showLabel )
        {
            set_label(inputLabel);
        }
        /*
        Old Protocol for Label Fields
        widget_basic::process_self(view_space,cam);

        New Protocol for labeled fields

        */
        //
        if( isEnabled)
        {
            isClicked = false;
            isHovered = false;
            clickedOutside = false;
            view_space = gpe::camera_find(view_space);
            cam = gpe::camera_find(cam);
            if(view_space!=nullptr && cam!=nullptr)
            {
                if( gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,view_space->x,view_space->y,view_space->x+view_space->w,view_space->y+view_space->h) )
                {
                    if (gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,fieldElementBox.x+view_space->x-cam->x,fieldElementBox.y+view_space->y-cam->y,fieldElementBox.x+fieldElementBox.w+view_space->x-cam->x,fieldElementBox.y+fieldElementBox.h+view_space->y-cam->y) )
                    {

                        isHovered = true;
                        if( (int)descriptionText.size()>0 )
                        {
                            main_overlay_system->update_tooltip(descriptionText);
                        }
                        else
                        {
                            main_overlay_system->update_tooltip(widget_name);
                        }
                        if( gpe::input->check_mouse_released( mb_left))
                        {
                            isClicked = true;
                            hasArrowkeyControl = true;
                            hasScrollControl = false;
                        }
                    }
                    else
                    {
                        if( showLabel )
                        {
                            if (gpe::point_between(gpe::input->mouse_position_x,gpe::input->mouse_position_y,widget_box.x+view_space->x-cam->x,fieldElementBox.y+view_space->y-cam->y,fieldElementBox.x+view_space->x-cam->x,fieldElementBox.y+widget_box.h+view_space->y-cam->y) )
                            {
                                if( (int)inputLabel.size() > 0 )
                                {
                                    main_overlay_system->update_tooltip( inputLabel);
                                }
                                else if( (int)descriptionText.size()>0 )
                                {
                                    main_overlay_system->update_tooltip(descriptionText);
                                }
                                else
                                {
                                    main_overlay_system->update_tooltip(widget_name);
                                }
                            }
                        }

                        if( gpe::input->check_mouse_released( mb_anybutton ) )
                        {
                            clickedOutside = true;
                            hasScrollControl = false;
                            hasArrowkeyControl = false;
                        }
                    }
                }
                else if( gpe::input->check_mouse_released( mb_anybutton ) )
                {
                    clickedOutside = true;
                    hasScrollControl = false;
                    hasArrowkeyControl = false;
                }
            }
        }

        //
        if( isClicked)
        {
            isInUse = true;
            inputFieldPos = 0;
            gpe::input->inputted_keys = "";
            get_cursor_from_mouse(view_space,cam);
        }
        if( isHovered)
        {
            gpe::cursor_main_controller->cursor_change("ibeam");
        }
        if(clickedOutside)
        {
            isInUse = false;
            reset_selection();
        }
        if( isHovered)
        {
            if( gpe::input->check_mouse_double_clicked( mb_left) )
            {
                if( lastfloatClickAction ==0)
                {
                    get_cursor_from_mouse(view_space,cam);
                    selectionCursorPos = cursorPos;
                    selectionEndCursorPos = cursorPos;
                    int iPrev = 0;
                    int jNext = 0;
                    if( cursorPos>=0 &&  cursorPos <= (int)textInputstring.size() )
                    {
                        if( textInputstring[cursorPos]==' ')
                        {
                            for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                            {
                                if( textInputstring[iPrev]==' ')
                                {
                                    selectionCursorPos--;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            for(jNext = cursorPos; jNext < (int)textInputstring.size(); jNext++ )
                            {
                                if( textInputstring[jNext]==' ')
                                {
                                    selectionEndCursorPos++;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        else if( stg_ex::char_is_alnum( textInputstring[cursorPos],false,false) )
                        {
                            for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                            {
                                if( stg_ex::char_is_alnum( textInputstring[iPrev],false,false) )
                                {
                                    selectionCursorPos--;
                                }
                                else
                                {
                                    break;
                                }
                            }

                            for(jNext = cursorPos; jNext < (int)textInputstring.size(); jNext++ )
                            {
                                if( stg_ex::char_is_alnum( textInputstring[jNext],false,false) )
                                {
                                    selectionEndCursorPos++;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        else
                        {
                            //for symbols
                            for(iPrev = cursorPos-1; iPrev >=0; iPrev-- )
                            {
                                if( stg_ex::char_is_alnum( textInputstring[iPrev],false,false)==false)
                                {
                                    selectionCursorPos--;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            for(jNext = cursorPos; jNext < (int)textInputstring.size(); jNext++ )
                            {
                                if( stg_ex::char_is_alnum( textInputstring[jNext],false,false)==false)
                                {
                                    selectionEndCursorPos++;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                    lastfloatClickAction = 1;
                }
                else
                {
                    select_all();
                    lastfloatClickAction = 0;
                }
                gpe::input->reset_all_input();

            }
            else if( gpe::input->check_mouse_pressed(0)   )
            {
                get_cursor_from_mouse(view_space,cam);
                selectionCursorPos = cursorPos;
                selectionEndCursorPos = cursorPos;
            }
            else if( gpe::input->check_mouse_down( mb_left )  )
            {
                get_cursor_from_mouse(view_space,cam);
                selectionEndCursorPos = cursorPos;
            }
            else if( gpe::input->check_mouse_down( mb_right ))
            {
                context_menu_open(-1,-1,128);
                main_context_menu->add_menu_option("Undo",0,rsm_gui->texture_add("back_buttonIcon", gpe::app_directory_name+"resources/buttons/backward.png"),-1,nullptr,true,true);
                main_context_menu->add_menu_option("Cut",1,rsm_gui->texture_add("cut_buttonIcon", gpe::app_directory_name+"resources/buttons/cut.png"),-1,nullptr,false,true);
                main_context_menu->add_menu_option("Copy",2,rsm_gui->texture_add("copy_buttonIcon", gpe::app_directory_name+"resources/buttons/copy.png"),-1,nullptr,false,true);
                main_context_menu->add_menu_option("Paste",3,rsm_gui->texture_add("paste_buttonIcon", gpe::app_directory_name+"resources/buttons/paste.png"),-1,nullptr,false,true);
                main_context_menu->add_menu_option("Delete",4,rsm_gui->texture_add("remove_buttonIcon", gpe::app_directory_name+"resources/buttons/remove.png"),-1,nullptr,true,true);
                main_context_menu->add_menu_option("Select All",5,rsm_gui->texture_add("sticky_buttonIcon", gpe::app_directory_name+"resources/buttons/sticky-note.png"),-1,nullptr,true,true);
                int menuSelection = context_menu_process();
                if( menuSelection==5)
                {
                    select_all();
                }
                else
                {
                    switch(menuSelection)
                    {
                    case 1:
                        cut_selection();
                        break;
                    case 2:
                        copy_selection();
                        break;
                    case 3:
                        pasteCommandGiven = true;
                        break;
                    case 4:
                        delete_selection();
                        break;
                    default:
                        break;
                    }
                    reset_selection();
                }
            }
            /*
            else if( resource_dragged!=nullptr )
            {
                if( gpe::input->check_mouse_released( mb_left))
                {
                    gpe::cursor_main_controller->cursor_change(GPE_CURSOR_IBEAM);
                    get_cursor_from_mouse(view_space, cam);
                    if( cursorPos >=0 && cursorPos <= (int)textInputstring.size() )
                    {
                        set_string( stg_ex::get_substring(textInputstring,0,cursorPos)+resource_dragged->get_name()+stg_ex::get_substring(textInputstring,cursorPos) );
                        resource_dragged = nullptr;
                    }
                }
            }*/
        }

        //limit_space_to_rect(view_space,&INP_mouse_position_x,&INP_mouse_position_y);
        if( isEnabled &&  isInUse )
        {
            cursorTimer+=gpe::time_keeper->get_delta_performance();
            if( cursorTimer > main_settings->cursorBlinkTime )
            {
                showCursor = !showCursor;
                cursorTimer = 0;
            }
            //special control action happening
            //Process Input Field delay timers
            if( gpe::input->kb_button_down[kb_backspace]  )
            {
                bscDelay+=gpe::time_keeper->get_delta_performance();
            }
            else
            {
                bscDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_delete] )
            {
                delDelay+=gpe::time_keeper->get_delta_performance();
            }
            else
            {
                delDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_up] )
            {
                upArrowDelay+=gpe::time_keeper->get_delta_performance();
            }
            else
            {
                upArrowDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_down] )
            {
                downArrowDelay+=gpe::time_keeper->get_delta_performance();
            }
            else
            {
                downArrowDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_left]  )
            {
                leftArrowDelay+=gpe::time_keeper->get_delta_performance();
            }
            else
            {
                leftArrowDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_right] )
            {
                rightArrowDelay+=gpe::time_keeper->get_delta_performance();
            }
            else
            {
                rightArrowDelay = 0;
            }
            if( gpe::input->kb_button_down[kb_enter] )
            {
                enterDelay+=gpe::time_keeper->get_delta_performance();
            }
            else
            {
                enterDelay = 0;
            }

            if( gpe::input->kb_button_down[kb_ctrl]  )
            {
                if( gpe::input->kb_button_down[kb_a])
                {
                    select_all();
                    showCursor = true;
                    cursorTimer = 0;
                }
                else if( gpe::input->kb_button_down[kb_c])
                {
                    copy_selection();
                    showCursor = true;
                    cursorTimer = 0;
                }
                else if( gpe::input->kb_button_released[kb_v])
                {
                    pasteCommandGiven = true;
                    showCursor = true;
                    cursorTimer = 0;
                }
                else if( gpe::input->kb_button_down[kb_x])
                {
                    cut_selection();
                    showCursor = true;
                    cursorTimer = 0;
                }
                else if( gpe::input->mouse_scrolling_up )
                {
                    move_left(4);
                    showCursor = true;
                    cursorTimer = 0;
                }
                else if( gpe::input->mouse_scrolling_down )
                {
                    move_right(4);
                    showCursor = true;
                    cursorTimer = 0;
                }
                else if( leftArrowDelay >= main_settings->textInputDelayTime )
                {
                    leftArrowDelay = 0;
                    if( gpe::input->kb_shift_pressed)
                    {
                        if( selectionCursorPos==selectionEndCursorPos )
                        {
                            selectionCursorPos = cursorPos;
                        }
                    }
                    if( cursorPos >=0 && cursorPos <= (int)textInputstring.size() )
                    {
                        if( cursorPos >=(int)textInputstring.size() )
                        {
                            cursorPos = (int)textInputstring.size()-1;
                        }
                        if( cursorPos < 0)
                        {
                            cursorPos = 0;
                        }
                        else if( cursorPos < (int)textInputstring.size() )
                        {
                            int iNCursorX = cursorPos-1;
                            for( iNCursorX = cursorPos-1; iNCursorX >=0; iNCursorX--)
                            {
                                if( stg_ex::char_is_alnum(textInputstring[iNCursorX],false,false)==false )
                                {
                                    break;
                                }
                            }
                            if( iNCursorX< 0)
                            {
                                iNCursorX = 0;
                            }
                            cursorPos = iNCursorX;
                        }
                        else
                        {
                            cursorPos = 0;
                        }
                    }
                    //scroll_to_cursor();
                    showCursor = true;
                    cursorTimer = 0;
                    if( gpe::input->kb_shift_pressed)
                    {
                        selectionEndCursorPos = cursorPos;
                    }
                    leftArrowDelay = 0;
                }
                else if( rightArrowDelay >= main_settings->textInputDelayTime )
                {
                    rightArrowDelay = 0;
                    if( gpe::input->kb_shift_pressed)
                    {
                        if( selectionCursorPos==selectionEndCursorPos )
                        {
                            selectionCursorPos = cursorPos;
                        }
                    }
                    if( cursorPos >=0 && cursorPos < (int)textInputstring.size() )
                    {
                        if( cursorPos >=(int)textInputstring.size() )
                        {
                            cursorPos = (int)textInputstring.size()-1;
                        }
                        if( cursorPos < 0)
                        {
                            cursorPos = 0;
                        }
                        else if( cursorPos < (int)textInputstring.size()-1 )
                        {
                            int iNCursorX = cursorPos-1;
                            for( iNCursorX = cursorPos+1; iNCursorX < (int)textInputstring.size(); iNCursorX++)
                            {
                                if( stg_ex::char_is_alnum(textInputstring[iNCursorX],false,false)==false )
                                {
                                    break;
                                }
                            }
                            if( iNCursorX< 0)
                            {
                                iNCursorX = 0;
                            }
                            cursorPos = iNCursorX;
                        }
                        else
                        {
                            cursorPos = 0;
                        }
                    }
                    //scroll_to_cursor();
                    showCursor = true;
                    cursorTimer = 0;
                    if( gpe::input->kb_shift_pressed)
                    {
                        selectionEndCursorPos = cursorPos;
                    }
                    rightArrowDelay = 0;
                }
            }
            else if( gpe::input->mouse_scrolling_up )
            {
                move_left(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( gpe::input->mouse_scrolling_down )
            {
                move_right(4);
                showCursor = true;
                cursorTimer = 0;
            }
            else if( gpe::input->kb_button_down[kb_alt])
            {
                //special alt action
                showCursor = true;
                cursorTimer = 0;
            }
            else if( bscDelay >= main_settings->textInputDelayTime )
            {
                bscDelay = 0;
                if( selectionCursorPos!=selectionEndCursorPos)
                {
                    delete_selection();
                }
                else
                {
                    int prevSize = (int)textInputstring.size();
                    if( prevSize>0  && cursorPos>0)
                    {
                        cursorPos-=1;
                        std::string lineToEdit = textInputstring+"";
                        textInputstring = stg_ex::get_substring(lineToEdit,0,cursorPos );
                        std::string rightSide = stg_ex::get_substring(lineToEdit,cursorPos+1);
                        textInputstring+=rightSide;
                        startXPos-=1;
                        if( startXPos < 0)
                        {
                            startXPos=0;
                        }
                    }
                }
                showCursor = true;
                cursorTimer = 0;
                bscDelay = 0;
                //gpe::input->reset_all_input();
            }
            else if( delDelay >= main_settings->textInputDelayTime   )
            {
                delDelay = 0;
                if( selectionCursorPos!=selectionEndCursorPos)
                {
                    delete_selection();
                }
                else
                {
                    int prevSize = (int)textInputstring.size();
                    if( prevSize>0  && cursorPos>0)
                    {
                        std::string lineToEdit = textInputstring+"";
                        textInputstring = stg_ex::get_substring(lineToEdit,0,cursorPos );
                        std::string rightSide = stg_ex::get_substring(lineToEdit,cursorPos+1);
                        textInputstring+=rightSide;
                        startXPos-=1;
                        if( startXPos < 0)
                        {
                            startXPos=0;
                        }
                    }
                    delDelay = 0;
                    gpe::input->kb_button_released[kb_delete] = false;
                }
                showCursor = true;
                cursorTimer = 0;
            }
            else if( leftArrowDelay >= main_settings->textInputDelayTime  )
            {
                reset_selection();
                if( cursorPos > 0)
                {
                    cursorPos-=1;
                    if( cursorPos <= startXPos)
                    {
                        move_left(1);
                        showCursor = true;
                        cursorTimer = 0;
                    }
                }
                showCursor = true;
                cursorTimer = 0;
                leftArrowDelay = 0;
            }
            else if( rightArrowDelay >= main_settings->textInputDelayTime  )
            {
                reset_selection(1);
                if( cursorPos < (int)textInputstring.size() )
                {
                    cursorPos+=1;
                    if( cursorPos >= startXPos+maxCharactersInView)
                    {
                        move_right(1);
                        showCursor = true;
                        cursorTimer = 0;
                    }
                }
                showCursor = true;
                cursorTimer = 0;
                rightArrowDelay = 0;
            }
            else if( upArrowDelay >= main_settings->textInputDelayTime  )
            {
                cursorHistory_pos--;
                cursorPos = 0;
                if( cursorHistory_pos >=0 && (int)listOfPaststrings.size() > 0 )
                {
                    record_string(textInputstring);
                    set_string(listOfPaststrings[cursorHistory_pos]);
                    select_all();
                    showCursor = true;
                    cursorTimer = 0;
                }
                showCursor = true;
                cursorTimer = 0;
                upArrowDelay = 0;
            }
            else if( downArrowDelay >= main_settings->textInputDelayTime  )
            {
                if( (int)listOfPaststrings.size() >0 && cursorHistory_pos>=-1 && cursorHistory_pos < (int)listOfPaststrings.size() -1 )
                {
                    cursorHistory_pos++;
                    cursorPos = 0;
                    record_string(textInputstring);
                    set_string(listOfPaststrings[cursorHistory_pos]);
                    select_all();
                }
                else
                {
                    gpe::error_log->report("Unable to navigate from ["+ stg_ex::int_to_string(cursorHistory_pos)+" with ["+ stg_ex::int_to_string( (int)listOfPaststrings.size() )+"] past strings....");
                }
                showCursor = true;
                cursorTimer = 0;
                downArrowDelay = 0;
            }
            else if( gpe::input->kb_button_pressed[kb_enter] )
            {
                isInUse = resubmissionAllowed;
                inputSubmitted = true;
                showCursor = true;
                cursorTimer = 0;
                enterDelay = 0;
                //gpe::error_log->report("Submitting self data...");
            }
            else
            {
                int morestringSize = (int)gpe::input->inputted_keys.size();
                if( morestringSize > 0)
                {
                    delete_selection();
                    textInputstring=stg_ex::get_substring(textInputstring,0,cursorPos)+gpe::input->inputted_keys+stg_ex::get_substring(textInputstring,cursorPos);
                    cursorPos+=(int)gpe::input->inputted_keys.size();
                    if( cursorPos>startXPos+maxCharactersInView)
                    {
                        startXPos+=morestringSize;
                    }
                    showCursor = true;
                    cursorTimer = 0;
                }
                gpe::input->inputted_keys = "";
            }

            if( pasteCommandGiven && gpe::input->clipboard_empty() == false )
            {
                delete_selection();
                std::string clipboardStr = gpe::input->clipboard_string();
                clipboardStr = stg_ex::string_replace_all(clipboardStr,"\r\n","\n");
                clipboardStr = stg_ex::string_replace_all(clipboardStr,"\r","\n");
                clipboardStr = stg_ex::string_replace_all(clipboardStr,"\t","    ");
                std::string leftBeforePaste = stg_ex::get_substring(textInputstring,0,cursorPos);
                std::string rightBeforePaste = "";
                if( cursorPos < (int)textInputstring.size() )
                {
                    rightBeforePaste = stg_ex::get_substring(textInputstring,cursorPos+1);
                }
                textInputstring=leftBeforePaste+clipboardStr+rightBeforePaste;
                cursorPos+=(int)clipboardStr.size();
                move_right( (int)clipboardStr.size() );
                showCursor = true;
                cursorTimer = 0;
            }
        }
        else
        {
            bscDelay = 0;
            delDelay = 0;
            enterDelay = 0;
            upArrowDelay = 0;
            downArrowDelay = 0;
            leftArrowDelay = 0;
            rightArrowDelay = 0;
        }
    }

    void widget_input_text::record_string( std::string strToRecord)
    {
        if( (int)strToRecord.size() > 0)
        {
            int recordListSize = (int)listOfPaststrings.size();
            if( recordListSize > 0)
            {
                bool duplicateStr = false;
                for( int i = 0; i < recordListSize; i++)
                {
                    if( strToRecord ==listOfPaststrings[i])
                    {
                        duplicateStr = true;
                    }
                }
                if( !duplicateStr )
                {
                    listOfPaststrings.insert( listOfPaststrings.begin(),strToRecord);
                    /*if( recordListSize > MAX_STORED_TEXT_PREVIOUS_ENTRIES)
                    {
                        listOfPaststrings.erase(listOfPaststrings.begin() );
                    }*/

                }
            }
            else
            {
                listOfPaststrings.push_back( strToRecord);
            }
        }
    }

    void widget_input_text::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
    {
        view_space = gpe::camera_find(view_space);
        cam = gpe::camera_find(cam);
        if( !isEnabled || cam == nullptr)
        {
            return;
        }

        if( showLabel && (int)inputLabel.size() > 0)
        {
            if( alignment_h == gpe::fa_center)
            {
                gpe::gfs->render_text( widget_box.x+widget_box.w/4-cam->x,widget_box.y-cam->y + widget_box.h/2,inputLabel,pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_center,gpe::fa_middle);
            }
            else if( alignment_h == gpe::fa_right )
            {
                gpe::gfs->render_text( widget_box.x+widget_box.w/2 -cam->x,widget_box.y-cam->y  + widget_box.h/2,inputLabel,pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_right,gpe::fa_middle);
            }
            else
            {
                gpe::gfs->render_text( widget_box.x-cam->x,widget_box.y-cam->y + widget_box.h/2,inputLabel,pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_left,gpe::fa_middle);
            }
        }

        if(showBorderBox)
        {
            if( hasValidInput )
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,pawgui::theme_main->input_color,false, 255 );
            }
            else
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,pawgui::theme_main->input_error_Box_color,false, 255 );
            }
        }



        if(selectionCursorPos!=selectionEndCursorPos )
        {
            int minHighlightPos = std::min(selectionCursorPos, selectionEndCursorPos);
            if( minHighlightPos < startXPos)
            {
                minHighlightPos = startXPos;
            }
            int maxHighlightPos = std::max(selectionCursorPos, selectionEndCursorPos);
            if( maxHighlightPos < startXPos)
            {
                maxHighlightPos = startXPos;
            }
            maxHighlightPos = std::min(startXPos+maxCharactersInView,maxHighlightPos);

            gpe::gcanvas->render_rectangle(
                fieldElementBox.x+(minHighlightPos-startXPos)*font_sizeW - cam->x,
                fieldElementBox.y+1 - cam->y,
                fieldElementBox.x+2+(maxHighlightPos-startXPos)*font_sizeW- cam->x,
                fieldElementBox.y+fieldElementBox.h-1 - cam->y,
                pawgui::theme_main->input_selected_color,false, 255 );
        }
        int subLength = 0;
        if( (int)textInputstring.size() > maxCharactersInView)
        {
            subLength = (int)textInputstring.size()-startXPos;
            if( subLength >maxCharactersInView)
            {
                subLength = maxCharactersInView;
            }
        }
        else
        {
            subLength =  (int)textInputstring.size();
        }

        if( subLength < 0)
        {
            subLength = 0;
        }

        if( (int)textInputstring.size()>0)
        {
            if( inputIsPassword )
            {
                gpe::gfs->render_text( fieldElementBox.x - cam->x + inputFieldPadding,fieldElementBox.y+fieldElementBox.h/2- cam->y,stg_ex::string_repeat("*",subLength),pawgui::theme_main->input_font_color,font_textinput,gpe::fa_left,gpe::fa_middle,255);
            }
            else
            {
                gpe::gfs->render_text( fieldElementBox.x - cam->x + inputFieldPadding,fieldElementBox.y+fieldElementBox.h/2- cam->y,stg_ex::get_substring(textInputstring,startXPos,subLength),pawgui::theme_main->input_font_color,font_textinput,gpe::fa_left,gpe::fa_middle,255);
            }
        }
        else if(showPlaceHolder && (int)placeHolderstring.size() > 0 )
        {
            if( (int)placeHolderstring.size() > maxCharactersInView)
            {
                subLength = (int)placeHolderstring.size()-startXPos;
                if( subLength >maxCharactersInView)
                {
                    subLength = maxCharactersInView;
                }
            }
            else
            {
                subLength =  (int)placeHolderstring.size();
            }
            gpe::gfs->render_text( fieldElementBox.x - cam->x + inputFieldPadding,fieldElementBox.y+fieldElementBox.h/2 - cam->y,stg_ex::get_substring(placeHolderstring,0,subLength),pawgui::theme_main->input_faded_font_color,font_textinput,gpe::fa_left,gpe::fa_middle);
        }

        if(showBorderBox)
        {
            if( isInUse )
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,pawgui::theme_main->input_highlight_outline_color,true);
            }
            else if( isHovered )
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,pawgui::theme_main->input_highlight_alt_color,true);
            }
            else
            {
                gpe::gcanvas->render_rectangle( fieldElementBox.x - cam->x,fieldElementBox.y - cam->y,fieldElementBox.x+fieldElementBox.w - cam->x,fieldElementBox.y+fieldElementBox.h - cam->y,pawgui::theme_main->input_outline_color,true);
            }
        }

        if(isInUse && prevCursorPos!=cursorPos && prevCursorPos >=startXPos && prevCursorPos <= startXPos+maxCharactersInView )
        {
            //gpe::gcanvas->render_vertical_line_color( fieldElementBox.x+padding_default/2+(prevCursorPos-startXPos)*font_sizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,pawgui::theme_main->input_color);
        }

        if(isInUse && cursorPos >=startXPos && cursorPos <= startXPos+maxCharactersInView )
        {
            if(showCursor)
            {
                gpe::gcanvas->render_vertical_line_color( fieldElementBox.x+(cursorPos-startXPos)*font_sizeW- cam->x + inputFieldPadding ,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,pawgui::theme_main->input_font_color);
            }
            else
            {
                //gpe::gcanvas->render_vertical_line_color( fieldElementBox.x+padding_default/2+(cursorPos-startXPos)*font_sizeW- cam->x,fieldElementBox.y - cam->y,fieldElementBox.y+fieldElementBox.h - cam->y,pawgui::theme_main->input_color);
            }
        }
    }

    void widget_input_text::reset_selection(int resetDirection)
    {
        if( selectionCursorPos!=selectionEndCursorPos )
        {
            if( resetDirection < 0)
            {
                cursorPos = std::min(selectionCursorPos, selectionEndCursorPos);
            }
            else
            {
                cursorPos = std::max(selectionCursorPos, selectionEndCursorPos);
            }
        }
        selectionCursorPos =0;
        selectionEndCursorPos = 0;
    }

    void widget_input_text::select_all()
    {
        selectionCursorPos = 0;
        selectionEndCursorPos = textInputstring.size();
        cursorPos = 0;
        gpe::input->reset_all_input();
    }

    void widget_input_text::set_height(int new_height)
    {
        if( showLabel)
        {
            fieldElementBox.h = new_height;
        }
        else
        {
            widget_box.h = new_height;
            fieldElementBox.h = new_height;
        }
        prevwidget_boxH = new_height;
    }

    void widget_input_text::set_label(std::string newLabel)
    {
        if( (int)newLabel.size() > 0)
        {
            inputLabel = newLabel;
            showLabel = true;

            widget_box.h=prevwidget_boxH;
            //widget_box.w = std::max( prevwidget_boxW, ( font_sizeW *(int)inputLabel.size()  ) );
            fieldElementBox.x = widget_box.x + widget_box.w/2;
            fieldElementBox.y = widget_box.y;
            //fieldElementBox.w = prevwidget_boxW;
            fieldElementBox.w = widget_box.w/2;
            fieldElementBox.h = widget_box.h;
        }
        else
        {
            fieldElementBox.x = widget_box.x;
            fieldElementBox.w = widget_box.w;
            fieldElementBox.h = widget_box.h;
            inputLabel = "";
            showLabel = false;
        }
    }

    void widget_input_text::set_name(std::string new_name)
    {
        if( (int)new_name.size() > 0)
        {
            /*if( widget_name == inputLabel )
            {
                set_label(new_name);
            }*/
            widget_name = new_name;
        }
    }

    void widget_input_text::set_number( float numberIn)
    {
        std::string newstring = stg_ex::float_to_string(numberIn);
        textInputstring = newstring;
        record_string(textInputstring);
        cursorPos = (int) textInputstring.size();

    }

    void widget_input_text::set_string(std::string newstring)
    {
        if( (int)textInputstring.size() > 0)
        {
            record_string( textInputstring );
        }
        if( (int)newstring.size() > 0)
        {
            record_string( newstring );
        }
        textInputstring = newstring;
        cursorPos = (int) textInputstring.size();
    }

    void widget_input_text::set_placeholder(std::string newstring)
    {
        placeHolderstring = newstring;
    }

    void widget_input_text::set_width(int new_width)
    {
        if( showLabel)
        {
            fieldElementBox.w = new_width;
        }
        else
        {
            widget_box.w = new_width;
            fieldElementBox.w = new_width;
        }
        widget_box.w = new_width;
        fieldElementBox.w = new_width;
        prevwidget_boxW = new_width;
    }

    void widget_input_text::set_max_width( int nMW)
    {
        maxWidth = nMW;
        if( maxWidth > 0 )
        {
            if( widget_box.w > maxWidth || fieldElementBox.w > maxWidth)
            {
                fieldElementBox.w = maxWidth;
                widget_box.w = maxWidth;
                prevwidget_boxW = maxWidth;
            }
        }
    }

    void widget_input_text::set_max_height( int nMH)
    {
        maxHeight = nMH;
        if( maxHeight > 0 && widget_box.h > maxHeight)
        {
            widget_box.h = maxHeight;
        }
    }

    bool widget_input_text::was_submitted()
    {
        return inputSubmitted;
    }


    widget_input_number::widget_input_number( std::string placeHolderText, bool onlyWholeNums, float hMin, float hMax)
    {
        resubmissionAllowed = false;
        textInputstring = "";
        widget_type = "inputnumber";
        widget_type_id = 5;
        forceValidNumber = false;
        showLabel = false;
        inputLabel = "";
        if( (int)placeHolderText.size()>512)
        {
            placeHolderText = placeHolderText.substr(0,512);
        }
        placeHolderstring = placeHolderText;
        showPlaceHolder = true;
        widget_box.x = 0;
        widget_box.y = 0;
        widget_box.w = 192;
        widget_box.h = 24;

        inputFieldPos = 0;
        onlyWholeNumbers = onlyWholeNums;

        minInputNumber = hMin;
        maxInputNumber = hMax;


        font_sizeH = 12;
        font_sizeW = 12;
        font_textinput->get_metrics("A",&font_sizeW,&font_sizeH);
        if( font_sizeW<=0)
        {
            font_sizeW = 12;
        }
        maxCharactersInView = widget_box.w/font_sizeW;

        hasValidInput = true;
        heldNumber = 0;
        startXPos = 0;
        cursorPos = 0;
        textInputstring = "0";
        set_label( placeHolderText );
    }

    widget_input_number::~widget_input_number()
    {

    }

    int widget_input_number::get_held_int()
    {
        return (int)get_held_number();
    }

    float widget_input_number::get_held_number()
    {
        heldNumber = stg_ex::string_to_float(textInputstring, INT_MIN);
        return heldNumber;
    }

    std::string widget_input_number::get_plain_string()
    {
        heldNumber = stg_ex::string_to_float(textInputstring, INT_MIN);
        return stg_ex::float_to_string(heldNumber);
    }

    float widget_input_number::make_valid_number( float fallbackNumber )
    {
        heldNumber = stg_ex::string_to_float(textInputstring, INT_MIN);
        if( heldNumber== INT_MIN || heldNumber < minInputNumber || heldNumber > maxInputNumber)
        {
            heldNumber = fallbackNumber;
            hasValidInput = true;
            set_string(""+ stg_ex::float_to_string( fallbackNumber ) );
        }
        return heldNumber;
    }

    void widget_input_number::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
    {
        hasValidInput = true;
        widget_input_text::process_self(view_space,cam);
        if( (int)textInputstring.size() > 0)
        {
            if( onlyWholeNumbers)
            {
                heldNumber = stg_ex::string_to_int(textInputstring, INT_MIN);
                if( heldNumber== INT_MIN || heldNumber < minInputNumber || heldNumber > maxInputNumber)
                {
                    heldNumber = 0;
                    if( forceValidNumber)
                    {
                        set_string("0");
                    }
                    hasValidInput = false;
                }
            }
            else
            {
                heldNumber = stg_ex::string_to_float(textInputstring, INT_MIN);
                if( heldNumber == INT_MIN || heldNumber < minInputNumber || heldNumber > maxInputNumber )
                {
                    heldNumber = 0;
                    if( forceValidNumber)
                    {
                        set_string("0");
                    }
                    hasValidInput = false;
                }
            }
        }
        else
        {
            heldNumber = 0;
            if( forceValidNumber)
            {
                set_string("0");
            }
            hasValidInput = false;
        }
    }
}
