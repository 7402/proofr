//
//  image_box.cpp
//  proofr
//
//  Created by michael on 11/11/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

#include <cmath>
#include <iostream>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "image_box.hpp"
#include "grids.hpp"
#include "main_window.hpp"

using namespace std;

Fl_Image_Box::Fl_Image_Box(Fl_Main_Window *main_window, Fl_Boxtype boxtype, int x, int y,
                           int width, int height, const char *label) :
    Fl_Box(boxtype, x, y, width, height, label)
{
    this->main_window = main_window;
}

Fl_Image_Box::~Fl_Image_Box()
{
}

void Fl_Image_Box::draw()
{
    Fl_Box::draw();

    Fl_Image *image = main_window->getPageImage();

    if (image != nullptr) {
        image->draw(x(), y(), image->w(), image->h(), 0, 0);
    }

    double pageHeight = image->h();

    for(size_t k = 0; k < grids.size(); k++) {
        Grid grid = grids[k];
        double y0 = y() + pageHeight * grid.y_rel;
        double y1 = y0 + grids[k].nrows * grid.row_height_rel * pageHeight;
        double x0 = x() + 1;
        double x1 = x0 + image->w() - 3;

        switch (grid.grid_type) {
            case Grid::HEADER:      fl_color(fl_rgb_color(0, 0, 255)); break;
            case Grid::TEXT:        fl_color(fl_rgb_color(255, 0, 0)); break;
            case Grid::FOOTNOTE:    fl_color(fl_rgb_color(0, 128, 0)); break;
            case Grid::FOOTER:      fl_color(fl_rgb_color(0, 0, 255)); break;
        }

        fl_line(x0, y0, x1, y0);
        fl_line(x0, y0, x0, y1);
        fl_line(x1, y0, x1, y1);

        for (int i = 1; i <= grid.nrows; i++) {
            double y = y0 + i * grid.row_height_rel * pageHeight;
            fl_line(x0, y, x1, y);
        }

        // drag control
        fl_rect(x0 + GRID_CONTROL_SIZE, y0, GRID_CONTROL_SIZE, GRID_CONTROL_SIZE);

        // stretch control
        fl_rect(x0, y1 - GRID_CONTROL_SIZE + 1, GRID_CONTROL_SIZE, GRID_CONTROL_SIZE);

        // add control
        fl_rect(x1 - GRID_CONTROL_SIZE + 1, y1 - GRID_CONTROL_SIZE + 1, GRID_CONTROL_SIZE, GRID_CONTROL_SIZE);
    }
}

void Fl_Image_Box::resize(int x, int y, int width, int height)
{
    Fl_Box::resize(x, y, width, height);
}

int Fl_Image_Box::handle(int event)
{
    switch(event) {
        case FL_PUSH:
        {
            double xm = Fl::event_x();
            double ym = Fl::event_y();

            drag_from_y = ym;

            bool found = false;
            Fl_Image *image = main_window->getPageImage();
            double pageHeight = image->h();

            for (size_t k = 0; k < grids.size() && !found; k++) {
                Grid grid = grids[k];
                double y0 = y() + pageHeight * grid.y_rel;
                double y1 = y0 + grids[k].nrows * grid.row_height_rel * pageHeight;
                double x0 = x() + 1;
                double x1 = x0 + image->w() - 3;

                double x_drag = x0 + GRID_CONTROL_SIZE;
                double y_drag = y0;
                double x_stretch = x0;
                double y_stretch = y1 - GRID_CONTROL_SIZE + 1;
                double x_add = x1 - GRID_CONTROL_SIZE + 1;
                double y_add = y1 - GRID_CONTROL_SIZE + 1;

                if (xm >= x_drag && xm < x_drag + GRID_CONTROL_SIZE &&
                        ym >= y_drag && ym < y_drag + GRID_CONTROL_SIZE) {
//                    cout << "DRAG" << endl;
                    control = DRAG;
                    found = true;

                } else if (xm >= x_stretch && xm < x_stretch + GRID_CONTROL_SIZE &&
                           ym >= y_stretch && ym < y_stretch + GRID_CONTROL_SIZE) {
//                    cout << "STRETCH" << endl;
                    control = STRETCH;
                    found = true;

                } else if (xm >= x_add && xm < x_add + GRID_CONTROL_SIZE &&
                           ym >= y_add && ym < y_add + GRID_CONTROL_SIZE) {
//                    cout << "ADD" << endl;
                    control = ADD;
                    found = true;

                } else {
                    control = NONE;
                }

                drag_index = k;
            }

        }
            return 1;
        case FL_DRAG:
        {
            double ym = Fl::event_y();

            if (ym != drag_from_y && control != NONE) {
                Fl_Image *image = main_window->getPageImage();
                double pageHeight = image->h();
                double delta = (ym - drag_from_y) / pageHeight;
                Grid grid = grids[drag_index];

                double bottom_rel = grid.y_rel + grid.nrows * grid.row_height_rel;
                double new_bottom_rel = bottom_rel + delta;

                switch (control) {
                    case DRAG:
                        grid.y_rel += delta;
                        drag_from_y = ym;
                       break;
                    case STRETCH:
                        if (new_bottom_rel > grid.y_rel) {
                            grid.row_height_rel *= (new_bottom_rel - grid.y_rel) / (bottom_rel - grid.y_rel);
                            drag_from_y = ym;
                        }
                        break;
                    case ADD:
                        if (new_bottom_rel > grid.y_rel) {
                            int new_nrows = ceil(((ym - y()) / pageHeight - grid.y_rel) / grid.row_height_rel);
                            if (grid.nrows != new_nrows && new_nrows > 0) {
                                grid.nrows = new_nrows;
                                drag_from_y = ym;
                            }
                        }
                       break;
                    default:
                        break;
                }

                grids[drag_index] = grid;
                redraw();
            }

        }
            return 1;
        case FL_RELEASE:
        {
            if (control != NONE) main_window->adjustSplitHeight();
            
            control = NONE;
        }
            return 1;
        default:
            return Fl_Box::handle(event);
    }
}

// fl_draw_image
