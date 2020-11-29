//
//  main_window.cpp
//  proofr
//
//  Created by michael on 11/11/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

#include <cmath>
#include <unistd.h>

#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>

#define FLTK_ABI_VERSION 10305  // fix bug

#include <FL/Enumerations.H>
#include <FL/Fl.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>

#include "main_window.hpp"
#include "grids.hpp"
#include "utils.hpp"

using namespace std;

Fl_Input *fl_input = new Fl_Input(0, 0, 0, 0);

void main_window_cb(Fl_Widget* widget, void *userp);
void merged_button_cb(Fl_Widget* widget, void *userp);
void split_button_cb(Fl_Widget* widget, void *userp);
void new_grid_button_cb(Fl_Widget* widget, void *userp);
void header_button_cb(Fl_Widget* widget, void *userp);
void text_button_cb(Fl_Widget* widget, void *userp);
void footnote_button_cb(Fl_Widget* widget, void *userp);
void footer_button_cb(Fl_Widget* widget, void *userp);
void delete_grid_button_cb(Fl_Widget* widget, void *userp);
void page_input_cb(Fl_Widget* widget, void *userp);
void edited_page_cb(Fl_Widget* widget, void *userp);
void go_page_button_cb(Fl_Widget* widget, void *userp);
void prev_page_button_cb(Fl_Widget* widget, void *userp);
void next_page_button_cb(Fl_Widget* widget, void *userp);
void font_bigger_button_cb(Fl_Widget* widget, void *userp);
void font_smaller_button_cb(Fl_Widget* widget, void *userp);
void input_narrower_button_cb(Fl_Widget* widget, void *userp);
void input_wider_button_cb(Fl_Widget* widget, void *userp);
void insert_word_break_button_cb(Fl_Widget* widget, void *userp);
void insert_emdash_button_cb(Fl_Widget* widget, void *userp);
void insert_section_break_button_cb(Fl_Widget* widget, void *userp);
void insert_paragraph_break_button_cb(Fl_Widget* widget, void *userp);
void insert_line_break_button_cb(Fl_Widget* widget, void *userp);
void save_page_button_cb(Fl_Widget* widget, void *userp);

void resizingTimer(void *data);

void export_text_cb(Fl_Widget* widget, void *userp);
void export_html_cb(Fl_Widget* widget, void *userp);
void undo_cb(Fl_Widget* widget, void *userp);
void cut_cb(Fl_Widget* widget, void *userp);
void copy_cb(Fl_Widget* widget, void *userp);
void paste_cb(Fl_Widget* widget, void *userp);
void delete_cb(Fl_Widget* widget, void *userp);
void set_even_default_grids_cb(Fl_Widget* widget, void *userp);
void set_odd_default_grids_cb(Fl_Widget* widget, void *userp);

const int LAST_PAGE = 453;

bool cleaned_text_path(int pageNumber, string& path);
bool cleaned_text_path(int pageNumber, string& path)
{
    char str[256];

    sprintf(str, "/Users/michael/Documents/Seneca/cu31924026554281_cubook/Cleaned/31924026554281_%06d.txt",
            pageNumber);

    path = str;

    return fileExists(str);
}

// =================================================================================================

