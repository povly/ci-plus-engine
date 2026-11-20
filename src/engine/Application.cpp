#include "Application.h"

#include "engine/Events/ApplicationEvent.h"
#include "engine/Log.h"

namespace Povly {
Application::Application() {}

Application::~Application() {}

void Application::Run() {
  WindowResizeEvent e(1280, 720);
  POVLY_CORE_TRACE(e);
  while (true);
}
}  // namespace Povly
