#pragma once

#include "engine/Application.h"

extern Povly::Application *Povly::CreateApplication();

int main(int argc, char **argv) {
  auto app = Povly::CreateApplication();
  app->Run();
  delete app;
}