Fl_Main_Window::Fl_Main_Window(int x, int y, int width, int height, const char *title) :
Fl_Double_Window(x, y, width, height, title)
{
    menu_bar = new Fl_My_Sys_Menu_Bar(0, 0, 0, 0);
    menu_bar->add("File/Export Text...",  0, export_text_cb, this);
    menu_bar->add("File/Export HTML...",  0, export_html_cb, this);
    menu_bar->add("Edit/Undo",  FL_COMMAND + 'z', undo_cb, this, FL_MENU_DIVIDER);
    menu_bar->add("Edit/Cut",  FL_COMMAND + 'x', cut_cb, this);
    menu_bar->add("Edit/Copy",  FL_COMMAND + 'c', copy_cb, this);
    menu_bar->add("Edit/Paste",  FL_COMMAND + 'v', paste_cb, this);
    menu_bar->add("Edit/Delete",  0, delete_cb, this);
    menu_bar->add("Grids/Set Even Default Grids",  0, set_even_default_grids_cb, this, FL_MENU_INACTIVE);
    menu_bar->add("Grids/Set Odd Default Grids",  0, set_odd_default_grids_cb, this, FL_MENU_INACTIVE);

    callback(main_window_cb);

    merged_button = new Fl_Round_Button(0, 0, 0, 0, "Merged");
    merged_button->set();
    merged_button->callback(merged_button_cb, this);

    split_button = new Fl_Round_Button(0, 0, 0, 0, "Split");
    split_button->callback(split_button_cb, this);

    merged_buttons = new Fl_Group(0, 0, 0, 0);
    new_grid_button = new Fl_Button(0, 0, 0, 0, "New Grid");
    new_grid_button->callback(new_grid_button_cb, this);

    header_button = new Fl_Round_Button(0, 0, 0, 0, "H");
    header_button->set();
    header_button->callback(header_button_cb, this);

    text_button = new Fl_Round_Button(0, 0, 0, 0, "T");
    text_button->callback(text_button_cb, this);

    footnote_button = new Fl_Round_Button(0, 0, 0, 0, "Fn");
    footnote_button->callback(footnote_button_cb, this);

    footer_button = new Fl_Round_Button(0, 0, 0, 0, "F");
    footer_button->callback(footer_button_cb, this);

    delete_grid_button = new Fl_Button(0, 0, 0, 0, "Delete Grid");
    delete_grid_button->callback(delete_grid_button_cb, this);

    page_input = new Fl_Input(0, 0, 0, 0, "Page");
    page_input->insert("0");
    page_input->callback(page_input_cb, this);
    page_input->when(FL_WHEN_CHANGED | FL_WHEN_ENTER_KEY);

    go_page_button = new Fl_Button(0, 0, 0, 0, "Go");
    go_page_button->callback(go_page_button_cb, this);
    go_page_button->deactivate();

    prev_page_button = new Fl_Button(0, 0, 0, 0, "<");
    prev_page_button->callback(prev_page_button_cb, this);

    next_page_button = new Fl_Button(0, 0, 0, 0, ">");
    next_page_button->callback(next_page_button_cb, this);

    merged_buttons->end();

    split_buttons = new Fl_Group(0, 0, 0, 0);

    font_bigger_button = new Fl_Button(0, 0, 0, 0, "A+");
    font_bigger_button->callback(font_bigger_button_cb, this);

    font_smaller_button = new Fl_Button(0, 0, 0, 0, "A-");
    font_smaller_button->callback(font_smaller_button_cb, this);

    input_narrower_button = new Fl_Button(0, 0, 0, 0, "->");
    input_narrower_button->callback(input_narrower_button_cb, this);

    input_wider_button = new Fl_Button(0, 0, 0, 0, "<-");
    input_wider_button->callback(input_wider_button_cb, this);

    insert_word_break_button = new Fl_Button(0, 0, 0, 0, "¬");
    insert_word_break_button->callback(insert_word_break_button_cb, this);

    insert_emdash_button = new Fl_Button(0, 0, 0, 0, "—");
    insert_emdash_button->callback(insert_emdash_button_cb, this);

    insert_paragraph_break_button = new Fl_Button(0, 0, 0, 0, "¶");
    insert_paragraph_break_button->callback(insert_paragraph_break_button_cb, this);

    insert_line_break_button = new Fl_Button(0, 0, 0, 0, "|");
    insert_line_break_button->callback(insert_line_break_button_cb, this);

    insert_section_break_button = new Fl_Button(0, 0, 0, 0, "§");
    insert_section_break_button->callback(insert_section_break_button_cb, this);

    save_page_button = new Fl_Button(0, 0, 0, 0, "Save");
    save_page_button->callback(save_page_button_cb, this);
    save_page_button->deactivate();
    save_page_button->labelcolor(FL_FOREGROUND_COLOR);

    split_buttons->end();

    scroll = new Fl_Scroll(0, 0, 0, 0);
    scroll->type(Fl_Scroll::VERTICAL_ALWAYS);
    scroll->color(FL_GRAY);

    merged_group = new Fl_Group(0, 0, 0, 0);
    box = new Fl_Image_Box(this, Fl_Boxtype::FL_BORDER_BOX, 0, 0, 0, 650, "");
    box->color(FL_BLUE);
    merged_group->end();

    split_group = new Fl_Group(0, 0, 0, 0);

    box2 = new Fl_Split_Box(this, Fl_Boxtype::FL_NO_BOX, 0, 0, 0, 350, "");
    box2->color(FL_GRAY);

    split_group->end();

    load_page(0);

    if (merged_button->value()) {
        split_buttons->hide();
        split_group->hide();
        scroll->remove(split_group);

    } else {
        merged_buttons->hide();
        merged_group->hide();
        scroll->remove(merged_group);
    }

    scroll->end();

    position_widgets(true);

    resizable(this);
    end();
    show();

}

Fl_Main_Window::~Fl_Main_Window()
{
    cout << "deleting Fl_Multi_Window" << endl;
}

