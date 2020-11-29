//
//  main.cpp
//  proofr
//
//  Created by michael on 11/11/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

//    https://www.fltk.org/software.php
//
//    cd ~/Downloads/fltk-1.3.5
//    ./configure --enable-localzlib
//    make
//    test/demo
//    sudo make install

//    GraphicsMagick http://www.graphicsmagick.org https://sourceforge.net/projects/graphicsmagick/files/
//    JasPer for JPEG-2000    https://www.ece.uvic.ca/~frodo/jasper/#download
//
//    cd ~/Downloads
//    mkdir build
//    cmake -G "Unix Makefiles" -H~/Downloads/jasper-2.0.14 -B~/Downloads/build \
//      -DGLUT_glut_LIBRARY=/System/Library/Frameworks/GLUT.framework
//    cd build
//    make clean all
//    make test
//    sudo make install
//
//    cd ~/Downloads/GraphicsMagick-1.3.35
//    make clean
//    ./configure
//    make
//    sudo make install

//
//    Targets > Build Settings: Search Paths
//      Header Search Paths: /usr/local/include
//      Library Search Paths: /usr/local/lib
//
//    Targets > Build Settings: Warnings - All languages
//      Documentation Comments: No
//
//    Targets > Build Phases: Link Binary With Libraries
//      Cocoa.framework
//      libfltk.a    [/usr/local/lib/libftlk.a]

#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_Sys_Menu_Bar.H>

#include "grids.hpp"
#include "main_window.hpp"

using namespace std;

void close_cb(Fl_Widget *widget, void *userp);

int systemHandler(void *event, void *data);

int main(int argc, const char * argv[]) {
    // initial lock of main thread https://www.fltk.org/doc-1.3/advanced.html
    Fl::lock();

    open_grids_db();

    Fl::option(Fl::OPTION_VISIBLE_FOCUS, false);

    Fl::scheme("gtk+");

    Fl_Sys_Menu_Bar *sys_menu_bar = new Fl_Sys_Menu_Bar(0, 0, 0, 0);
    sys_menu_bar->add("File/Close", FL_COMMAND + 'w', close_cb);

    Fl::visual(FL_DOUBLE | FL_INDEX);

    new Fl_Main_Window(100, 100, 700, 500, "proofr");

    Fl::run();

    close_grids_db();

    return 0;
}

void close_cb(Fl_Widget *widget, void *userp)
{
    cout << "Menu Close!" << endl;

    Fl_Window * window = Fl_Window::current();
    if (window != nullptr) {
        (window->callback())(window, nullptr);
    }
}

