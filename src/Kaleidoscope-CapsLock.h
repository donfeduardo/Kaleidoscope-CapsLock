#pragma once

#include "Kaleidoscope-LEDControl.h"

class CapsLock_ : public kaleidoscope::Plugin {
  public:
    CapsLock_(void) {}
    static cRGB color;
    static uint8_t shiftHue;
    static uint8_t highlightShiftKeys;

    kaleidoscope::EventHandlerResult onSetup(void);
    kaleidoscope::EventHandlerResult afterEachCycle();
    kaleidoscope::EventHandlerResult onKeyswitchEvent(Key &mappedKey, KeyAddr key_addr, uint8_t keyState);
    static bool swCapsLockState;
    static bool useHwState;
    static uint8_t cycleCount;

  private:
    static bool capsCleanupDone;
};

extern CapsLock_ CapsLock;
