# Как запускается движок

Файлы: `src/Povly.h`, `src/engine/EntryPoint.h`, `src/engine/Application.h`,
`src/sandbox/SandboxApp.cpp`.

## Цепочка запуска

```
Sandbox (игра)                          Движок (libPovly.so)
────────────────                        ────────────────────
SandboxApp.cpp:
  #include <Povly.h>                    Povly.h — «зонтичный» заголовок:
      │                                 подключает Application, Log, EntryPoint
      ▼
  class Sandbox : public Application    EntryPoint.h — здесь живёт main():
  CreateApplication() {                 ┌─────────────────────────────────┐
    return make_unique<Sandbox>();  ───▶│ int main() {                    │
  }                                     │   Povly::Log::Init();           │
                                        │   POVLY_CORE_WARN("Init Log!"); │
                                        │   auto app = CreateApplication();│
                                        │   app->Run();                   │
                                        │   app.reset();                  │
                                        │ }                               │
                                        └─────────────────────────────────┘
```

## Идея: main() принадлежит движку, а не игре

Обычная программа: свой main в каждом проекте. В движке — наоборот: **main
написан внутри движка**, а игра лишь отдаёт движку свой класс приложения.

Зачем:

- движок сам решает, что и в каком порядке делать при старте (логгер, окна,
  буферы — позже);
- `Run()` — «главный цикл» (game loop): каждый кадр — опрос событий, обновление,
  рендер. Сейчас в нём тестовая заготовка:

```cpp
// Application.cpp
void Application::Run() {
  WindowResizeEvent e(1280, 720);
  POVLY_CORE_TRACE(e);
  while (true);   // временно: пустой бесконечный цикл — цикл кадров
}
```

## Фабрика CreateApplication

```cpp
// Application.h (движок) — обещание: «кто-то снаружи даст мне Application»
std::unique_ptr<Application> CreateApplication();

// EntryPoint.h — напоминание для линкера (extern: «определено в другом файле»)
extern std::unique_ptr<Povly::Application> Povly::CreateApplication();

// SandboxApp.cpp (игра) — выполнение обещания
std::unique_ptr<Povly::Application> Povly::CreateApplication() {
  return std::make_unique<Sandbox>();
}
```

Это **фабричная функция**: движок не знает, какой класс у игры, — он знает
только интерфейс `Application` и просит «дай мне приложение». Игра в ответ
создаёт своего наследника.

Тип возврата — `unique_ptr` — сам документирует договор: «создал — и всё,
владение твоё, движок».

## Зонтичный заголовок

```cpp
// Povly.h — весь публичный интерфейс одним include
#include "engine/Application.h"
#include "engine/Log.h"
#include "engine/EntryPoint.h"
```

Игре достаточно `#include <Povly.h>`. Когда интерфейс вырастет — окна, ввод,
рендер — всё добавится сюда, и у пользователей движка ничего не поменяется.

## app.reset() в конце main

`app` — unique_ptr. `reset()` удаляет приложение **явно, до** выхода из main:
все деструкторы игры отработают ещё пока программа «жива» (позже это важно:
графические контексты надо уничтожать до выгрузки библиотек).