void Fl_Main_Window::load_page(int pageNumber)
{
//    cout << "load_page(" << pageNumber << ")" << endl;

    if (current_page >= 0) leaving_page(current_page);

    char str[256];

    sprintf(str, "/Users/michael/Documents/Seneca/cu31924026554281_cubook/IMAGES/31924026554281_%06d.JP2",
            pageNumber);
    src_image_path = string(str);
//    if (!fileExists(src_image_path)) cout << "No such file: " << src_image_path << endl;

    sprintf(str, "/Users/michael/Documents/Seneca/cu31924026554281_cubook/PNG/31924026554281_%06d.png",
            pageNumber);
    dest_image_path = string(str);
//    if (!fileExists(dest_image_path)) cout << "No such file: " << dest_image_path << endl;

    sprintf(str, "/Users/michael/Documents/Seneca/cu31924026554281_cubook/OCR/31924026554281_%06d.txt",
            pageNumber);
    src_text_file_path = string(str);
//    if (!fileExists(src_text_file_path)) cout << "No such file: " << src_text_file_path << endl;


    cleaned_text_path(pageNumber, dest_text_file_path);

//    sprintf(str, "/Users/michael/Documents/Seneca/cu31924026554281_cubook/Cleaned/31924026554281_%06d.txt",
//            pageNumber);
//    dest_text_file_path = string(str);
////    if (!fileExists(dest_text_file_path)) cout << "No such file: " << dest_text_file_path << endl;

    if (fileExists(src_image_path) &&
            (fileExists(dest_text_file_path) || fileExists(src_text_file_path))) {

        if (!fileToString(dest_text_file_path, pageText)) fileToString(src_text_file_path, pageText);

        entering_page(pageNumber);

        stringstream sstr;
        sstr << "/usr/local/bin/gm convert " << src_image_path << " " << dest_image_path;
        system(sstr.str().c_str());

        pageImage = new Fl_PNG_Image(dest_image_path.c_str());

        pageImageSourceWidth = pageImage->w();
        pageImageSourceHeight = pageImage->h();
        pageImageWidth = pageImageSourceWidth;
        pageImageHeight = pageImageSourceHeight;

        for (size_t k = 0; k < inputs->size(); k++) {
            delete inputs->at(k);
        }

        inputs->clear();

        bool skipping_spaces = true;
        string line;
        for (size_t k = 0; k < pageText.length(); k++) {
            unsigned char c = pageText[k];
            if (c == '\x0b' || k == pageText.length() - 1) {
                if (line.length() > 0) {
                    //                cout << "] " << line << endl;

                    Fl_Input *input = new Fl_Input(0, 0, 0, 3 * input_text_size / 2);
                    input->textcolor(FL_DARK_BLUE);
                    input->textfont(FL_TIMES);
                    input->textsize(input_text_size);
                    input->insert(line.c_str());
                    input->callback(edited_page_cb, this);
                    input->when(FL_WHEN_CHANGED);
                    split_group->add(input);

                    inputs->push_back(input);

                    line.clear();
                    skipping_spaces = true;
                }

            } else if (c == ' ' && !skipping_spaces) {
                skipping_spaces = true;

            } else if (c != ' ') {
                if (skipping_spaces && line.length() != 0) {
                    line.append(" ");
                }
                skipping_spaces = false;

                if (c == 0xAC) {            // Latin-1 not ¬
                    line.append("¬");

                } else  if (c == 0xB6) {    // latin-1 pilcrow ¶
                    line.append("¶");

                } else  if (c == 0xA7) {    // latin-1 section sign §
                    line.append("§");

                } else  if (c == 0xA6) {    // latin-1 broken bar ¦ (placeholder for em dash)
                    line.append("—");       // em dash

                } else  if ((c & 0x80) != 0) {    // high-ascii
                    cout << "load_page unknown high ASCII 0x" << hex << setw(2) << uppercase << setfill('0') <<
                        (unsigned int)(unsigned char)c << endl;

                    line.append("▽");

                } else {
                    line.append((char *)&c, 1);
                }
            }
        }

        finishResizing();
        current_page = pageNumber;

        if (pageNumber % 2 == 0) {
            ((Fl_Menu_Item *)menu_bar->find_item(set_even_default_grids_cb))->activate();
            ((Fl_Menu_Item *)menu_bar->find_item(set_odd_default_grids_cb))->deactivate();

        } else {
            ((Fl_Menu_Item *)menu_bar->find_item(set_even_default_grids_cb))->deactivate();
            ((Fl_Menu_Item *)menu_bar->find_item(set_odd_default_grids_cb))->activate();
        }

        menu_bar->do_update();

        menu_bar->redraw();
    }

    page_input->value(to_string(current_page).c_str());

    save_page_button->deactivate();
    save_page_button->labelcolor(FL_FOREGROUND_COLOR);
    page_input->labelcolor(FL_FOREGROUND_COLOR);
    merged_buttons->redraw();
}

void Fl_Main_Window::resize(int x, int y, int width, int height)
{
    Fl::remove_timeout(resizingTimer);
    Fl::add_timeout(0.5, resizingTimer, this);
    resize(x, y, width, height, false);
}

void Fl_Main_Window::finishResizing()
{
//    cout << "finishResizing" << endl;
    resize(x(), y(), w(), h(), true);
    redraw();
}

void Fl_Main_Window::resize(int x, int y, int width, int height, bool adjustImage)
{
//    cout << "resize " << width << "x" << height << " " << (adjustImage ? "T" : "F") << endl;

    Fl_Double_Window::resize(x, y, width, height);
    position_widgets(adjustImage);
}

