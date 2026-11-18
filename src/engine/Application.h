#pragma once

#include "Core.h"
#include <memory>

namespace Povly {
    class POVLY_API Application {
        public:
            Application();
            virtual ~Application();

            void Run();
    };

    std::unique_ptr<Application> CreateApplication();
}