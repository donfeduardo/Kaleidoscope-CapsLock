#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

namespace kaleidoscope {
class CapsLock_ : public KaleidoscopePlugin {
 public:
  CapsLock_(void) {}

  void begin(void) final;

 private:
  static void capsLockLoopHook(const bool postClear);
  static Key  capsLockEventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state); 

  static bool capsCleanupDone;
  static bool capsState;
};
}

extern kaleidoscope::CapsLock_ CapsLock;
