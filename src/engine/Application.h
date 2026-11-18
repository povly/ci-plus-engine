#pragma once

#include "Core.h"

namespace Povly {
    class POVLY_API Application {
        public:
            Application();
            virtual ~Application();

            void Run();
    };

    Application* CreateApplication();
}