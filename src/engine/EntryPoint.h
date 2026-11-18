#pragma once

#include "engine/Application.h"
#include <memory>

extern std::unique_ptr<Povly::Application> Povly::CreateApplication();

int main(int argc, char **argv) {
  auto app = Povly::CreateApplication();
  app->Run();
  app.reset();
}