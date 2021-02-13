#pragma once
#include <NsApp/Application.h>
#include <NsApp/ApplicationLauncher.h>
#include <NsApp/EmbeddedXamlProvider.h>
//#include <NsApp/EmbeddedFontProvider.h>
#include <NsCore/RegisterComponent.h>

#include "Application.h"
#include "App.xaml.bin.h"
#include "MainWindow.h"
#include "MainWindow.xaml.bin.h"

using namespace Noesis;
using namespace NoesisApp;

class AppLauncher final : public ApplicationLauncher {
private:
    void RegisterComponents() const override {
        RegisterComponent<RssReader::App>();
        RegisterComponent<RssReader::MainWindow>();
    }

    Ptr<XamlProvider> GetXamlProvider() const override {
        EmbeddedXaml xamls[] =
        {
            { "App.xaml", App_xaml },
            { "MainWindow.xaml", MainWindow_xaml }
        };

        return *new EmbeddedXamlProvider(xamls);
    }
    /*
    Ptr<FontProvider> GetFontProvider() const override {
        EmbeddedFont fonts[] =
        {
            { "", Roboto_Regular_ttf },
            { "", Roboto_Bold_ttf }
        };

        return *new EmbeddedFontProvider(fonts);
    }
    */
};
