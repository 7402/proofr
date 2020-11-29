//
//  split_box.cpp
//  proofr
//
//  Created by michael on 11/17/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//


#include <cmath>
#include <iostream>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "main_window.hpp"
#include "split_box.hpp"
#include "grids.hpp"

using namespace std;

Fl_Split_Box::Fl_Split_Box(Fl_Main_Window *main_window, Fl_Boxtype boxtype, int x, int y,
                           int width, int height, const char *label) :
    Fl_Box(boxtype, x, y, width, height, label)
{
    this->main_window = main_window;
}

Fl_Split_Box::~Fl_Split_Box()
{
}

void Fl_Split_Box::draw()
{
    Fl_Box::draw();

    Fl_Image *image = main_window->getPageImage();
    if (image != nullptr) {
        double yn = y();
        for (size_t k = 0; k < grids.size(); k++) {
            double yi = grids[k].y_rel * image->h();
            double row_height = grids[k].row_height_rel * image->h();
            for (int n = 0; n < grids[k].nrows; n++) {
                image->draw(x(), (int)round(yn), image->w(), row_height, 0, (int)round(yi));
                yi += row_height;
                yn += row_height + main_window->edit_space();
            }
        }
    }
}

void Fl_Split_Box::resize(int x, int y, int width, int height)
{
//    cout << "Fl_Split_Box::resize(" << x << ", " << y << ", " << width << ", " << height << ")" << endl;
    Fl_Box::resize(x, y, width, height);
}