void Fl_Main_Window::position_widgets(bool adjustImage)
{
    int width = w();
    int height = h();

    merged_button->resize(MARGIN, MARGIN, 60, 20);
    split_button->resize(3 * MARGIN / 2 + 60, MARGIN, 60, 20);

    merged_buttons->resize(split_button->x() + split_button->w(), MARGIN,
                           width - (split_button->x() + split_button->w()), 20);
    new_grid_button->resize(merged_buttons->x() + 10, MARGIN, 80, 20);

    header_button->resize(new_grid_button->x() + new_grid_button->w() + 5, MARGIN, 30, 20);
    text_button->resize(header_button->x() + header_button->w(), MARGIN, 30, 20);
    footnote_button->resize(text_button->x() + text_button->w(), MARGIN, 35, 20);
    footer_button->resize(footnote_button->x() + footnote_button->w(), MARGIN, 30, 20);

    delete_grid_button->resize(footer_button->x() + footer_button->w() + 10, MARGIN, 90, 20);
    page_input->resize(delete_grid_button->x() + delete_grid_button->w() + 55, MARGIN, 40, 20);
    go_page_button->resize(page_input->x() + page_input->w() + 5, MARGIN, 40, 20);
    prev_page_button->resize(go_page_button->x() + go_page_button->w() + 5, MARGIN, 20, 20);
    next_page_button->resize(prev_page_button->x() + prev_page_button->w() + 5, MARGIN, 20, 20);

    split_buttons->resize(split_button->x() + split_button->w(), MARGIN,
                          width - (split_button->x() + split_button->w()), 20);

    font_bigger_button->resize(split_buttons->x() + 20, MARGIN, 40, 20);
    font_smaller_button->resize(font_bigger_button->x() + font_bigger_button->w() + 10, MARGIN, 40, 20);
    input_narrower_button->resize(font_smaller_button->x() + font_smaller_button->w() + 25, MARGIN, 30, 20);
    input_wider_button->resize(input_narrower_button->x() + input_narrower_button->w() + 10, MARGIN, 30, 20);
    insert_word_break_button->resize(input_wider_button->x() + input_wider_button->w() + 25, MARGIN, 30, 20);
    insert_emdash_button->resize(insert_word_break_button->x() + insert_word_break_button->w() + 10, MARGIN, 30, 20);
    insert_paragraph_break_button->resize(
        insert_emdash_button->x() + insert_emdash_button->w() + 10, MARGIN, 30, 20);
    insert_line_break_button->resize(
        insert_paragraph_break_button->x() + insert_paragraph_break_button->w() + 10, MARGIN, 30, 20);
    insert_section_break_button->resize(
        insert_line_break_button->x() + insert_line_break_button->w() + 10, MARGIN, 30, 20);

    save_page_button->resize(
        insert_section_break_button->x() + insert_section_break_button->w() + 30, MARGIN, 80, 20);

    scroll->resize(MARGIN, 2 * MARGIN + 20, width - 2 * MARGIN, height - 20 - 3 * MARGIN);

    if (adjustImage) {
        int desiredWidth = width - 2 * MARGIN - scroll->scrollbar.w();
        int desiredHeight = (int)round(
            (double)desiredWidth * pageImageSourceHeight / pageImageSourceWidth);

        if (pageImageWidth != desiredWidth || pageImageHeight != desiredHeight) {
            stringstream sstr;
            sstr << "/usr/local/bin/gm convert -resize " <<
                desiredWidth << "x" << desiredHeight << " " << src_image_path << " " << dest_image_path;

            system(sstr.str().c_str());

            if (pageImage != nullptr) {
                delete pageImage;
                pageImage = nullptr;

            } else {
                cout << "No converted page image" << endl;
            }

            pageImage = new Fl_PNG_Image(dest_image_path.c_str());

            box->redraw();
            box2->redraw();

            pageImageWidth = desiredWidth;
            pageImageHeight = desiredHeight;
        }
    }

    merged_group->resize(MARGIN, 2 * MARGIN + 20, pageImageWidth, pageImageHeight);
    box->resize(MARGIN, 2 * MARGIN + 20, pageImageWidth, pageImageHeight);

    adjustSplitHeight();
}

void Fl_Main_Window::adjustSplitHeight()
{
    int width = w();

    size_t grid_count = 0;
    double split_height = 0;
    for (int k = 0; k < grids.size(); k++) {
        grid_count += grids[k].nrows;
        split_height += grids[k].nrows * grids[k].row_height_rel * pageImageHeight;
    }

    split_height += grid_count * edit_space();
    int box2_height = (int)ceil(split_height);

    split_group->resize(MARGIN / 2, 2 * MARGIN + 20, width - 3 * MARGIN / 2 - scroll->scrollbar.w(), box2_height);
    box2->resize(MARGIN / 2, 2 * MARGIN + 20, width - 3 * MARGIN / 2 - scroll->scrollbar.w(), box2_height);

    size_t current_input = 0;
    size_t current_grid = 0;
    int current_row_of_grid = 0;
    int current_nrows = grids.size() > 0 ? grids[0].nrows : 0;
    double current_row_height = grids.size() > 0 ? grids[0].row_height_rel * pageImageHeight : 0.0;
    double y_input = box2->y();
    while (current_grid < grids.size() && current_input < inputs->size()) {
        y_input += current_row_height;
        inputs->at(current_input)->resize(
            MARGIN + input_text_inset, (int)round(y_input) + 5,
            width - 3 * MARGIN - scroll->scrollbar.w() - input_text_inset, 3 * input_text_size / 2);

        y_input += edit_space();

        if (++current_row_of_grid >= current_nrows) {
            current_grid++;
            current_row_of_grid = 0;

            if (current_grid < grids.size()) {
                current_nrows = grids[current_grid].nrows;
                current_row_height = grids[current_grid].row_height_rel * pageImageHeight;
            }
        }

        current_input++;
    }
}

void Fl_Main_Window::merged()
{
//    cout << "Merged!" << endl;
    split_button->clear();

    split_buttons->hide();
    split_group->hide();
    scroll->remove(split_group);

    scroll->add(merged_group);
    merged_group->show();
    merged_buttons->show();
}

void Fl_Main_Window::split()
{
//    cout << "Split!" << endl;
    merged_button->clear();

    merged_buttons->hide();
    merged_group->hide();
    scroll->remove(merged_group);

    scroll->add(split_group);
    split_group->show();
    split_buttons->show();
}

void Fl_Main_Window::new_grid()
{
    Grid::GridType gridType = Grid::TEXT;
    if (header_button->value()) {
        gridType = Grid::HEADER;
        text_button->set();
        header_button->clear();

    } else if (footnote_button->value()) {
        gridType = Grid::FOOTNOTE;

    } else if (footer_button->value()) {
        gridType = Grid::FOOTER;
    }

    newGrid(pageImageHeight, gridType);
    box->redraw();

    adjustSplitHeight();
    box2->redraw();
}

void Fl_Main_Window::header_grid()
{
    text_button->clear();
    footnote_button->clear();
    footer_button->clear();
}

void Fl_Main_Window::text_grid()
{
    header_button->clear();
    footnote_button->clear();
    footer_button->clear();
}

void Fl_Main_Window::footnote_grid()
{
    header_button->clear();
    text_button->clear();
    footer_button->clear();
}

