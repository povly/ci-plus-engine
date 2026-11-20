#pragma once

#include <sstream>

#include "engine/Events/Event.h"
#include "engine/KeyCodes.h"

namespace Hazel {

class KeyEvent : public Povly::Event {
 public:
  Povly::KeyCode GetKeyCode() const { return m_KeyCode; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
 protected:
  KeyEvent(const Povly::KeyCode keycode) : m_KeyCode(keycode) {}

  Povly::KeyCode m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
 public:
  KeyPressedEvent(const Povly::KeyCode keycode, bool isRepeat = false)
      : KeyEvent(keycode), m_IsRepeat(isRepeat) {}

  bool IsRepeat() const { return m_IsRepeat; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat
       << ")";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)
 private:
  bool m_IsRepeat;
};

class KeyReleasedEvent : public KeyEvent {
 public:
  KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << m_KeyCode;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
 public:
  KeyTypedEvent(const Povly::KeyCode keycode) : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyTypedEvent: " << m_KeyCode;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyTyped)
};
}  // namespace Hazel
