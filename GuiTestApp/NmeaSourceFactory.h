#include "NmeaSource.h"

#include <memory>

inline std::shared_ptr<INmeaSource> CreateNmeaSource() {
    return std::make_shared<NmeaSource>();
}

inline NmeaSource* CreateNmeaSource1() {
    return new NmeaSource();
}