//
//  image_box.hpp
//  proofr
//
//  Created by michael on 11/11/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

#ifndef image_box_hpp
#define image_box_hpp

#include <FL/Fl_Box.H>
#include <FL/Fl_RGB_Image.H>

class Fl_Main_Window;

class Fl_Image_Box : public Fl_Box
{
public:
    const static int GRID_CONTROL_SIZE = 16;

    Fl_Image_Box(Fl_Main_Window *main_window, Fl_Boxtype boxtype, int x, int y, int width, int height, const char *label);

    virtual ~Fl_Image_Box();

    virtual void draw();
    virtual void resize(int x, int y, int width, int height);
    virtual int handle(int event);

protected:
    enum Control { NONE, DRAG, STRETCH, ADD } control = NONE;
    size_t drag_index;
    double drag_from_y;

    Fl_Main_Window *main_window = nullptr;
};

#endif /* image_box_hpp */
