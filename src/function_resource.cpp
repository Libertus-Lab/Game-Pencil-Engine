/*
functionResource.cpp
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

#include "function_resource.h"

functionResource::functionResource(GPE_GeneralResourceContainer * pFolder)
{
    projectParentFolder = pFolder;

    /*cancelResourceButton->disable_self();
    confirmResourceButton->disable_self();
    loadResourceButton->disable_self();
    saveResourceButton->disable_self();
    */
    //textEditorButtonBar->set_width(256);
    functionReturnType = new GPE_TextInputBasic("void","");
    parametersField = new GPE_TextInputBasic("","parameter1, parameter2, etc");
    parametersField->set_label("Function Parameters");
    functionCode = new GPE_TextAreaInputBasic(false);
    renameBox->set_coords(GENERAL_GPE_PADDING,-1 );
    saveButton = new GPE_ToolIconButton( APP_DIRECTORY_NAME+"resources/gfx/iconpacks/fontawesome/save.png","Save Changes",-1,24);
    saveButton->set_height(parametersField->get_height() );
    saveButton->set_width(parametersField->get_height() );
    functionEditorList = new GPE_GuiElementList();
    //loadResourceButton->disable_self();
    //saveResourceButton->disable_self();
}

functionResource::~functionResource()
{
    if(functionEditorList!=NULL)
    {
        delete functionEditorList;
        functionEditorList = NULL;
    }

    if(functionCode!=NULL)
    {
        delete functionCode;
        functionCode = NULL;
    }

    if(functionReturnType!=NULL)
    {
        delete functionReturnType;
        functionReturnType = NULL;
    }

    if(parametersField!=NULL)
    {
        delete parametersField;
        parametersField = NULL;
    }

    if(renameBox!=NULL)
    {
        delete renameBox;
        renameBox = NULL;
    }
    if( saveButton!=NULL)
    {
        delete saveButton;
        saveButton = NULL;
    }

}

bool functionResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    return true;
}

bool functionResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void functionResource::compile_cpp()
{

}

void functionResource::integrate_into_syntax()
{
    if( CURRENT_PROJECT!=NULL)
    {
        std::string fReturnType = "unknown_void";
        if( functionReturnType!=NULL)
        {
            if( functionReturnType->get_string().size() > 0)
            {
                fReturnType = functionReturnType->get_string();
            }
        }
        std::string functionDescription = "User defined function...";
        if( functionCode!=NULL)
        {
            std::string firstLineOfCode = functionCode->get_line_string(0);
            if( (int)firstLineOfCode.size() > 2)
            {
                if( firstLineOfCode[0]=='/' && (firstLineOfCode.at(1)=='/' || firstLineOfCode.at(1)=='*' ) )
                {
                    functionDescription = get_substring(firstLineOfCode,2);
                }
            }
        }
        if( parametersField !=NULL )
        {
            CURRENT_PROJECT->add_project_function(resourceName,functionDescription,parametersField->get_string(),fReturnType,"User Defined Global Function");
        }
        else
        {
            CURRENT_PROJECT->add_project_function(resourceName,functionDescription,"",fReturnType,"User Defined Global Function");
        }
    }
}

void functionResource::open_code( int lineNumb, int colNumb,std::string codeTitle)
{
    if( functionCode!=NULL && functionCode->has_content() )
    {
        functionCode->scroll_to_pos(lineNumb, colNumb);
    }
}

