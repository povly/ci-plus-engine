#include "engine/Application.h"
#include <Povly.h>
#include <memory>

class Sandbox : public Povly::Application {
public:
  Sandbox() {}

  ~Sandbox() {}
};

std::unique_ptr<Povly::Application> Povly::CreateApplication() { 
    return std::make_unique<Sandbox>();
}
