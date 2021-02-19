#include "ObjectsRegistry.h"

#include "NmeaSourceFactory.h"
#include "LoggerFactory.h"
#include "NmeaProcessorFactory.h"

ObjectsRegistry::ObjectsRegistry() {
    registry_["Logger"] = std::shared_ptr<ILoadable>(CreateLogger());
    registry_["NmeaProcessor"] = std::shared_ptr<ILoadable>(CreateNmeaProcessor());
    registry_["NmeaSource"] = std::shared_ptr<ILoadable>(CreateNmeaSource());

    for (auto& i : registry_) {
        i.second->initialize();
    }
}

ObjectsRegistry::~ObjectsRegistry() {
    for (auto& i : registry_) {
        i.second->uninitialize();
    }
}
