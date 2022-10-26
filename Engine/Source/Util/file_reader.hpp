#pragma once

#include <string>
#include <sstream>
#include <fstream>


/**
 * @brief Gets the contents of the .json file
 *
 * @param path The path to the .json file
 *
 * @return Returns a string with all the components on an entity
 */
std::string GetFileContents(const std::string path);