//
//  utils.hpp
//  proofr
//
//  Created by michael on 11/11/20.
//  Copyright © 2020 Michael Budiansky. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <string>
#include "sqlite3.h"

// trim leading and trailing spaces from string
std::string trim(std::string& str);

// write string to file
void stringToFile(const std::string& str, const std::string& path);

// can file be read
bool fileExists(const std::string& path);

// read string from file
bool fileToString(const std::string& path, std::string& str);

// read next line from stream, return false when no more
bool nextLine(std::istream& is, std::string& line);

bool doSql(sqlite3 *db, const std::string& sql, bool logFail = true);

#endif /* utils_hpp */
