# static и синглтон

## Статический метод

```cpp
// Event.h, разворачивается макросом EVENT_CLASS_TYPE(WindowResize):
static EventType GetStaticType() { return EventType::WindowResize; }
```

- Вызывается **без объекта**, через класс: `WindowResizeEvent::GetStaticType()`.
- Нет `this` (объекта-то нет) → видит только статические поля и свои аргументы.
- Парочка `GetStaticType()` (static) / `GetEventType()` (virtual) — приём:
  первый отвечает «кто ты» на уровне класса (нужен диспетчеру для сравнения),
  второй — на уровне конкретного объекта.

## Статическое поле

`src/engine/Log.h` и `Log.cpp`:

```cpp
// Log.h — объявление (что такое поле есть)
class POVLY_API Log {
  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// Log.cpp — определение (сам объект, один на всю программу)
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
```

- Статическое поле существует **в одном экземпляре на весь класс**, а не в каждом
  объекте (объектов Log вообще не создают).
- Определение обязано жить в .cpp — иначе «multiple definition» при включении
  заголовка в несколько файлов.

## Синглтон через static

`Log` — класс, у которого всё статическое: один логгер на всю программу,
никаких `Log l;` не создаётся. Инициализация — один раз в начале:

```cpp
// EntryPoint.h
Povly::Log::Init();

// Log.cpp
void Log::Init() {
  spdlog::set_pattern("%^[%T] %n: %v%$");
  s_CoreLogger  = spdlog::stdout_color_mt("Povly");
  s_ClientLogger = spdlog::stdout_color_mt("APP");
}
```

Это упрощённый синглтон (без защиты от «забыли вызвать Init» — полноценный
делают через static-локальную переменную в функции-геттере; в движке так
позволительно: Init зовётся первым делом в main).

## Статическая локальная переменная (для справки)

```cpp
Logger& Get() {
  static Logger instance;   // создаётся при первом вызове, живёт до конца программы
  return instance;
}
```

Так делают классические синглтоны — в этом проекте не используется, но
встретится в других библиотеках (и в туториалах, по которым сделан движок).
