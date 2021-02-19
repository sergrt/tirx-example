#pragma once
#include "Logger.h"

inline Logger* CreateLogger() {
    return new Logger();
}
