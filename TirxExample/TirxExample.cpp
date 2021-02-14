#include <vld.h>
#include <NsApp/EntryPoint.h>

#include "Common.h"
#include "ILogger.h"

#include "AppLauncher.h"

int NsMain(int argc, char** argv) {
    auto logger = registry.findObject<ILogger>("Logger");
    logger->log_unsafe("--> Application started\n");
    AppLauncher launcher;
    launcher.SetArguments(argc, argv);
    launcher.SetApplicationFile("App.xaml");
    auto res = launcher.Run();
    return res;
}