void Fl_Main_Window::footer_grid()
{
    header_button->clear();
    text_button->clear();
    footnote_button->clear();
}

void Fl_Main_Window::delete_grid()
{
    deleteLastGrid();
    box->redraw();

    adjustSplitHeight();
    box2->redraw();
}

void Fl_Main_Window::go_page()
{
//    cout << "go_page" << endl;
    int new_page_number = atoi(page_input->value());
    if (new_page_number >= 0) load_page(new_page_number);
    go_page_button->deactivate();
    focus(merged_button);
}

void Fl_Main_Window::prev_page()
{
//    cout << "prev_page" << endl;
    if (current_page > 0) load_page(current_page - 1);
}

void Fl_Main_Window::next_page()
{
//    cout << "next_page" << endl;
    load_page(current_page + 1);
}

void Fl_Main_Window::font_bigger()
{
//    cout << "font_bigger" << endl;
    input_text_size++;

    for (size_t k = 0; k < inputs->size(); k++) {
        inputs->at(k)->textsize(input_text_size);
    }

    adjustSplitHeight();
    redraw();
}

void Fl_Main_Window::font_smaller()
{
//    cout << "font_smaller" << endl;
    input_text_size--;
    if (input_text_size < 12) input_text_size = 12;

    for (size_t k = 0; k < inputs->size(); k++) {
        inputs->at(k)->textsize(input_text_size);
    }

    adjustSplitHeight();
    redraw();
}

void Fl_Main_Window::input_narrower()
{
//    cout << "input_narrower" << endl;
    input_text_inset += 2;
    adjustSplitHeight();
    redraw();
}

void Fl_Main_Window::input_wider()
{
//    cout << "input_wider" << endl;
    input_text_inset -= 2;
    if (input_text_size < 0) input_text_size = 0;

    adjustSplitHeight();
    redraw();
}

void Fl_Main_Window::insert_word_break()
{
    //    cout << "insert_word_break" << endl;
    Fl_Widget *widget = Fl::focus();
    bool found = false;
    for (size_t k = 0; k < inputs->size() && !found; k++) {
        found = widget == inputs->at(k);
    }

    if (found) {
        ((Fl_Input *)widget)->insert("¬");
    }
}

void Fl_Main_Window::insert_emdash()
{
    //    cout << "insert_word_break" << endl;
    Fl_Widget *widget = Fl::focus();
    bool found = false;
    for (size_t k = 0; k < inputs->size() && !found; k++) {
        found = widget == inputs->at(k);
    }

    if (found) {
        ((Fl_Input *)widget)->insert("—");
    }
}

void Fl_Main_Window::insert_line_break()
{
    //    cout << "insert_line_break" << endl;
    Fl_Widget *widget = Fl::focus();
    bool found = false;
    for (size_t k = 0; k < inputs->size() && !found; k++) {
        found = widget == inputs->at(k);
    }

    if (found) {
        ((Fl_Input *)widget)->insert("|");
    }
}

void Fl_Main_Window::insert_paragraph_break()
{
    //    cout << "insert_paragraph_break" << endl;
    Fl_Widget *widget = Fl::focus();
    bool found = false;
    for (size_t k = 0; k < inputs->size() && !found; k++) {
        found = widget == inputs->at(k);
    }

    if (found) {
        ((Fl_Input *)widget)->insert("¶ ");
    }
}

void Fl_Main_Window::insert_section_break()
{
    //    cout << "insert_section_break" << endl;
    Fl_Widget *widget = Fl::focus();
    bool found = false;
    for (size_t k = 0; k < inputs->size() && !found; k++) {
        found = widget == inputs->at(k);
    }

    if (found) {
        ((Fl_Input *)widget)->insert("§");
    }
}

void Fl_Main_Window::save_page()
{
//    cout << "save_page" << endl;
    stringstream stext;

    for (size_t k = 0; k < inputs->size(); k++) {
        string line(inputs->at(k)->value());

        for (size_t i = 0; i < line.length(); i++) {
            unsigned char c = line.at(i);
            if (c == 0xC2) {
                if (++i < line.length()) {
                    c = line.at(i);
                    switch (c) {
                        case 0xAC:      stext << '\xAC';    break;  // not
                        case 0xB6:      stext << '\xB6';    break;  // pilcrow
                        case 0xA7:      stext << '\xA7';    break;  // section sign
                        default:
                            cout << "save_page 1 unknown high ascii " << hex << setw(2) << uppercase << setfill('0') <<
                            (unsigned int)(unsigned char)c << endl;
                    }
                }

            } else if (c == 0xE2) {
                if (++i < line.length()) {
                    c = line.at(i);
                    if (c == 0x80 && ++i < line.length()) {
                        c = line.at(i);
                        if (c == 0x94) {
                            stext << '\xA6';    // broken bar, placeholder for emdash
                        } else {
                            cout << "save_page 2 unknown high ascii " << hex << setw(2) << uppercase << setfill('0') <<
                            (unsigned int)(unsigned char)c << endl;
                        }

                    } else {
                        cout << "save_page 3 unknown high ascii " << hex << setw(2) << uppercase << setfill('0') <<
                        (unsigned int)(unsigned char)c << endl;
                    }
                }

            } else {
                if ((c & 0x80) != 0) {
                    cout << "save_page 4 unknown high ascii " << hex << setw(2) << uppercase << setfill('0') <<
                        (unsigned int)(unsigned char)c << endl;
                }
                stext << c;
            }
        }

        stext << '\x0B';
    }

    stringToFile(stext.str(), dest_text_file_path);

    save_page_button->deactivate();
    save_page_button->labelcolor(FL_FOREGROUND_COLOR);
    page_input->labelcolor(FL_FOREGROUND_COLOR);
}

