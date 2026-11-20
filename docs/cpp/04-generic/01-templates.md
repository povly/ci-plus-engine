# Шаблоны

Шаблон — заготовка, из которой компилятор порождает код под конкретный тип.

## Шаблонная функция: EventDispatcher::Dispatch

```cpp
// Event.h
class EventDispatcher {
 public:
  EventDispatcher(Event& event) : m_Event(event) {}

  template <typename T, typename F>
  bool Dispatch(const F& func) {
    if (m_Event.GetEventType() == T::GetStaticType()) {
      m_Event.m_Handled |= func(static_cast<T&>(m_Event));
      return true;
    }
    return false;
  }

 private:
  Event& m_Event;
};
```

Вызов:

```cpp
dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& ev) { ... });
```

- `T` задаём явно в `<...>` — `T = WindowResizeEvent`. Компилятор подставляет
  его в текст: `T::GetStaticType()` превращается в `WindowResizeEvent::GetStaticType()`,
  а `static_cast<T&>` — в приведение `Event& → WindowResizeEvent&`.
- `F` — тип переданной функции (лямбды). Его компилятор выводит сам, писать не надо.
- Для каждого нового `T` компилятор порождает отдельную версию функции.

Зачем всё это: событие приходит как общий `Event&`, а обработчик хочет
конкретный тип. Dispatch сравнивает типы и зовёт твой код только при совпадении —
безопасный аналог «проверить и привести тип».

## Специализация: «моя версия для моего типа»

Общий шаблон `fmt::formatter<T>` из библиотеки fmt не знает, как печатать
события. Фикс из `Event.h` добавляет свою версию для всех наследников Event:

```cpp
namespace fmt {
template <typename T>
struct formatter<T, std::enable_if_t<std::is_base_of_v<Povly::Event, T>, char>>
    : formatter<std::string> {
  auto format(const T& event, format_context& ctx) const {
    return fmt::format_to(ctx.out(), "{}", event.ToString());
  }
};
}  // namespace fmt
```

Читать это так: «если T — наследник Event (`std::is_base_of_v`), печатай его
через `ToString()`». Это продвинутый приём; сейчас достаточно понять, что
шаблон можно **специализировать** — задать особое поведение для своих типов.

## Шаблонный класс (для справки)

`std::shared_ptr<spdlog::logger>` — тоже шаблон: один класс `shared_ptr`
на все типы. Написать свой можно так: `template <typename T> class Box { T value; };`.
