# Пространства имён и устройство .h/.cpp

## namespace — ограждение имён

```cpp
namespace Povly {
class Event { ... };
}  // namespace Povly

Povly::Event e;              // извне — с префиксом
// внутри namespace Povly — просто Event
```

Зачем: `Povly::Event` не столкнётся с `Event` из чужой библиотеки. Вендорный
spdlog живёт в `spdlog::` и `fmt::` — поэтому движок и библиотека мирно
соседствуют в одном бинарнике.

Комментарий `}  // namespace Povly` — convention: закрывающая скобка namespace
может быть за экраном от открывающей, подпись помогает.

## Ведущий `::`

```cpp
// Log.h
#define POVLY_CORE_TRACE(...) ::Povly::Log::GetCoreLogger()->trace(__VA_ARGS__)
```

`::` в начале = «искать Povly от глобальной области». Если макрос развернётся
внутри чьего-то namespace Povly::Meh::, без ведущего `::` путь мог бы
разрешиться не туда.

## Объявление vs определение

- **Объявление** (declaration): «есть такая штука». Компилятору достаточно,
  чтобы код скомпилировался.
- **Определение** (definition): «вот она сама, целиком». Нужно линкеру,
  чтобы собрался исполняемый файл.

```cpp
// Log.h — объявления:
class Log {
  static void Init();                          // метод объявлен
  static std::shared_ptr<spdlog::logger> s_CoreLogger;   // поле объявлено
};

// Log.cpp — определения:
void Log::Init() { ... }                       // тело метода
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;      // само статическое поле
```

`extern` из EntryPoint.h — чистое объявление функции, определённой в другом
файле:

```cpp
extern std::unique_ptr<Povly::Application> Povly::CreateApplication();
// «она существует, тело найдёт линкер»
```

## Зачем пара .h/.cpp

- `.h` — публичный каталог: что есть. Его читают все, кто делает `#include`.
- `.cpp` — кухня: как устроено. Компилируется отдельно.
- Правило One Definition: объявлений сколько угодно, определение — одно.
  Поэтому методы с телом внутри класса в .h — ок (inline), а вот свободные
  функции с телом в .h, включённой в много мест, — ошибка линковки
  (лечится `inline` или переездом в .cpp).

## Header guard vs pragma once (было/стало)

```cpp
#ifndef POVLY_LOG_H     // старый способ (include guards)
#define POVLY_LOG_H
... весь файл ...
#endif

#pragma once            // современный способ — то же самое, одной строкой
```

В проекте везде `#pragma once` — поддерживается всеми актуальными компиляторами.
