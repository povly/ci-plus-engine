# Система событий целиком

Файлы: `src/engine/Events/*.h`, `src/engine/Application.cpp`.

## Зачем события

Окно изменило размер, нажата клавиша, двигается мышь — это происходит
**внутри** операционной системы/библиотеки окна, а реагировать должен
**твой** код. Событие — пакет данных о том, что произошло: его создают на
месте происшествия и передают всем заинтересованным.

## Устройство

**1. Контракт** (`Event.h`) — абстрактный класс, требует от любого события:
знать свой тип (`GetEventType`), имя (`GetName`), категории (`GetCategoryFlags`),
уметь рассказать о себе (`ToString`).

**2. Конкретные события** — наследники:

```
Event (контракт)
├─ KeyEvent (промежуточный: m_KeyCode)
│  ├─ KeyPressedEvent (+ m_IsRepeat)   ├─ MouseMovedEvent (x, y)
│  ├─ KeyReleasedEvent                 ├─ MouseScrolledEvent (dx, dy)
│  └─ KeyTypedEvent                    ├─ MouseButtonEvent (промежуточный: m_Button)
├─ WindowResizeEvent (w, h)            │  ├─ ...Pressed
├─ WindowCloseEvent                    │  └─ ...Released
└─ AppTick / AppUpdate / AppRender
```

**3. Макросы** — каждому событию без копипасты дают 3 метода типа и метод
категорий (`EVENT_CLASS_TYPE`, `EVENT_CLASS_CATEGORY`).

**4. Диспетчер** — безопасное сужение типа:

```cpp
void OnEvent(Event& e) {                    // e пришёл как ОБЩИЙ тип
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& ev) {
    // сюда попадём, только если e на самом деле WindowResizeEvent
    return true;
  });
  dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& ev) {
    // ...
    return true;
  });
}
```

`Dispatch<T>` сравнивает тип объекта (`GetEventType()`) с типом класса
(`T::GetStaticType()`); совпало — вызывает твою лямбду уже с правильным типом
(`static_cast<T&>`), не совпало — пропускает. Внутри шаблон
(см. `04-generic/01-templates.md`), лямбда — `04-generic/02-lambdas.md`.

## Что происходит сейчас в коде

```cpp
// Application.cpp — временная проверка, что всё работает
WindowResizeEvent e(1280, 720);   // создали событие вручную
POVLY_CORE_TRACE(e);              // печатает "WindowResizeEvent: 1280, 720"
while (true);                     // будущий главный цикл (пока пустой)
```

Позже (по роадмапу `docs/game-engine-roadmap.md`) события начнёт присылать
окно GLFW: каждый кадр движок соберёт их и раздаст через OnEvent/диспетчер.

## Печать события в лог

`POVLY_CORE_TRACE(e)` работает благодаря специализации `fmt::formatter` в
`Event.h`: «любой наследник Event печатается через свой ToString()».
История, почему она понадобилась — `07-old-vs-new/README.md`, пункт про fmt.
