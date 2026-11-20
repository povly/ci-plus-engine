# Как писали раньше и почему сейчас так

C++ живёт с 1980-х, и в коде часто видны оба слоя: «наследие» и «современно».
Здесь — пары «было → стало», которые видны прямо в этом проекте.

---

## 1. typedef → using

```cpp
typedef unsigned short KeyCode;   // было (C-стиль)
using KeyCode = uint16_t;         // стало (C++11) — читается слева направо,
                                  // работает с шаблонами
```

## 2. NULL → nullptr

```cpp
int* p = NULL;      // было: макрос = 0, путался с целым числом
int* p = nullptr;   // стало: настоящий «указатель в никуда», свой тип
```

## 3. new/delete → умные указатели

```cpp
// было: ручное управление — забыл delete или исключение → утечка
Application* app = CreateApplication();
app->Run();
delete app;

// стало (движок, EntryPoint.h): владелец-обёртка, удалит сама
auto app = Povly::CreateApplication();   // std::unique_ptr
app->Run();
app.reset();                             // или просто выход из области видимости
```

Парное правило современности: создавать сразу под умным указателем:

```cpp
std::unique_ptr<Application> p(new Sandbox());   // можно, но...
return std::make_unique<Sandbox>();              // лучше (SandboxApp.cpp)
```

## 4. include guards → #pragma once

```cpp
#ifndef POVLY_LOG_H      // было: три строки-забора в каждом заголовке
#define POVLY_LOG_H
...
#endif

#pragma once             // стало: одна строка, то же самое
```

## 5. Константы-макросы → constexpr

```cpp
#define GRAVITY -9.8        // было: без типа, невидимо для отладчика
constexpr double GRAVITY = -9.8;   // стало: тип есть, отладчик видит
```

(В проекте остался `#define BIT(x)` — макрос-функция; современный эквивалент
`constexpr int Bit(int x) { return 1 << x; }` — но макрос оставили: он
применяется в enum, где нужны константы времени компиляции из целых выражений.)

## 6. Обычный enum → enum class

```cpp
enum EventType { WindowResize, ... };        // было: утекает в int и наружу
enum class EventType { WindowResize, ... };  // стало: строгий тип
```

В проекте оба — намеренно: `EventType` (идентификатор) строгий,
`EventCategory` (битовая маска) обычный. Подробнее — `02-types/01`.

## 7. printf-стиль → потокобезопасные библиотеки с проверкой формата

```cpp
printf("Hello %d\n", a);              // было: формат строкой, типы не проверяются
std::cout << "Hello " << a << "\n";   // было-2: безопасно, но многословно
POVLY_CORE_INFO("Hello! {0}", a);     // стало: spdlog+fmt — формат проверяется
                                      // КОМПИЛЯТОРОМ, плюс уровни, цвета, время
```

## 8. История с operator<< (случилось в этом проекте)

**Раньше** (старые fmt/spdlog) достаточно было определить ostream-оператор —
и объект печатался в лог неявно:

```cpp
std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}
POVLY_CORE_TRACE("{0}", e);   // работало само собой
```

**Почему изменилось**: неявные преобразования «что угодно в строку» прятали
ошибки (опечатки печатались как hex-адреса и т.п.). fmt 9+ убрал неявный
ostream — тип обязан явно объявить себя печатаемым.

**Сейчас** (фикс в `Event.h`): специализация `fmt::formatter` для наследников
Event — «печатай через ToString()»:

```cpp
namespace fmt {
template <typename T>
struct formatter<T, std::enable_if_t<std::is_base_of_v<Povly::Event, T>, char>>
    : formatter<std::string> {
  auto format(const T& event, format_context& ctx) const {
    return fmt::format_to(ctx.out(), "{}", event.ToString());
  }
};
}
```

Твоя ошибка сборки «type_is_unformattable_for<WindowResizeEvent>» — это как раз
новая защита fmt: она поймала отсутствие форматтера **до запуска программы**.
Старый `printf` молча бы напечатал мусор.

## 9. Сырые массивы/C-строки → std::string, std::vector

```cpp
char name[64];               // было: следи за размером сам, '\0' сам
std::string name = "Povly";  // стало: размер, копирование, сравнение — сами
```

## 10. Собственный main в каждой программе → main внутри движка

Не языковая, а архитектурная эволюция (фреймворки — Hazel, Unity, Qt):

```cpp
// обычная программа:                       // движок (EntryPoint.h):
int main() { /* всё сам */ }                int main() {
                                               Log::Init();
                                               auto app = CreateApplication(); // игра подключается
                                               app->Run();
                                             }
```

Почему: общий код запуска (логи, окно, контекст графики) пишется один раз
в движке; игра диктует только «что за приложение» (см. `05-engine/01`).

---

## Общий принцип эволюции C++

| Было | Проблема | Стало | Выигрыш |
|---|---|---|---|
| `typedef`, `NULL`, `#define`, сырые `new`/`delete`, обычный `enum`, printf | ошибки находятся в рантайме или молчат | `using`, `nullptr`, `constexpr`, умные указатели, `enum class`, fmt | ошибки ловит компилятор; утечи и гонки исключены конструкцией |

Тренд один: **перенести обнаружение ошибки с «во время работы» на «во время
компиляции»**. Именно это и случилось с твоей ошибкой логгера.
