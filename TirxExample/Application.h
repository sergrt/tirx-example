#pragma once
#include <NsApp/Application.h>

namespace TirxExample {
    class App final : public NoesisApp::Application {
    public:
        App() {
            
        }
    private:
        NS_IMPLEMENT_INLINE_REFLECTION_(App, Application, "TirxExample.App")
    };
}