void functionResource::preprocess_self(std::string alternatePath)
{
    if( resourcePostProcessed ==false  || file_exists(alternatePath) )
    {
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_submessages( "Processing Function", resourceName );
        }

        bool usingAltSaveSource = false;
        std::string newFileIn ="";
        std::string soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/functions/";
        if( file_exists(alternatePath) )
        {
            newFileIn = alternatePath;
            soughtDir = get_path_from_file(newFileIn);
            usingAltSaveSource = true;
        }
        else
        {
            newFileIn = soughtDir + resourceName+".gpf";
        }


        std::string  functionCodeLoadLocation = soughtDir+resourceName+".cps";
        if( functionCode!=NULL)
        {
            functionCode->import_text(functionCodeLoadLocation);
            functionCode->activate_self();
            functionCode->init_save_history();
        }

        std::string otherColContainerName = "";

        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        GPE_Report("Loading function - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
        {
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string section="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                double foundFileVersion = 0;
                int fCursorX = 0;
                int fCursorY = 0;
                while ( gameResourceFileIn.good() )
                {
                    getline (gameResourceFileIn,currLine); //gets the next line of the file
                    currLineToBeProcessed = trim_left_inplace(currLine);
                    currLineToBeProcessed = trim_right_inplace(currLineToBeProcessed);

                    if( foundFileVersion <=0)
                    {
                        //Empty Line skipping is only allowed at the top of the file
                        if(!currLineToBeProcessed.empty() )
                        {
                            //Comment skipping is only allowed at the top of the file
                            if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                            {
                                //searches for an equal character and parses through the variable
                                equalPos=currLineToBeProcessed.find_first_of("=");
                                if(equalPos!=(int)std::string::npos)
                                {
                                    //if the equalPos is present, then parse on through and carryon
                                    keyString = currLineToBeProcessed.substr(0,equalPos);
                                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                    if( keyString=="Version")
                                    {
                                        foundFileVersion = string_to_double(valString);
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
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( keyString=="ResourceName")
                                {
                                    renameBox->set_string(valString);
                                }
                                else if( keyString=="Parameters")
                                {
                                    parametersField->set_string(valString);
                                }
                                if( keyString=="ReturnType")
                                {
                                    functionReturnType->set_string(valString);
                                }
                                else if( keyString=="Cursor")
                                {
                                    fCursorY = split_first_int(valString,',');
                                    fCursorX = string_to_int(valString,0);
                                    functionCode->set_ycursor(fCursorY);
                                    functionCode->set_xcursor(fCursorX);
                                }
                            }
                        }
                    }
                    else
                    {
                        GPE_Report("Invalid FoundFileVersion ="+double_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
    }
}

void functionResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
}

void functionResource::process_self(GPE_Rect * viewedSpace, GPE_Rect * cam)
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && saveButton!=NULL && functionEditorList!=NULL && parametersField!=NULL && functionCode!=NULL )
    {
        functionEditorList->set_coords( 0,0 );
        functionEditorList->set_width(viewedSpace->w );
        functionEditorList->set_height(viewedSpace->h);
        functionEditorList->clear_list();
        if( PANEL_GENERAL_EDITOR!=NULL )
        {
            functionEditorList->barXMargin = functionEditorList->barYMargin = 0;
            functionEditorList->barXPadding = functionEditorList->barYPadding = 0;
            functionCode->set_width(viewedSpace->w );
            functionCode->set_height(viewedSpace->h );

            //Process function parameter elements in the Editor Panel
            PANEL_GENERAL_EDITOR->add_gui_element(renameBox,true);
            PANEL_GENERAL_EDITOR->add_gui_element(parametersField,true);
            PANEL_GENERAL_EDITOR->add_gui_element(confirmResourceButton,true);
            PANEL_GENERAL_EDITOR->add_gui_element(cancelResourceButton,true);
            PANEL_GENERAL_EDITOR->process_self();

            functionEditorList->add_gui_element_fullsize( functionCode );
            functionEditorList->process_self( viewedSpace,cam );
            if( confirmResourceButton->is_clicked() )
            {
                save_resource();
            }
            else if( cancelResourceButton->is_clicked() )
            {
                if( GPE_Display_Basic_Prompt("Are you sure you will like to reverse code changes?","This will load in data from save-file!", true )== DISPLAY_QUERY_YES )
                {
                    resourcePostProcessed = false;
                    preprocess_self();
                }
            }
        }
        else
        {
            functionEditorList->barXMargin = functionEditorList->barYMargin = GENERAL_GPE_PADDING;
            functionEditorList->barXPadding = functionEditorList->barYPadding = GENERAL_GPE_PADDING;
            functionEditorList->set_coords( 0,0 );
            functionCode->set_width(viewedSpace->w-GENERAL_GPE_PADDING*4 );
            functionCode->set_height(viewedSpace->h - GENERAL_GPE_PADDING*4 - 32 );
            functionEditorList->add_gui_element(saveButton,false);
            functionEditorList->add_gui_element(renameBox,false);
            functionEditorList->add_gui_element(parametersField,true);
            functionEditorList->add_gui_element(functionCode,true);
            functionEditorList->process_self( viewedSpace,cam );
            if( saveButton->is_clicked() )
            {
                save_resource();
            }
        }

        // parametersField->set_height(renameBox->get_height() );
    }
}

void functionResource::render_self(GPE_Rect * viewedSpace, GPE_Rect * cam,bool forceRedraw )
{
    viewedSpace = GPE_find_camera(viewedSpace);
    cam = GPE_find_camera(cam);
    if(cam!=NULL && viewedSpace!=NULL && functionEditorList!=NULL )
    {
        functionEditorList->render_self( viewedSpace, cam, forceRedraw );
    }
}

void functionResource::save_resource(std::string alternatePath, int backupId)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Processing Function", resourceName );
    }
    appendToFile(alternatePath,"blank");
    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = get_path_from_file(alternatePath);
    if( path_exists(soughtDir) )
    {
        newFileOut = alternatePath;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = fileToDir(parentProjectName)+"/gpe_project/resources/functions/";
        newFileOut = soughtDir + resourceName+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);

            if( functionCode!=NULL)
            {
                //    *fileTarget << functionCode->get_xcursor() << "," << functionCode->get_ycursor() << ",";
                std::string  functionCodeSaveLocation = soughtDir+resourceName+".cps";
                if( usingAltSaveSource)
                {
                    if( file_exists(functionCodeSaveLocation) )
                    {
                        /*
                        if( GPE_Display_Basic_Prompt("[WARNING]Function File Already exists?","Are you sure you will like to overwrite your ["+resourceName+".cps] Function file? This action is irreversible!")==DISPLAY_QUERY_YES)
                        {
                            functionCode->export_text(functionCodeSaveLocation );
                        }
                        */
                    }
                    else
                    {
                        functionCode->export_text(functionCodeSaveLocation );
                    }
                }
                else
                {
                    functionCode->export_text(functionCodeSaveLocation );
                }
                newSaveDataFile << "Cursor=" << functionCode->get_ycursor() << "," << functionCode->get_xcursor() << "\n";
            }
            else
            {
                //*fileTarget << "0,0,";
            }

            std::string paramStrSize = parametersField->get_string();
            if( paramStrSize.size() > 0)
            {
                newSaveDataFile << "Parameters=" << parametersField->get_string() << "\n";
            }
            else
            {
                //*fileTarget << " ||,";
            }
            newSaveDataFile << "ReturnType=" << functionReturnType->get_string() << "\n";
            newSaveDataFile.close();
            if( !usingAltSaveSource)
            {
                isModified = false;
            }
        }
        else
        {
            GPE_Main_Logs->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        GPE_Main_Logs->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

int functionResource::search_for_string(std::string needle)
{
    int foundStrings = 0;
    GPE_Main_Logs->log_general_comment("Searching ["+resourceName+"] function..");
    if( functionCode!=NULL && GPE_MAIN_GUI!=NULL && functionCode->has_content() )
    {
        GPE_MAIN_GUI->searchResultProjectName = parentProjectName;
        GPE_MAIN_GUI->searchResultResourceId = globalResouceIdNumber;
        GPE_MAIN_GUI->searchResultResourceName = resourceName;
        foundStrings=functionCode->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked(),true);
    }
    return foundStrings;
}

int functionResource::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundStrings = 0;
    if( functionCode!=NULL && MAIN_SEARCH_CONTROLLER!=NULL && functionCode->has_content() )
    {
        foundStrings=functionCode->find_all_strings(needle,MAIN_SEARCH_CONTROLLER->findMatchCase->is_clicked() );
        if( GPE_LOADER != NULL )
        {
            GPE_LOADER->update_messages( "Replacing Substring", needle, "with ["+newStr+"]" );
        }

        if( foundStrings > 0)
        {
            int replaceCount =  functionCode->replace_all_found(needle, newStr );
            GPE_LOADER->update_messages( "Replaced Substring", needle, int_to_string(replaceCount) +" times" );
            MAIN_SEARCH_CONTROLLER->showFindAllResults = true;
        }
        else
        {
            GPE_LOADER->update_messages( "Replacing Substring", needle, "No matches found");
            MAIN_SEARCH_CONTROLLER->showFindAllResults = false;
        }
        //MAIN_OVERLAY->update_temporary_message(displayMessageTitle,displayMessageSubtitle,displayMessageString,1);
    }
    return foundStrings;
}

bool functionResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( GPE_LOADER != NULL )
    {
        GPE_LOADER->update_submessages( "Saving Function", resourceName );
    }
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Function=" << resourceName << "," << get_global_rid() << ",\n";
            save_resource();
            return true;
        }
    }
    return false;
}
