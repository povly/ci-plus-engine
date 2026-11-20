#pragma once

#include <memory>

#include "Core.h"

namespace Povly {
class POVLY_API Application {
 public:
  Application();
  virtual ~Application();

  void Run();
};

std::unique_ptr<Application> CreateApplication();
}  // namespace Povly