/*
paw_gui_base.h
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

#ifndef PAW_GUI_BASE_H
#define PAW_GUI_BASE_H

#include "paw_gui_themes.h"
#include "paw_gui_overlay.h"

const int PGUI_MAJOR_VERSION = 1;
const int PGUI_MINOR_VERSION = 1;
const int PGUI_PATCHLEVEL = 2;

const int nestedFolderLimit = 4;
const int SEARCH_MODE_FIND = 1;
const int SEARCH_MODE_GOTO = 2;
const int SEARCH_MODE_REPLACE = 3;
const int SEARCH_MODE_FINDALL = 4;
const int SEARCH_MODE_REPLACEALL = 5;

const int DISPLAY_QUERY_YES = 1;
const int DISPLAY_QUERY_NO = 0;
const int DISPLAY_QUERY_CANCEL = 2;

extern int POPUP_FONT_SIZE_WIDTH;
extern int POPUP_FONT_SIZE_HEIGHT;
extern int POPUP_MENU_VALUE;

extern std::string CURRENT_PROJECT_NAME;
extern std::string GPE_Action_Message;
extern int GPE_Action_ID;
extern bool GPE_ACTION_IS_CONTEXT;

extern GPE_Texture * GPE_CHECKMARK_IMAGE;
extern GPE_Texture  * GPE_DROPDOWN_ARROW;

class GPE_GUI_Settings
{
public:
    int defaultElementWidth;
    int defaultElementHeight;
    GPE_Rect textAreaFindBox;
    bool useShortProjectNames;
    //Text Editor Related Settings [ BEGIN ]
    double textInputDelayTime;
    double normalInputDelayTime;
    double textAreaDelayTime;
    double cursorBlinkTime;
    bool autoFindMouseTabs;
    int tabSpaceCount;

    bool showTextEditorLineCount;
    bool showTextEditorSyntaxHightlight;
    int defaultLineCountWidth;
    //Text Editor Related Settings [ END ]
    //Default Folder Related Settings [ BEGIN ]
    std::string fileOpenAudioDir;
    std::string fileOpenVideoDir;
    std::string fileOpenSpriteDir;
    std::string fileOpenTextureDir;
    std::string fileOpenTilesheetDir;
    std::string fileOpenFontDir;
    std::string fileOpenFunctionDir;
    std::string fileOpenTextFileDir;
    std::string fileOpenImportFileDir;
    std::string fileSaveExportFileDir;

    std::string fileSaveTextFileDir;
    std::string fileOpenDefaultDir;
    std::string fileSaveDefaultDir;
    //Default Folder RelatedSettings [ END ]
    //std::string
    GPE_GUI_Settings();
    ~GPE_GUI_Settings();
};

extern GPE_GUI_Settings * MAIN_GUI_SETTINGS;

extern GPE_DataManager * guiRCM;

class GPE_GeneralGuiElement
{
protected:
    GPE_Rect elementBox;
    bool clickedOutside;
    bool isClicked;
    bool isPressed;
    bool isRightClicked;
    bool isEnabled;
    bool isHovered;
    bool isModified;
    bool isInUse;
    std::string opName;
    std::string guiListTypeName;
    int guiListTypeId;
    int maxWidth, maxHeight;
    bool isFullWidth;
public:
    int hAlign, vAlign;
    int dynamicId;
    bool needsNewLine;
    int indentationLevel;
    bool autoResizes;
    int upDelayTime;
    int downDelayTime;
    int leftDelayTime;
    int rightDelayTime;
    bool hasArrowkeyControl;
    bool hasScrollControl;
    bool hasLineBreak;
    std::string descriptionText;
    bool showBorderBox;
    GPE_GeneralGuiElement();
    virtual ~GPE_GeneralGuiElement() =0;
    void disable_self();
    void enable_self();
    int get_height();
    int get_width();
    int get_max_height();
    int get_max_width();
    int get_xpos();
    int get_x2pos();
    int get_ypos();
    int get_y2pos();
    std::string get_name();
    std::string get_element_type();
    virtual std::string get_data();
    virtual std::string get_plain_string();
    virtual void load_data(std::string dataString);
    virtual void remove_data(std::string dataString);
    bool is_clicked();
    bool is_rightclicked();
    bool is_enabled();
    virtual void move_pos(int newX = 0, int newY = 0);
    bool is_full_width();
    bool is_hovered();
    void set_clicked( bool clickedVal = true);
    void set_rightclicked( bool clickedVal = true);
    virtual void set_height(int newHeight);
    void set_hovered(bool hoverVal);
    virtual void set_width(int newWidth);
    virtual void set_coords(int newX = -1, int newY = -1);
    virtual void prerender_self( );
    virtual void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect *cam = NULL);
    virtual void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    bool requires_newline();
    void scale_height(double scaleH);
    void scale_width(double scaleW);
    void scale_object(double scaleValue );
    void set_full_width( bool makeFullWidth );
    virtual void set_name(std::string newName);
    virtual void set_max_width( int nMW);
    virtual void set_max_height( int nMH);
    //Modifable properties
    bool is_inuse();
    bool is_modified();
    void switch_inuse(bool newUse);
    void update_box( double nx, double ny, double nw, double nh);
    void update_paragraph(int foundMaxWidth = -1 );
};


void GPE_change_cursor(SDL_SystemCursor id);

void update_action_message(std::string newMessage);
void update_popup_info(std::string newMessage="", int selectedOpId=-1, bool popIsContext = false);


#endif
