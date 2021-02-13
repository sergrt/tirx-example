#include "MainWindow.h"

#include "NmeaProcessor.h"

#include <NsGui/ListBox.h>
#include <NoesisPCH.h>

#include "RunLoop.h"

#include "Common.h"
#include "ILogger.h"

#include <sstream>

int int_value = 100;

namespace RssReader {


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

        skin_combo_box_->SetSelectedIndex(0);

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
        auto x =
            (ComboBoxItem*)skin_combo_box_->GetSelectedItem();

        const std::string name = x->GetContent()->ToString().Str();
        Color bg_color;
        if (name == "Light") {
            bg_color = Color(200, 220, 200);
        } else {
            bg_color = Color(100, 120, 100);
        }
        for (auto& s : {"TopBarBg", "RightBarBg", "BottomBarBg"}) {
            auto tb = FindName<Rectangle>(s);
            tb->SetFill(new SolidColorBrush(bg_color));
        }

        auto theme = GetTheme(name.c_str() , "Green");
        ResourceDictionary* resources = this->GetResources();
        resources->GetMergedDictionaries()->Clear();
        resources->GetMergedDictionaries()->Add(theme);
        
    }
    void MainWindow::OnSubscribeClicked(BaseComponent* /*sender*/, const RoutedEventArgs& /*e*/) {
        auto logger = registry.findObject<ILogger>("Logger");
        std::cout << "--\n";
        logger_subscription_ = logger->add_data_source(&log_data_source_);
        mouse_pos_subscription_ = mouse_pos_observable_.subscribe([this](int x, int y) {
            std::string str = "x = " + std::to_string(x) + ", y = " + std::to_string(y);
            mouse_pos_label_->SetContent(str.c_str());
        });
        
        /*
        static int i = 0;
        ui_logs_observable_.get_subscriber().on_next("OnGoToClicked()");
        observable_.next("Observable value = " + std::to_string(i));
        
        int_observable_.next(int_value, i);
        ++int_value;

        if (i == 9) {
            i = 0;
            int_observable_.error("overflow");
        } else {
            ++i;
        }
            */
        /*
        auto x = std::this_thread::get_id();
        std::cout << "click thread " << x << "\n";
        const std::string content = "Dummy";
        if (auto list_box = FindName<ListBox>("listBox")) {
            auto item = new ListBoxItem();
            item->SetContent(content.c_str());
            if (auto items = list_box->GetItems()) {
                items->Insert(0, item);
                //if (items->Count() > 0)
                //    items->RemoveAt(1);
            }
        }
        */
    }

    void MainWindow::OnUnsubscribeClicked(BaseComponent* /*sender*/, const RoutedEventArgs& /*e*/) {
        auto logger = registry.findObject<ILogger>("Logger");

        //logger->RemoveSource(&log_observable_);
        logger_subscription_.unsubscribe();
        mouse_pos_subscription_.unsubscribe();

        
        //logger->AddObservableSubscription(&observable_);
        //logger->AddObservableSubscription(&int_observable_);
    }

    void MainWindow::OnSendLogEventClicked(BaseComponent* /*sender*/, const RoutedEventArgs& /*e*/) {
        static int log_event_id = 0;
        const std::string log_text = "Log event #" + std::to_string(log_event_id++);
        log_data_source_.next(log_text);
    }
}
