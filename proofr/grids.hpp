//
//  grids.hpp
//  proofr
//
//  Created by michael on 11/16/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

#ifndef grids_hpp
#define grids_hpp

#include <vector>

struct Grid {
    double y_rel;
    double row_height_rel;
    int nrows;
    enum GridType { HEADER, TEXT, FOOTNOTE, FOOTER } grid_type;
};
typedef struct Grid Grid;

extern std::vector<Grid> grids;

void open_grids_db();
void close_grids_db();

void copy_to_default_odd_grids();
void copy_to_default_even_grids();

void leaving_page(int page_number);
void entering_page(int page_number);

void newGrid(double pageHeight, Grid::GridType gridType);
void deleteLastGrid(void);

std::string to_grid_type_name(Grid::GridType grid_type);
Grid::GridType to_grid_type_enum(std::string name);
bool read_grids(int page_number, std::vector<Grid>& page_grids);
bool write_grids(int page_number, const std::vector<Grid>& page_grids);

#endif /* grids_hpp */
