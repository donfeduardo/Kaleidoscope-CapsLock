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

  static bool capsCleanupDone;
};
}

extern kaleidoscope::CapsLock_ CapsLock;
