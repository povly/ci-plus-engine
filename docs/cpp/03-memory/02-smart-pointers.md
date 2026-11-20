# Умные указатели: unique_ptr и shared_ptr

Проблема сырых указателей — помнить `delete`:

```cpp
Application* app = new Application();
app->Run();
delete app;                    // забыли → утечка памяти
                               // исключение до delete → утечка
```

Умный указатель — обёртка, которая удаляет объект сама.

## unique_ptr — один владелец

```cpp
// src/engine/EntryPoint.h
auto app = Povly::CreateApplication();   // std::unique_ptr<Application>
app->Run();
app.reset();                             // удалить объект прямо сейчас
// а чаще просто: выходит из области видимости — объект удалился сам
```

- Объектом владеет **ровно один** unique_ptr. Копировать нельзя, можно только
  передать (move) — владение «переезжает».

## shared_ptr — совместное владение

```cpp
// src/engine/Log.h
static std::shared_ptr<spdlog::logger> s_CoreLogger;

// Log.cpp
s_CoreLogger = spdlog::stdout_color_mt("Povly");
```

- Внутри счётчик ссылок: сколько shared_ptr смотрят на объект. Стал ноль —
  объект удаляется. «Последний выключает свет».
- `GetCoreLogger()` возвращает `std::shared_ptr<spdlog::logger>&` — ссылку на
  сам указатель, чтобы каждый вызов лога не трогал счётчик (лишняя работа).

## make_unique / make_shared — создание

```cpp
// src/sandbox/SandboxApp.cpp
std::unique_ptr<Povly::Application> Povly::CreateApplication() {
  return std::make_unique<Sandbox>();
}
```

Вместо `std::unique_ptr<Application>(new Sandbox())`. Короче, безопаснее
(нет случайного `new` без владельца) и для shared_ptr — одним выделением памяти.

## Как это используется в движке

```cpp
// Application.h — фабрика: игра возвращает движку своё приложение
std::unique_ptr<Application> CreateApplication();
```

Смысл типов честно виден в сигнатуре:
- возвращается **unique_ptr** → «создал — и передал владение целиком тебе»;
- где-нибудь был бы **shared_ptr** → «владение общее, следи сам»;
- сырой `T*` → «объект не мой, я его только читаю».

Сравнение с `new/delete` — в `07-old-vs-new/README.md`.
