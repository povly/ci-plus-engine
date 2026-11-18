# Дорожная карта: свой движок + мультиплеерная игра уровня DayZ

> Зафиксировано: 2026-11-18. Цель: собственный кроссплатформенный движок и на нём —
> мультиплеерная survival-игра с большим онлайном. Решение принято осознанно,
> вопреки оценке сроков/рисков. Работаем от игры, а не от движка.

## 1. Цель и рамки

- **Язык:** строго C++. Kohi (C) — только справочник по архитектуре, не основной трек.
- **ОС:** Windows + Linux + macOS. На маке ни OpenGL (>4.1), ни Vulkan нативно —
  значит, рендер с самого начала за абстракцией (`IRenderer`), бэкенды подключаются.
- **GPU:** все современные (Vulkan) или вообще все, включая старые (OpenGL).
- **Сеть:** авторитетный сервер, UDP, свой протокол; цель — 32–64+ игрока
  (для справки: DayZ = 60/сервер, GTA Online = 30/сессия, больше — FiveM-подобный interest management).
- **ECS:** свой, с полным пониманием.

## 2. Ключевые архитектурные решения

1. **Рендер — за интерфейсом, первый бэкенд OpenGL 4.5/4.6.**
   Причины: скорость обучения (треугольник ~150 строк vs ~900 в Vulkan),
   работа на всех видеокартах, концепции переносятся в Vulkan напрямую.
   Vulkan — вторым бэкендом позже, отдельным этапом.
2. **Граница «с нуля» = публичные API и открытые библиотеки** (Vulkan, OpenGL,
   Win32/X11/Cocoa, сокеты, asio). Неприемлемо: чужие закрытые фреймворки-заготовки
   (типа Walnut), сделанные за кадром.
3. **Неткод свой, поверх сырых UDP-сокетов** по методике Фидлера:
   слоты клиентов, challenge-handshake, защита от спуфинга/DDoS-амплификации,
   надёжные сообщения поверх ненадёжного канала.
4. **Game-driven development:** движок наращивается только когда фича нужна игре.
   Всегда есть играрабельная сборка.

## 3. Учебные ресурсы