void Fl_Main_Window::page_input_changed()
{
//    cout << "page_input_changed" << endl;
    go_page_button->activate();
}

void Fl_Main_Window::edited_page()
{
//    cout << "edited_page" << endl;
    save_page_button->activate();
    save_page_button->labelcolor(fl_rgb_color(144, 0, 0));
    page_input->labelcolor(fl_rgb_color(144, 0, 0));
}

void append_space_or_newline_to_line(Grid::GridType grid_type, bool use_line_breaks, bool within_tag,
                                     int &non_tag_line_length_text, int &non_tag_line_length_footnote, int min_line_length, string &line);

void append_space_or_newline_to_line(Grid::GridType grid_type, bool use_line_breaks, bool within_tag,
    int &non_tag_line_length_text, int &non_tag_line_length_footnote, int min_line_length, string &line)
{
    switch (grid_type) {
        case Grid::TEXT:
            if (use_line_breaks && non_tag_line_length_text >= min_line_length) {
                line.append("\n");
                non_tag_line_length_text = 0;

            } else {
                line.append(" ");
                if (!within_tag) non_tag_line_length_text++;
            }
            break;

        case Grid::FOOTNOTE:
            if (use_line_breaks && non_tag_line_length_footnote >= min_line_length) {
                line.append("\n");
                non_tag_line_length_footnote = 0;

            } else {
                line.append(" ");
                if (!within_tag) non_tag_line_length_footnote++;
            }
            break;

        default:
            line.append(" ");
            break;
    }
}

