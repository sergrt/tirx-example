#pragma once

#include "NmeaSource.h"

inline NmeaSource* CreateNmeaSource() {
    return new NmeaSource();
}