#pragma once

#include <ostream>
#include <string>
#include <type_traits>

#include "engine/Core.h"
#include "spdlog/fmt/fmt.h"

namespace Povly {
enum class EventType {
  None = 0,
  WindowClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  AppTick,
  AppUpdate,
  AppRender,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum EventCategory {
  None = 0,
  EventCategoryApplication = BIT(0),
  EventCategoryInput = BIT(1),
  EventCategoryKeyboard = BIT(2),
  EventCategoryMouse = BIT(3),
  EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                \
  static EventType GetStaticType() { return EventType::type; }                \
  virtual EventType GetEventType() const override { return GetStaticType(); } \
  virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int GetCategoryFlags() const override { return category; }

class POVLY_API Event {
  friend class EventDispatcher;

 public:
  virtual EventType GetEventType() const = 0;
  virtual const char* GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); };

  bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & category;
  }

 protected:
  bool m_Handled = false;
};

class EventDispatcher {
 public:
  EventDispatcher(Event& event) : m_Event(event) {}

  // F will be deduced by the compiler
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

std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}
}  // namespace Povly

// fmt (bundled in spdlog) has no implicit ostream formatting since v9,
// so events need a formatter to be loggable: POVLY_CORE_TRACE("{0}", e).
namespace fmt {
template <typename T>
struct formatter<T, std::enable_if_t<std::is_base_of_v<Povly::Event, T>, char>>
    : formatter<std::string> {
  auto format(const T& event, format_context& ctx) const {
    return fmt::format_to(ctx.out(), "{}", event.ToString());
  }
};
}  // namespace fmt
