# Наследование и «контракт» (абстрактный класс)

## Базовый класс Event — контракт

`src/engine/Events/Event.h`:

```cpp
class POVLY_API Event {
 public:
  virtual EventType GetEventType() const = 0;    // чисто виртуальная (= 0)
  virtual const char* GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); }  // просто виртуальная

  bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & category;
  }

 protected:
  bool m_Handled = false;

  friend class EventDispatcher;
};
```

Расшифровка:

- `virtual ... = 0` — **чисто виртуальная функция**: тела нет, каждый наследник
  ОБЯЗАН написать свою версию. Класс с хоть одной такой функцией —
  **абстрактный**: `Event e;` не скомпилируется.
  Это и есть «контракт» / интерфейс: Event диктует «любое событие обязано уметь
  назвать свой тип, имя, категорию, рассказать о себе».
- `virtual` без `= 0` (ToString) — наследник МОЖЕТ переопределить, но не обязан:
  есть версия по умолчанию (вернуть имя).
- Виртуальность = вызов выбирается по **реальному** типу объекта во время
  работы программы, а не по типу ссылки/указателя.
- `friend class EventDispatcher;` — разрешает EventDispatcher писать в
  защищённое `m_Handled`.

## Наследник

```cpp
class WindowResizeEvent : public Event {   // «является Event»
 public:
  std::string ToString() const override { ... }   // своя версия
  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
```

- `: public Event` — публичное наследование: WindowResizeEvent «является» Event,
  поэтому его можно передавать туда, где ждут `Event&` (так работает диспетчер).
- `override` — «я переопределяю виртуальную функцию родителя». Опечатался в
  имени/подписи — ошибка компиляции, а не тихо новый метод. Всегда пиши.

## Двухуровневая иерархия

```
Event
├─ KeyEvent (абстрактный: хранит m_KeyCode, конструктор protected)
│  ├─ KeyPressedEvent   (+ m_IsRepeat)
│  ├─ KeyReleasedEvent
│  └─ KeyTypedEvent
├─ MouseButtonEvent (абстрактный: хранит m_Button)
│  ├─ MouseButtonPressedEvent
│  └─ MouseButtonReleasedEvent
├─ WindowResizeEvent, WindowCloseEvent, AppTickEvent, ...
```

Промежуточный класс (KeyEvent) собирает то, что общее для группы: поле кода
клавиши и геттер. Сам он абстрактный — нет `EVENT_CLASS_TYPE`, а конструктор
`protected` — снаружи создать нельзя, только наследникам.

Конструктор родителя вызывается в списке инициализации:

```cpp
KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
    : KeyEvent(keycode),          // ← конструктор родителя заполняет m_KeyCode
      m_IsRepeat(isRepeat) {}
```

Порядок всегда один: сначала родитель, потом свои поля.

## Наследование в самом движке

`src/sandbox/SandboxApp.cpp`:

```cpp
class Sandbox : public Povly::Application {
 public:
  Sandbox() {}
  ~Sandbox() {}
};
```

Игра наследует `Application` и подсовывает движку свой класс. Движок в ответ
обещает правильное удаление — поэтому:

## Почему `virtual ~Application()`

```cpp
Application();
virtual ~Application();
```

Движок хранит игру как указатель на **базу**: `std::unique_ptr<Application> app`.
Когда `app` уничтожается, вызывается деструктор через указатель на Application.
Без `virtual` вызвался бы только `~Application()`, а `~Sandbox()` — нет
(утечки, недоочистка). С `virtual` — деструктор реального типа.
**Правило: у любого класса, от которого наследуют, деструктор виртуальный**
(или protected non-virtual, но это продвинутый случай).
