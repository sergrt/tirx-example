#include "MainWindow.h"

#include "NmeaProcessor.h"
#include "Common.h"
#include "ILogger.h"

#include <NsGui/ListBox.h>
#include <NoesisPCH.h>

#include <sstream>


namespace {
    const Color light_bg_color(200, 220, 200);
    const Color dark_bg_color(100, 120, 100);
}

namespace TirxExample {

ResourceDictionary* MainWindow::GetTheme(const char* color, const char* accent) {
    String themeName(String::VarArgs(), "Theme/NoesisTheme.Brushes.%s%s.xaml", color, accent);
    ThemeDictionaries::Iterator it = mThemeDictionaries.Find(themeName);
    if (it != mThemeDictionaries.End()) {
        return it->value;
    }

    Ptr<ResourceDictionary> theme = GUI::LoadXaml<ResourceDictionary>(themeName.Str());
    mThemeDictionaries.Insert(themeName, theme);
    return theme;
}

MainWindow::MainWindow() {
    InitializeComponent();
}

void MainWindow::InitializeComponent() {
    Noesis::GUI::LoadComponent(this, "MainWindow.xaml");

    nmea_monitor_ = FindName<ListBox>("NmeaMonitor");
    gps_data_monitor_ = FindName<ListBox>("GpsDataMonitor");
    mouse_pos_label_ = FindName<Label>("MousePosLabel");
    background_image_ = FindName<Image>("BackgroundImage");

    auto logger = registry.findObject<ILogger>("Logger");
    //logger->AddSource(ui_logs_observable_);
    nmea_processor_ = findObject<INmeaProcessor>("NmeaProcessor");

    background_image_->MouseMove() += [this](BaseComponent* sender, const MouseEventArgs& args) {
        mouse_pos_observable_.next(static_cast<int>(args.position.x), static_cast<int>(args.position.y));
    };

    skin_combo_box_ = FindName<ComboBox>("comboBox");
    skin_combo_box_->SelectionChanged() += [this](BaseComponent* sender, const SelectionChangedEventArgs& e) {
        OnSkinSelected(sender, e);
    };

    skin_combo_box_->SetSelectedIndex(0);

    if (!nmea_processor_)
        return;

    auto observable = nmea_processor_->get_pos_data_stream();
    gps_subscription_ = observable->subscribe([this](std::pair<double, std::string> lat, std::pair<double, std::string> lon) {
        std::stringstream sstream;
        sstream << lat.first << lat.second << ", " << lon.first << lon.second;
        const std::string content = sstream.str();
        auto item = new ListBoxItem();
        item->SetContent(content.c_str());
        if (auto items = gps_data_monitor_->GetItems()) {
            items->Insert(0, item);
        }
    });

    nmea_subscription_ = nmea_processor_->get_sentences_stream()->subscribe([this](std::string sentence) {
        auto item = new ListBoxItem();
        item->SetContent(sentence.c_str());
        if (auto items = nmea_monitor_->GetItems()) {
            items->Insert(0, item);
        }
    });
}

MainWindow::~MainWindow() {
    std::cout << "~MainWindow()\n";
    logger_subscription_.unsubscribe();
    mouse_pos_subscription_.unsubscribe();
    nmea_subscription_.unsubscribe();
    gps_subscription_.unsubscribe();
}

bool MainWindow::ConnectEvent(BaseComponent* source, const char* event, const char* handler) {
    NS_CONNECT_EVENT(Noesis::Button, Click, OnSubscribeClicked);
    NS_CONNECT_EVENT(Noesis::Button, Click, OnUnsubscribeClicked);
    NS_CONNECT_EVENT(Noesis::Button, Click, OnSendLogEventClicked);

    return false;
}

void MainWindow::OnSkinSelected(BaseComponent* /*sender*/, const SelectionChangedEventArgs& /*e*/) {
    auto* x =
        (ComboBoxItem*)skin_combo_box_->GetSelectedItem();

    const std::string name = x->GetContent()->ToString().Str();
    Color bg_color = name == "Light" ? light_bg_color : dark_bg_color;
    for (auto& s : { "TopBarBg", "RightBarBg", "BottomBarBg" }) {
        auto tb = FindName<Rectangle>(s);
        tb->SetFill(new SolidColorBrush(bg_color));
    }

    auto* theme = GetTheme(name.c_str(), "Green");
    ResourceDictionary* resources = this->GetResources();
    resources->GetMergedDictionaries()->Clear();
    resources->GetMergedDictionaries()->Add(theme);

}

void MainWindow::OnSubscribeClicked(BaseComponent* /*sender*/, const RoutedEventArgs& /*e*/) {
    auto logger = registry.findObject<ILogger>("Logger");
    logger_subscription_ = logger->add_data_source(&log_data_source_);

    mouse_pos_subscription_ = mouse_pos_observable_.subscribe([this](int x, int y) {
        const std::string str = "x = " + std::to_string(x) + ", y = " + std::to_string(y);
        mouse_pos_label_->SetContent(str.c_str());
    });
}

void MainWindow::OnUnsubscribeClicked(BaseComponent* /*sender*/, const RoutedEventArgs& /*e*/) {
    logger_subscription_.unsubscribe();
    mouse_pos_subscription_.unsubscribe();
}

void MainWindow::OnSendLogEventClicked(BaseComponent* /*sender*/, const RoutedEventArgs& /*e*/) {
    static int log_event_id = 0;
    const std::string log_text = "Log event #" + std::to_string(log_event_id++);
    log_data_source_.next(log_text);
}

}
