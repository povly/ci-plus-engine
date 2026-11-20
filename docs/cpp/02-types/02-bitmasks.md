# Битовые маски

Категории событий хранятся в одном `int` — по биту на категорию.

```cpp
// Core.h
#define BIT(x) (1 << x)

// Event.h
enum EventCategory {
  None = 0,
  EventCategoryApplication  = BIT(0),  // 1 = 0b0001
  EventCategoryInput        = BIT(1),  // 2 = 0b0010
  EventCategoryKeyboard     = BIT(2),  // 4 = 0b0100
  EventCategoryMouse        = BIT(3),  // 8 = 0b1000
  EventCategoryMouseButton  = BIT(4),  // 16
};
```

## Три операции

**Сдвиг** `1 << x` — поставить единицу в бит номер x:

```cpp
BIT(0) → 0b0001
BIT(3) → 0b1000
```

**Или `|`** — склеить флаги в комбинацию:

```cpp
// MouseEvent.h — кнопка мыши сразу в трёх категориях
EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
// 0b1000 | 0b0010 | 0b10000 = 0b11010
```

**И `&`** — проверить наличие флага; ненулевой результат = «флаг есть»:

```cpp
// Event.h
bool IsInCategory(EventCategory category) {
  return GetCategoryFlags() & category;
}

e.IsInCategory(EventCategoryInput);   // правда, если бит Input выставлен
```

## Зачем

Один `int` — 32 бита — 32 независимые «галочки» вместо 32 bool-полей.
Дёшево передавать, быстро проверять, легко добавлять новые категории.

## То же самое в жизни

- `glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)` в OpenGL — та же маска.
- Права доступа к файлам в Linux (`rwx`) — тоже биты.