void Fl_Main_Window::export_text(bool to_html)
{
    bool show_page_index = false;
    bool show_page_number = false;
    bool footnotes_after_paragraph = false;
    bool use_line_breaks = true;
    int min_line_length = 60;

    ostringstream oss_text;
    ostringstream oss_fn;
    ostringstream oss_header;

    if (to_html) {
        oss_text <<
            "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
            " \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
            "\n"
            "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n"
            "\n"
            "<head>\n"
            "\n"
            "<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\n"
            "<meta http-equiv=\"Content-Style-Type\" content=\"text/css\" />\n"
            "\n"
            "<title>Of Peace of Mind, by Lucius Annaeus Seneca</title>\n"
            "\n"
            "<style type=\"text/css\">\n"
            "\n"
            "body {\n"
            "    margin-left: 10%;\n"
            "    margin-right: 10%;\n"
            "}\n"
            "div.text {\n"
            "    text-align: justify;\n"
            "}\n"
            "\n";

        if (show_page_index) {
            // show in red to signal for debugging only
            oss_text <<
                ".npage {\n"
                "    # visibility: hidden;\n"
                "    position: absolute;\n"
                "    left: 92%;\n"
                "    font-size: smaller;\n"
                "    text-align: right;\n"
                "    color: red;\n"
                "}\n";

        } else {
            oss_text <<
                ".npage {\n"
                "    # visibility: hidden;\n"
                "    position: absolute;\n"
                "    left: 92%;\n"
                "    font-size: smaller;\n"
                "    text-align: right;\n"
                "}\n";
        }

        oss_text <<
            "\n"
            ".center {\n"
            "    text-align: center;\n"
            "}\n"
            "\n"
            "blockquote {\n"
            "    font-size: 95%;\n"
            "}\n"
            "\n"
            ".centerhead {\n"
            "    text-align: center;\n"
            "    font-size: 120%;\n"
            "}\n"
            "\n"
            "h1 {\n"
            "    text-align: center;\n"
            "    font-size: 140%;\n"
            "    font-weight: normal;\n"
            "    #line-height: 0.8;\n\n"
            "}"
            "\n"
            "h2 {\n"
            "    text-align: center;\n"
            "    font-size: 180%;\n"
            "    font-weight: normal;\n"
            "}\n"
            "\n"
            "p.footnote {\n"
            "    font-size: 90%;\n"
            "    font-family: sans-serif;\n"
            "}\n"
            "\n"
            "fn {\n"
            "    vertical-align: top;\n"
            "    font-size: 70%;\n"
            "    font-family: sans-serif;\n"
            "}\n"
            "\n"
            "</style>\n"
            "</head>\n"
            "\n"
            "<body>\n"
            "\n";
    }

    bool previous_whitespace_char = true;
    bool within_tag = false;
    bool first_paragraph = true;
    int non_tag_line_length_text = 0;
    int non_tag_line_length_footnote = 0;
    bool breaking_word_text = false;
    bool breaking_word_footnote = false;

    for (int n = 0; n < LAST_PAGE; n++) {
        string next_cleaned_path;
        bool next_cleaned_path_exists = cleaned_text_path(n, next_cleaned_path);

        if (next_cleaned_path_exists) {
            string next_page_text;
            fileToString(next_cleaned_path, next_page_text);

            std::vector<Grid> next_grids;
            read_grids(n, next_grids);

            if (next_grids.size() > 0) {
                size_t grid_index = 0;
                int row_in_grid = 0;
                Grid grid = next_grids.at(grid_index);

                bool skipping_spaces = true;
                string line;
                for (size_t k = 0; k < next_page_text.length() && grid_index < next_grids.size(); k++) {
                    unsigned char c = next_page_text[k];
                    if (c == '\x0b') {
                        if (line.length() > 0) {
                            // finished line
                            bool breaking_word;
                            switch (grid.grid_type) {
                                case Grid::TEXT:        breaking_word = breaking_word_text;     break;
                                case Grid::FOOTNOTE:    breaking_word = breaking_word_footnote; break;
                                default:                breaking_word = false;                  break;
                            }

                            if (!breaking_word && line.length() > 0) {

                                append_space_or_newline_to_line(grid.grid_type, use_line_breaks,
                                                                within_tag, non_tag_line_length_text,
                                                                non_tag_line_length_footnote, min_line_length, line);

                                previous_whitespace_char = true;
                            }

                            switch (grid.grid_type) {
                                case Grid::HEADER:
                                    if (show_page_index) {
                                        if (to_html) {
                                            oss_text << "<span class=\"npage\">" << n << "</span>";

                                        } else {
                                            oss_text << endl << "{" << n << "}" << endl;
                                        }

                                    } else if (show_page_number) {
                                        if (to_html) {
                                            oss_text << "<span class=\"npage\">" << line << "</span>";

                                        } else {
                                            oss_text << endl << "{" << line << "}" << endl;
                                        }
                                    }
                                    break;
                                case Grid::TEXT:
                                    oss_text << line;
                                    breaking_word_text = false;
                                    break;
                                case Grid::FOOTNOTE:
                                    if (to_html) {
                                        if (oss_fn.str().length() == 0) {
                                            oss_fn << endl <<  "<hr />" << endl;
                                        }

                                        if (row_in_grid == 0) {
                                            oss_fn <<
                                                "<a href=\"#fn-NNN\" name=\"fnref-NNN\" id=\"fnref-NNN\"><sup>[NNN]</sup></a>\n"
                                                "<p class=\"footnote\">\n"
                                                "<a name=\"fn-NNN\" id=\"fn-NNN\"></a> <a href=\"#fnref-NNN\">[NNN]</a>\n";
                                        }

                                        oss_fn << line;

                                        if (row_in_grid == grid.nrows - 1) {
                                            oss_fn << endl << "</p>\n\n<hr />" << endl<< endl;
                                        }

                                    } else {
                                        if (oss_fn.str().length() == 0) {
                                            oss_fn << endl << ("_____") << endl;
                                        }

                                        oss_fn << line;

                                        if (row_in_grid == grid.nrows - 1) {
                                            oss_fn << endl << ("_____") << endl;
                                        }
                                    }
                                    breaking_word_footnote = false;
                                    break;
                                case Grid::FOOTER:
                                    /* ignore */
                                    break;
                            }

                            line.clear();
                            skipping_spaces = true;

                            if (++row_in_grid >= grid.nrows) {
                                if (++grid_index < next_grids.size()) {
                                    row_in_grid = 0;
                                    grid = next_grids.at(grid_index);
                                }
                            }
                        }

                    } else if (c == ' ' && !skipping_spaces) {
                        skipping_spaces = true;

                    } else if (c != ' ') {
                        if (skipping_spaces && line.length() != 0) {
                            append_space_or_newline_to_line(grid.grid_type, use_line_breaks,
                                                            within_tag, non_tag_line_length_text,
                                                            non_tag_line_length_footnote, min_line_length, line);

                            previous_whitespace_char = true;
                        }

                        skipping_spaces = false;

                        switch (c) {
                            case 0xAC:      // Latin-1 not ¬
                                if (grid.grid_type == Grid::TEXT) {
                                    breaking_word_text = true;

                                } else if (grid.grid_type == Grid::FOOTNOTE) {
                                    breaking_word_footnote = true;
                                }
                                break;

                            case 0xB6:      // latin-1 pilcrow ¶
                                k++;
                                skipping_spaces = true;

                                if (to_html) oss_text << (first_paragraph ? "<p>" : "</p>\n\n<p>");
                                first_paragraph = false;

                                if (footnotes_after_paragraph && oss_fn.str().length() > 0) {
                                    oss_text << oss_fn.str();
                                    oss_fn.str("");
                                    if (to_html) oss_text << (first_paragraph ? "<p>" : "</p>\n\n<p>");

                                } else {
                                    if (!to_html) oss_text << endl;
                                }

                                if (!to_html) oss_text << endl;
                                non_tag_line_length_text = 0;
                                break;

                            case '|':
                                oss_text << line;
                                line.clear();
                                skipping_spaces = true;

                                oss_text << endl;
                                non_tag_line_length_text = 0;
                                break;

                            case 0xA7:      // Latin-1 section sign §
                                if (oss_fn.str().length() > 0) {
                                    oss_text << line;
                                    line.clear();
                                    skipping_spaces = true;

                                    oss_text << endl << oss_fn.str() << endl;
                                    non_tag_line_length_text = 0;
                                    oss_fn.str("");
                                }
                                break;

                            case 0xA6:      // latin-1 broken bar ¦ placeholder for em dash
                                // line.append("--");
                                line.append(to_html ? "&mdash;" : "—");
                                if (grid.grid_type == Grid::TEXT) {
                                    if (!within_tag) non_tag_line_length_text++;

                                } else if (grid.grid_type == Grid::FOOTNOTE) {
                                    if (!within_tag) non_tag_line_length_footnote++;
                                }

                                previous_whitespace_char = false;
                                break;

                            case '"':
                                if (within_tag) {
                                    line.append("\"");

                                } else {
                                    if (to_html) {
                                        line.append(previous_whitespace_char ? "&ldquo;" : "&rdquo;");

                                    } else {
                                        line.append(previous_whitespace_char ? "“" : "”");
                                    }

                                    if (grid.grid_type == Grid::TEXT) {
                                        non_tag_line_length_text++;

                                    } else if (grid.grid_type == Grid::FOOTNOTE) {
                                        non_tag_line_length_footnote++;
                                    }
                                }

                                previous_whitespace_char = false;
                                break;

                            case '\'':
                                if (within_tag) {
                                    line.append("'");

                                } else {
                                    if (to_html) {
                                        line.append(previous_whitespace_char ? "&lsquo;" : "&rsquo;");

                                    } else {
                                        line.append(previous_whitespace_char ? "‘" : "’");
                                    }

                                    if (grid.grid_type == Grid::TEXT) {
                                        non_tag_line_length_text++;

                                    } else if (grid.grid_type == Grid::FOOTNOTE) {
                                        non_tag_line_length_footnote++;
                                    }
                                }

                                previous_whitespace_char = false;
                                break;

                            default:
                                if ((c & 0x80) != 0) {
                                    cout << "export_text unknown high ascii " << hex << setw(2) << uppercase << setfill('0') <<
                                    (unsigned int)(unsigned char)c << endl;
                                }

                                line.append((char *)&c, 1);
                                if (c == '<') within_tag = true;
                                if (!within_tag) {
                                    if (grid.grid_type == Grid::TEXT) {
                                        non_tag_line_length_text++;

                                    } else if (grid.grid_type == Grid::FOOTNOTE) {
                                        non_tag_line_length_footnote++;
                                    }
                                }
                                
                                if (!within_tag) previous_whitespace_char = c == ' ';
                                if (c == '>') within_tag = false;
                                break;
                        }
                    }
                }
            }
        }
    }

    if (to_html) {
        if (!first_paragraph) oss_text << "</p>" << endl;

        oss_text << endl;
        oss_text << "</body>" << endl;
        oss_text << "</html>" << endl;
    }

    stringToFile(oss_text.str(), to_html ?
                 "/Users/michael/Documents/Seneca/cu31924026554281_cubook/file.html" :
                 "/Users/michael/Documents/Seneca/cu31924026554281_cubook/file.txt");
}

