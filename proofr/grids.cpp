//
//  grids.cpp
//  proofr
//
//  Created by michael on 11/16/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

#include <iostream>
#include <sstream>

#include "sqlite3.h"

#include "grids.hpp"
#include "utils.hpp"

std::vector<Grid> grids;

const int DEFAULT_ODD_PAGE = -1;
const int DEFAULT_EVEN_PAGE = -2;

std::vector<Grid> default_odd_grids;
std::vector<Grid> default_even_grids;

sqlite3 *db = nullptr;

using namespace std;


string to_grid_type_name(Grid::GridType grid_type)
{
    switch (grid_type) {
        case Grid::HEADER:      return "HEADER";
        case Grid::TEXT:        return "TEXT";
        case Grid::FOOTNOTE:    return "FOOTNOTE";
        case Grid::FOOTER:      return "FOOTER";
        default:                return "TEXT";
    }
}

Grid::GridType to_grid_type_enum(string name)
{
    if (name == "HEADER") return Grid::HEADER;
    else if (name == "TEXT") return Grid::TEXT;
    else if (name == "FOOTNOTE") return Grid::FOOTNOTE;
    else if (name == "FOOTER") return Grid::FOOTER;
    else return Grid::TEXT;
}

bool read_grids(int page_number, std::vector<Grid>& page_grids)
{
    page_grids.clear();

    ostringstream oss;
    oss << "SELECT * FROM grids WHERE page_number = " << page_number <<
    " ORDER BY y_rel;";

    string sql = oss.str();

//    cout << sql << endl;

    sqlite3_stmt *stmt;
    bool success = SQLITE_OK == sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    int result;
    while (success && SQLITE_ROW == (result = sqlite3_step(stmt))) {
        Grid next_grid;
        next_grid.y_rel = sqlite3_column_double(stmt, 1);
        next_grid.row_height_rel = sqlite3_column_double(stmt, 2);
        next_grid.nrows = sqlite3_column_int(stmt, 3);
        const char *name = (const char *)sqlite3_column_text(stmt, 4);
        next_grid.grid_type = to_grid_type_enum(name);

        page_grids.push_back(next_grid);
    }

//    cout << result << endl;

    sqlite3_finalize(stmt);

    return success;
}

bool write_grids(int page_number, const std::vector<Grid>& page_grids)
{
    ostringstream oss;
    oss << "DELETE FROM grids WHERE page_number = " << page_number << ";" ;

    string sql = oss.str();

//    cout << sql << endl;

    bool success = doSql(db, sql.c_str());

    for (size_t k = 0; k < page_grids.size() && success; k++) {
        Grid next_grid = page_grids.at(k);

        oss.str("");
        oss << "INSERT INTO grids VALUES (" << page_number << ", " <<
        next_grid.y_rel  << ", " << next_grid.row_height_rel  << ", " <<
        next_grid.nrows  << ", '" <<
        to_grid_type_name(next_grid.grid_type) << "');";

        sql = oss.str();

//        cout << sql << endl;

        success = doSql(db, sql.c_str());
    }

    return success;
}

void open_grids_db()
{
    bool success = SQLITE_OK ==
        sqlite3_open("/Users/michael/Documents/Seneca/cu31924026554281_cubook/grids.sql3", &db);

    success = doSql(db, "CREATE TABLE IF NOT EXISTS grids ("
                    "page_number INTEGER,"
                    "y_rel NUMBER,"
                    "row_height_rel NUMBER,"
                    "nrows INTEGER,"
                    "grid_type TEXT"
                    ");");

    if (success) {
        success = doSql(db, "CREATE INDEX IF NOT EXISTS grids_index ON grids (page_number, y_rel);");
    }

    read_grids(DEFAULT_ODD_PAGE, default_odd_grids);
    read_grids(DEFAULT_EVEN_PAGE, default_even_grids);
}

void close_grids_db()
{
    if (db != nullptr) {
        write_grids(DEFAULT_ODD_PAGE, default_odd_grids);
        write_grids(DEFAULT_EVEN_PAGE, default_even_grids);

        sqlite3_close(db);
        db = nullptr;
    }
}

void leaving_page(int page_number)
{
    write_grids(page_number, grids);
}

void entering_page(int page_number)
{
    read_grids(page_number, grids);

    if (grids.size() == 0 && page_number != 0) {
        grids = page_number % 2 == 0 ? default_even_grids : default_odd_grids;
    }
}

void copy_to_default_odd_grids()
{
    default_odd_grids = grids;
}

void copy_to_default_even_grids()
{
    default_even_grids = grids;
}

void newGrid(double pageHeight, Grid::GridType gridType)
{
    Grid nextGrid;

    nextGrid.grid_type = gridType;
    
    switch (gridType) {
        case Grid::HEADER:      nextGrid.nrows = 1; break;
        case Grid::TEXT:        nextGrid.nrows = 6; break;
        case Grid::FOOTNOTE:    nextGrid.nrows = 2; break;
        case Grid::FOOTER:      nextGrid.nrows = 1; break;
    }

    if (grids.size() == 0) {
        nextGrid.y_rel = 10.0 / pageHeight;
        nextGrid.row_height_rel = 20.0 / pageHeight;

    } else {
        Grid lastGrid = grids.at(grids.size() - 1);
        double bottom = lastGrid.y_rel + lastGrid.nrows * lastGrid.row_height_rel;

        nextGrid.y_rel = bottom + 10.0 / pageHeight;
        nextGrid.row_height_rel = lastGrid.row_height_rel;
    }

    grids.push_back(nextGrid);
}

void deleteLastGrid(void)
{
    if (grids.size() > 0) {
        grids.pop_back();
    }
}

//void drawGrid(int index, double x0, double y0, double pageWidth, double pageHeight)
//{
//
//}
//
//void drawGrids(double x0, double y0, double pageWidth, double pageHeight)
//{
//
//}

