#include "ObjectsRegistry.h"

#include "NmeaSourceFactory.h"
#include "LoggerFactory.h"
#include "NmeaProcessorFactory.h"

ObjectsRegistry::ObjectsRegistry() {
    auto nmea_source_ptr = std::shared_ptr<ILoadable>(CreateNmeaSource1());
    //registry_["NmeaSource"] = nmea_source_ptr;



    
    
    //registry_["NmeaSource"] = nmea_source_ptr;//nmea_source_ptr;//std::static_pointer_cast<INmeaSource>(nmea_source_ptr);
    registry_["Logger"] = std::shared_ptr<ILoadable>(CreateLogger1());//std::shared_ptr<ILogger>(CreateLogger());
    registry_["NmeaProcessor"] = std::shared_ptr<ILoadable>(CreateNmeaProcessor1());//std::shared_ptr<ILogger>(CreateLogger());
    registry_["NmeaSource"] = nmea_source_ptr;

    for (auto& i : registry_) {
        i.second->initialize();
    }
}


ObjectsRegistry::~ObjectsRegistry() {
    for (auto& i : registry_) {
        i.second->uninitialize();
    }
}

