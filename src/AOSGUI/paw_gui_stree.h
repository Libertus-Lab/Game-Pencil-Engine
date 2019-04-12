/*
paw_gui_stree.h
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

#ifndef PAW_GUI_STREE_H
#define PAW_GUI_STREE_H

#include "paw_gui_panel_list.h"
#include "paw_gui_buttons.h"
#include "paw_gui_text_input.h"

class GPE_SpecialMenu_Branch: public GPE_GeneralGuiElement
{
protected:
    int branchType;
    int barXPadding, barYPadding;
    bool subMenuIsOpen;
    int globalId;
    bool isVisible;
    int lastWidth;
    std::string name;
    int optionSuperType;
    //GPE_Texture * textTexture;
    GPE_Texture * iconTexture;
public:
    int rightClickedId;
    int branchLevel;
    std::vector < GPE_SpecialMenu_Branch * > subElements;
    bool isLocked;
    int selectedSubOption;
    GPE_SpecialMenu_Branch * selectedBranch;
    GPE_SpecialMenu_Branch * parentBranch;
    GPE_SpecialMenu_Branch * treeParent;
    int foundX2Pos;
    int previouslySoughtId;
    bool mouseInRange;
    GPE_SpecialMenu_Branch();
    ~GPE_SpecialMenu_Branch();
    void add_to_list( GPE_GuiElementList * tList = NULL);
    void add_to_searched_list( GPE_GuiElementList * tList, std::string needleString );
    virtual void add_branch( GPE_SpecialMenu_Branch * branch, bool changeId = true  );
    virtual bool can_add_new_branch();
    GPE_SpecialMenu_Branch * get_branch_from_pos( int pos );
    GPE_SpecialMenu_Branch * get_branch_from_id( int id );
    GPE_SpecialMenu_Branch * get_first_branch();
    GPE_SpecialMenu_Branch * get_last_branch();
    int get_global_id();
    std::string get_name();
    int get_selected_id();
    int get_type();
    bool hovering_openclose( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL );
    virtual int increment_count();
    void insert_branch( GPE_SpecialMenu_Branch * branch, int pos = -1 );
    int matches(GPE_SpecialMenu_Branch * otherBranch );
    bool is_visible();
    void open_and_view();
    void process_self(GPE_Rect * viewedSpace = NULL, GPE_Rect *cam = NULL);
    bool remove_branch( int branchId );
    void remove_branch(std::string branchName );
    void remove_branch( GPE_SpecialMenu_Branch * branch );
    void remove_all_branches();
    virtual void render_branch();
    void render_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL, bool forceRedraw = true);
    virtual bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);
    void set_global_id( int newId );
    virtual void set_name( std::string newName );
};

//Trees aren't parents of branches, but for this class they are :-)
class GPE_SpecialMenu_Tree: public GPE_SpecialMenu_Branch
{
private:
    int maxSuperFolderCount;
    int seekedX2Pos;
    int barTitleWidth;
    int barTitleHeight;
    int globalBranchCounter;
    GPE_ToolIconButton * addButton;
    GPE_TextInputBasic * searchField;
    std::vector < GPE_SpecialMenu_Branch * > searchResultsElements;
public:
    bool useMetaTop;
    GPE_GuiElementList * treeList;
    bool justResized;
    bool showYScroll;
    bool beingResized;
    bool treeResized;
    bool branchMoved;
    GPE_Texture * opTexture;
    std::string needleBranch;
    GPE_SpecialMenu_Tree( std::string menuName = "",  int optionSuperType = -1, int optionId = -1 );
    ~GPE_SpecialMenu_Tree( );
    bool add_button_clicked();
    bool can_add_new_branch();
    int increment_count();
    void process_self( GPE_Rect * viewedSpace = NULL, GPE_Rect * cam = NULL );
    void render_branch();
    void render_self( GPE_Rect * viewedSpace = NULL,GPE_Rect * cam = NULL, bool forceRedraw = true);
    void toggle_self();
    bool save_branch_data(std::ofstream * fileTarget, int nestedFoldersIn = 0);
    void select_branch(GPE_SpecialMenu_Branch * branch );
};

#endif
