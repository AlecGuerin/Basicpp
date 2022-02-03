#ifndef BASICPP_H
#define BASICPP_H

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

/**
 * @brief The DebugLevel enum id used to list the debuging info.
 */
enum DebugLevel{
    L_NONE= -1,
    L_CRITICAL,
    L_ERROR,
    L_WARNING,
    L_INFO,
    L_VERBOSE,
    L_DEBUG
};

/**
 * @brief DEAULT_ERROR_MSG Default message when an exception occured.
 */
const std::string DEAULT_ERROR_MSG = "An exception occured";


#endif // BASICPP_H
