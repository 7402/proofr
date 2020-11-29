//
//  utils.cpp
//  proofr
//
//  Created by michael on 11/11/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

#include <fstream>  // must precede .h includes

#include "utils.hpp"

#include <iostream>
#include <sstream>
//#include <stdexcept>
//#include <unistd.h>
//#include <vector>

using namespace std;

// can file be read
bool fileExists(const std::string& path)
{
    ifstream ifs(path.c_str());
    bool ok = ifs.is_open();

    if (ok) ifs.close();

    return ok;
}

// read string from file
bool fileToString(const std::string& path, std::string& str)
{
    ifstream ifs(path.c_str());
    bool ok = ifs.is_open();

    if (ok) {
        ostringstream oss;
        oss << ifs.rdbuf();

        str = oss.str();

        ifs.close();

    } else {
        str.clear();
    }

    return ok;
}

// write string to file
void stringToFile(const std::string& str, const std::string& path)
{
    ofstream ofs(path.c_str());

    ofs << str;

    ofs.close();
}

// read next line from stream, return false when no more
bool nextLine(std::istream& is, std::string& line)
{
    line.clear();

    while (line.length() == 0 && !is.eof()) {
        bool done = false;
        while (!done && !is.eof()) {
            char c = is.get();
            if (c == 0x0B) {
                done = true;
            } else {
                line.append(&c);
            }
        }
    }

    return line.length() > 0;
}

bool doSql(sqlite3 *db, const string& sql, bool logFail)
{
    //cout << "try: " << sql << endl;
    // TODO use codecvt_utf8

    bool success = false;

    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result == SQLITE_OK) {
        result = sqlite3_step(stmt);
        success = result == SQLITE_DONE || result == SQLITE_ROW;
    }

    sqlite3_finalize(stmt);

    if (!success && logFail) {
        cout << "feh: (" << result << ") " << sql << endl;
    }

    return success;
}

