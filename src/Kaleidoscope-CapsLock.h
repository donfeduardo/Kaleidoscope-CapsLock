#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Macros.h"
#include "LEDUtils.h"

class CapsLock_ : public KaleidoscopePlugin {
 public:
  CapsLock_(void) {}

  void begin(void) final;

 private:
  static void loopHook(const bool postClear);
  static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state); 

  static bool capsCleanupDone;
  static bool capsState;
};

extern CapsLock_ CapsLock;
