//
//  split_box.hpp
//  proofr
//
//  Created by michael on 11/17/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

#ifndef split_box_hpp
#define split_box_hpp

#include <FL/Fl_Box.H>
#include <FL/Fl_RGB_Image.H>

class Fl_Main_Window;

class Fl_Split_Box : public Fl_Box
{
public:
    Fl_Split_Box(Fl_Main_Window *main_window, Fl_Boxtype boxtype, int x, int y, int width, int height, const char *label);

    virtual ~Fl_Split_Box();

    virtual void draw();
    virtual void resize(int x, int y, int width, int height);

protected:
    Fl_Main_Window *main_window = nullptr;
};

#endif /* split_box_hpp */
