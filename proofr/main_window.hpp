//
//  main_window.hpp
//  proofr
//
//  Created by michael on 11/11/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

#ifndef main_window_hpp
#define main_window_hpp

#include <vector>

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Sys_Menu_Bar.H>

#include "image_box.hpp"
#include "split_box.hpp"

// =================================================================================================

// work around bug in version check

class Fl_My_Sys_Menu_Bar : public Fl_Sys_Menu_Bar
{
public:
    Fl_My_Sys_Menu_Bar(int x,int y,int w,int h,const char *l=0) :
        Fl_Sys_Menu_Bar(x, y, w, h, l)
    {
    };

    void do_update(void) { update(); };
};

// =================================================================================================

class Fl_Main_Window : public Fl_Double_Window
{
public:
    const static int MARGIN = 20;

    Fl_Main_Window(int x, int y, int width, int height, const char *title = 0);
    virtual ~Fl_Main_Window();

    virtual void resize(int x, int y, int width, int height);
    virtual void position_widgets(bool adjustImage);
    virtual void adjustSplitHeight(void);

    virtual void merged();
    virtual void split();
    virtual void new_grid();
    virtual void header_grid();
    virtual void text_grid();
    virtual void footnote_grid();
    virtual void footer_grid();
    virtual void delete_grid();
    virtual void go_page();
    virtual void prev_page();
    virtual void next_page();
    virtual void font_bigger();
    virtual void font_smaller();
    virtual void input_narrower();
    virtual void input_wider();
    virtual void insert_word_break();
    virtual void insert_emdash();
    virtual void insert_section_break();
    virtual void insert_paragraph_break();
    virtual void insert_line_break();
    virtual void save_page();
    virtual void page_input_changed();
    virtual void edited_page();

    void export_text(bool to_html);
    void undo(void);
    void cut(void);
    void copy(void);
    void paste(void);
    void delete_selection(void);
    void set_even_default_grids(void);
    void set_odd_default_grids();


    void resize(int x, int y, int width, int height, bool adjustImage);
    void finishResizing(void);

    int edit_space(void) { return 3 * input_text_size; };

    Fl_Image *getPageImage(void) { return pageImage; };
    int getCurrentPage() { return current_page; };

protected:
    bool resizing = false;

    int current_page = -3;
    virtual void load_page(int pageNumber);

    std::string src_image_path;
    std::string dest_image_path;
    std::string src_text_file_path;
    std::string dest_text_file_path;

    std::string pageText;
    int pageImageSourceWidth = 0;
    int pageImageSourceHeight = 0;
    int pageImageWidth = 0;
    int pageImageHeight = 0;
    Fl_Image *pageImage = nullptr;

    Fl_Fontsize input_text_size = 18;
    int input_text_inset = 20;

    Fl_My_Sys_Menu_Bar *menu_bar = nullptr;
    Fl_Round_Button *merged_button = nullptr;
    Fl_Round_Button *split_button = nullptr;
    Fl_Button *new_grid_button = nullptr;
    Fl_Round_Button *header_button = nullptr;
    Fl_Round_Button *text_button = nullptr;
    Fl_Round_Button *footnote_button = nullptr;
    Fl_Round_Button *footer_button = nullptr;
    Fl_Button *delete_grid_button = nullptr;
    Fl_Input *page_input = nullptr;
    Fl_Button *go_page_button = nullptr;
    Fl_Button *prev_page_button = nullptr;
    Fl_Button *next_page_button = nullptr;
    Fl_Button *font_bigger_button = nullptr;
    Fl_Button *font_smaller_button = nullptr;
    Fl_Button *input_narrower_button = nullptr;
    Fl_Button *input_wider_button = nullptr;
    Fl_Button *insert_word_break_button = nullptr;
    Fl_Button *insert_emdash_button = nullptr;
    Fl_Button *insert_section_break_button = nullptr;
    Fl_Button *insert_paragraph_break_button = nullptr;
    Fl_Button *insert_line_break_button = nullptr;
    Fl_Button *save_page_button = nullptr;
    Fl_Scroll *scroll = nullptr;
    Fl_Group *merged_buttons = nullptr;
    Fl_Group *merged_group = nullptr;
    Fl_Image_Box *box = nullptr;
    Fl_Group *split_buttons = nullptr;
    Fl_Group *split_group = nullptr;
    std::vector<Fl_Input *> *inputs = new std::vector<Fl_Input *>();
    Fl_Split_Box *box2 = nullptr;
};


#endif /* main_window_hpp */