### Движок — основной трек на C++
| Ресурс | Что даёт | Роль в треке |
|---|---|---|
| [PardCode — Game Engine Tutorial Series](https://www.youtube.com/playlist?list=PLv8DnRaQOs5-MR-zbP1QUdq5FL0FWqVzg) ([DirectX](https://www.pardcode.com/cpp-3d-game-tutorial-series) / [OpenGL](https://www.pardcode.com/opengl-3d-game-tutorial-series) версии) | C++, всё руками: Win32/X11/Cocoa, ввод, физика, entity system | **стержень фазы 1** |
| [OGLDEV / Etay Meiri](https://www.youtube.com/@OGLDEV) ([ogldev.org](https://ogldev.org)) | OpenGL C++ под Win+Linux; свежая серия Vulkan For Beginners; террейн, скелетная анимация | **стержень фазы 2 и 6** |
| [Brendan Galea — Vulkan Game Engine Tutorials](https://www.youtube.com/playlist?list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR) | Лучший C++ туториал по Vulkan + математика с выводом (закончена в 2022) | Vulkan-бэкенд позже |
| [vkguide.dev](https://vkguide.dev/) | Текстовый гайд Vulkan 1.3 / C++20, архитектура уровня движка, GPU-driven rendering | Vulkan-бэкенд позже |
| [Dave Churchill — COMP 4300](https://www.youtube.com/playlist?list=PL_xRyXins848nDj2v-TJYahzvs-XW9sVV) | Университетский курс: ECS лекциями, EntityManager, cache coherency, memory pooling, SFML | **стержень фазы 3** |
| [Gustavo Pezzi — C++ Game Engine Programming](https://pikuma.com/) | Платный: ECS с нуля, data-oriented design, Lua-скриптинг, SDL2 | альтернатива Churchill, глубже |
| [Ozzadar Dev — Cross Platform Game Engine Development](https://www.youtube.com/playlist?list=PLKH0QdYnI6AnUW3V3xeUuIxr0nyE9j61h) | Маленькая серия: Vulkan+SDL+MoltenVK (macOS) | ориентир, не основной |
| Русскоязычное: [Делаем игровой движок на C++ и OpenGL с нуля](https://www.youtube.com/playlist?list=PL6x9Hnsyqn2XU7vc8-oFLojbibK91fVd-) — качество не проверено | параллель на родном языке | опционально |

### Справочник по архитектуре (язык C, не трек)
| Ресурс | Что даёт |
|---|---|
| [Kohi — Travis Vroman](https://www.youtube.com/playlist?list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj) ([kohiengine.com](https://kohiengine.com), [github](https://github.com/travisvroman/kohi)) | Полный движок с нуля на камеру: платформенный слой, ECS, редактор, ассет-пайплайн. Смотреть выборочно, когда «как это устроено в целом» непонятно из C++-материалов |

### Сеть / мультиплеер
| Ресурс | Что даёт |
|---|---|
| [Glenn Fiedler — Gaffer On Games](https://gafferongames.com/) | Канон: «What Every Programmer Needs To Know About Game Networking», серия «Building a Game Network Protocol» (UDP, client-server, challenge, защита), предсказание, интерполяция |
| [Gabriel Gambetta — Fast-Paced Multiplayer](https://www.gabrielgambetta.com/client-server-game-architecture.html) | Классика: client-server, предсказание, reconciliation, лаг-компенсация |
| [javidx9 — Networking in C++ #1–4](https://www.youtube.com/watch?v=2QI3c4pL6lE&list=PLrO_9FMX8xJE8NgepZR1etrsU63fDDGxO) | MMO client/server на asio (публичная открытая либа), всё объясняется с нуля |
| [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) | Библия сырых сокетов, POSIX + Windows |
| [nanoMMO](https://nanommo.nenuadrian.com/) | MMORPG на C++20: авторитетный сервер, protobuf, Docker, нагрузочные тесты |
| [codersblock — Multiplayer FPS in C++](https://codersblock.org/multiplayer-fps/) | 8 частей: свой неткод на голых сокетах (Winsock-привкус, переносится) |

## 4. Дорожная карта (фазы)

| Фаза | Что строим | Учимся по | Оценка (соло) |
|---|---|---|---|
| 0 | C++ до уверенности: RAII, умные указатели, move, базовые шаблоны | Cherno C++ | постоянно |
| 1 | Ядро: окно на 3 ОС (Win32/X11/Cocoa), game loop, ввод, логгер, ФС | PardCode (Kohi — параллельно смотреть, не писать) | 2–4 мес |
| 2 | Рендер за `IRenderer`: OpenGL 4.5 — меши, текстуры, свет, фреймбуферы | OGLDEV / LearnOpenGL | 3–6 мес |
| 3 | ECS + первый геймплей: ходьба, камера, коллизии, стрельба | Churchill COMP 4300 | 2–3 мес |
| 4 | Первый мультиплеер: 2 клиента + авторитетный сервер, синхрон движения | Фидлер | 2–4 мес |
| 5 | Масштаб сети: interest management, дельта-снапшоты, предсказание, лаг-компенсация → 8 → 32 → 64 игрока | Фидлер + Gambetta | 4–8 мес |
| 6 | Контент-системы: стриминг террейна, скелетная анимация, инвентарь + персистентность (SQLite → Postgres), транспорт | OGLDEV (террейн, скелетная анимация) | 6–12 мес |
| 7 | Игра: выживание, лут, заражённые, баланс | — | ∞ |

**Веха после фазы 4:** мини-DayZ — несколько человек бегают по миру и видят друг друга.
С этого момента проект = игра, а не движок.

**Vulkan-бэкенд** за тем же `IRenderer` — после фазы 5, отдельным этапом:
Brendan Galea (практика) + vkguide.dev (архитектура). Обе на C++.

## 5. Правила выживания на марафоне

1. Каждый день/неделю — коммит. Каждый месяц — работающая сборка.
2. Девлог (публичный) — анкер мотивации + будущее комьюнити.
3. Один активный фронт. Никаких «рендер + сеть + ECS параллельно».
4. Не выпиливать сетевую архитектуру из прототипа: мультиплеер закладывается
   в ядро геймплея сразу (клиент шлёт ввод, сервер — состояние).

## 6. Первый шаг (эта неделя)

1. Окружение: clang/gcc + CMake + gdb на своей ОС.
2. PardCode — Game Engine Tutorial Series, первые эпизоды; Kohi #000 параллельно — только посмотреть общую архитектуру.
3. Прочитать: Фидлер, «What Every Programmer Needs To Know About Game Networking».
4. Цель месяца: своё окно, собирающееся на трёх ОС.

## 7. Справочные цифры (чтобы калибровать ожидания, а не пугаться)

- DayZ Standalone: ~60 человек команда, 6+ лет до 1.0, свой движок, готовая карта.
- Rust: начат ~4 людьми на Unity как DayZ-like, 11+ лет, студия 30+.
- Vintage Story: ядро 2 разработчика, 8+ лет — реальный потолок соло.
- GTA Online: 16–30 игроков/сессия; сотни+ — только кастомный неткод (FiveM) с interest management.
