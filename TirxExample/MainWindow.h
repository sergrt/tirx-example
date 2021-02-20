#pragma once

#include "INmeaProcessor.h"

#include <NsApp/Window.h>
#include <NsGui/Button.h>
#include <NsGui/TextBlock.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/ComboBox.h>

#include "tirx.h"

#include <thread>

namespace Noesis {
    class Image;
    class Label;
    class ListBox;
}

using namespace Noesis;
using namespace NoesisApp;

namespace TirxExample {
    class MainWindow final : public ::Window {
    public:
        MainWindow();
        ~MainWindow();

    private:
        void InitializeComponent();

        bool ConnectEvent(BaseComponent* source, const char* event, const char* handler) override;

        void OnSubscribeClicked(BaseComponent* sender, const RoutedEventArgs& e);
        void OnUnsubscribeClicked(BaseComponent* sender, const RoutedEventArgs& e);
        void OnSendLogEventClicked(BaseComponent* sender, const RoutedEventArgs& e);
        void OnSkinSelected(BaseComponent* sender, const SelectionChangedEventArgs& e);

    private:
        ListBox* nmea_monitor_;
        ListBox* gps_data_monitor_;
        Label* mouse_pos_label_;
        Image* background_image_;
        ComboBox* skin_combo_box_;

        typedef Noesis::HashMap<Noesis::String, Noesis::Ptr<Noesis::ResourceDictionary>>
            ThemeDictionaries;

        ThemeDictionaries mThemeDictionaries;
        ResourceDictionary* GetTheme(const char* color, const char* accent);


        std::shared_ptr<INmeaProcessor> nmea_processor_;

        tirx::Observable<std::string> log_data_source_;
        tirx::Subscription logger_subscription_;

        tirx::Observable<int, int> mouse_pos_observable_;
        tirx::Subscription mouse_pos_subscription_;

        tirx::Subscription nmea_subscription_;
        tirx::Subscription gps_subscription_;

        NS_IMPLEMENT_INLINE_REFLECTION_(MainWindow, Window, "TirxExample.MainWindow")
    };
}
