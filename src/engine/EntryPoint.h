#pragma once

#include <memory>

#include "engine/Application.h"
#include "engine/Log.h"

extern std::unique_ptr<Povly::Application> Povly::CreateApplication();

int main(int argc, char** argv) {
  Povly::Log::Init();
  POVLY_CORE_WARN("Initialized Log!");
  int a = 5;
  POVLY_CLIENT_INFO("Hello! {0}", a);

  auto app = Povly::CreateApplication();
  app->Run();
  app.reset();
}
