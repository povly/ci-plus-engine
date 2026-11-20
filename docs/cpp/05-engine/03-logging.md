# Логгер

Файлы: `src/engine/Log.h`, `Log.cpp`, макросы, фикс форматтера.

## Устройство

Статический класс-синглтон `Log` с двумя логгерами spdlog:

```cpp
// Log.cpp
void Log::Init() {
  spdlog::set_pattern("%^[%T] %n: %v%$");   // формат строки: [время] имя: текст, %^...%$ = цвет

  s_CoreLogger = spdlog::stdout_color_mt("Povly");   // "_mt" = потокобезопасный
  s_CoreLogger->set_level(spdlog::level::trace);

  s_ClientLogger = spdlog::stdout_color_mt("APP");
  s_ClientLogger->set_level(spdlog::level::trace);
}
```

Два логгера — чтобы separating сообщения движка и игры:

```
[12:34:56] Povly: Initialized Log!      ← движок (POVLY_CORE_...)
[12:34:56] APP: Hello! 5                ← игра (POVLY_CLIENT_...)
```

## Макросы

```cpp
// Log.h
#define POVLY_CORE_TRACE(...) ::Povly::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define POVLY_CORE_INFO(...)  ...
#define POVLY_CORE_WARN(...)  ...
...
```

Зачем макрос, если можно звать `Log::GetCoreLogger()->trace(...)` напрямую:

- короче и видно уровень в имени (`POVLY_CORE_WARN` читается как предупреждение);
- в release-сборках такой макрос легко превращается в пустоту — все trace-логи
  исчезают **до компиляции**, ноль накладных расходов:

```cpp
#ifdef POVLY_RELEASE
  #define POVLY_CORE_TRACE(...)   // ничего не делает
#endif
```

`__VA_ARGS__` — «все аргументы, что передали в макрос»: `POVLY_CORE_INFO("x={0}", x)`.

## Уровни

`trace` → `debug` → `info` → `warn` → `error` → `critical` — от самого
детального к самому важному. `set_level(trace)` = «показывай всё».

## Как событие попадает в лог

Форматная строка логгера — это fmt: `POVLY_CORE_INFO("Hello! {0}", a)` —
`{0}` подставляется вместо `a`. fmt проверяет **на этапе компиляции**, что все
аргументы умеют печататься. Для `WindowResizeEvent` это умение добавлено
специализацией `fmt::formatter` (в `Event.h`): печатай наследников Event через
их `ToString()`. Без неё — та самая ошибка «unformattable», с которой всё
начиналось.