void Fl_Main_Window::undo()
{
    Fl_Widget *widget = Fl::focus();
    bool found = widget == page_input;
    for (size_t k = 0; k < inputs->size() && !found; k++) {
        found = widget == inputs->at(k);
    }

    if (found) {
        ((Fl_Input *)widget)->undo();
    }
}

void Fl_Main_Window::cut()
{
    Fl_Widget *widget = Fl::focus();
    bool found = widget == page_input;
    for (size_t k = 0; k < inputs->size() && !found; k++) {
        found = widget == inputs->at(k);
    }

    if (found) {
        ((Fl_Input *)widget)->copy(1);
        ((Fl_Input *)widget)->cut();
    }
}

void Fl_Main_Window::copy()
{
    Fl_Widget *widget = Fl::focus();
    bool found = widget == page_input;
    for (size_t k = 0; k < inputs->size() && !found; k++) {
        found = widget == inputs->at(k);
    }

    if (found) {
        ((Fl_Input *)widget)->copy(1);
    }
}

void Fl_Main_Window::paste()
{
    Fl::paste(*page_input, 1);
}

void Fl_Main_Window::delete_selection()
{
    Fl_Widget *widget = Fl::focus();
    bool found = widget == page_input;
    for (size_t k = 0; k < inputs->size() && !found; k++) {
        found = widget == inputs->at(k);
    }

    if (found) {
        ((Fl_Input *)widget)->cut();
    }
}

void Fl_Main_Window::set_even_default_grids()
{
    copy_to_default_even_grids();
}

void Fl_Main_Window::set_odd_default_grids()
{
    copy_to_default_odd_grids();
}


// =================================================================================================

void main_window_cb(Fl_Widget* widget, void *userp)
{
//    cout << "Window Close!" << endl;

    Fl_Main_Window *window = (Fl_Main_Window *)widget;

    int currentPage = window->getCurrentPage();
    if (currentPage > 0) leaving_page(currentPage);

    close_grids_db();
    
    window->hide();
}

void merged_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->merged();
}

void split_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->split();
}

void new_grid_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->new_grid();
}

void header_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->header_grid();
}

void text_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->text_grid();
}

void footnote_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->footnote_grid();
}

void footer_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->footer_grid();
}

void delete_grid_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->delete_grid();
}

void page_input_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->page_input_changed();
}

void edited_page_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->edited_page();
}

void go_page_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->go_page();
}

void prev_page_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->prev_page();
}

void next_page_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->next_page();
}

void font_bigger_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->font_bigger();
}

void font_smaller_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->font_smaller();
}

void input_narrower_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->input_narrower();
}

void input_wider_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->input_wider();
}

void insert_word_break_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->insert_word_break();
}

void insert_emdash_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->insert_emdash();
}

void insert_section_break_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->insert_section_break();
}

void insert_paragraph_break_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->insert_paragraph_break();
}

void insert_line_break_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->insert_line_break();
}

void save_page_button_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->save_page();
}

void resizingTimer(void *data)
{
    Fl_Main_Window *mainWindow = (Fl_Main_Window *)data;
    mainWindow->finishResizing();
}

void export_text_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->export_text(false);
}

void export_html_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->export_text(true);

}

void undo_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->undo();
}

void cut_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->cut();
}

void copy_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->copy();
}

void paste_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->paste();
}

void delete_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->delete_selection();
}

void set_even_default_grids_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->set_even_default_grids();
}

void set_odd_default_grids_cb(Fl_Widget* widget, void *userp)
{
    ((Fl_Main_Window *)userp)->set_odd_default_grids();
}

