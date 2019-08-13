#include "Kaleidoscope-CapsLock.h"
#include "Kaleidoscope.h"

bool CapsLock_::capsCleanupDone = true;
bool CapsLock_::swCapsLockState = false;
bool CapsLock_::useHwState = false;
uint8_t CapsLock_::highlightShiftKeys = 2;
cRGB CapsLock_::color = CRGB(255, 0, 0);
uint8_t CapsLock_::shiftHue = 170;
uint8_t CapsLock_::cycleCount = 0;

kaleidoscope::EventHandlerResult CapsLock_::onSetup(void) {
  swCapsLockState = !!(kaleidoscope::hid::getKeyboardLEDs() & LED_CAPS_LOCK);
  return kaleidoscope::EventHandlerResult::OK;
}

static bool getCapsLockState() {
  if (CapsLock_::useHwState) {
    return !!(kaleidoscope::hid::getKeyboardLEDs() & LED_CAPS_LOCK);
  } else {
    return CapsLock_::swCapsLockState;
  }
}

static void syncCapsLock (bool state) {
  bool hwState = !!(kaleidoscope::hid::getKeyboardLEDs() & LED_CAPS_LOCK);
  if (hwState == state) {
    CapsLock_::cycleCount++;
    if (CapsLock_::cycleCount > 1) {
      CapsLock_::useHwState = true;
    }
    CapsLock_::swCapsLockState = hwState;
  } else {
    CapsLock_::cycleCount = 0;
    CapsLock_::useHwState = false;
    CapsLock_::swCapsLockState = state;
  }
}

kaleidoscope::EventHandlerResult CapsLock_::onKeyswitchEvent(
  Key &mappedKey, KeyAddr key_addr, uint8_t keyState) {
  if (CapsLock_::useHwState) {
    /*
      If hardware state is found to be reliable, we'll use that.
      If hardware state fails to change in syncCapsLock subroutine, this will be skipped.
    */
    syncCapsLock(kaleidoscope::hid::getKeyboardLEDs() & LED_CAPS_LOCK);
    return kaleidoscope::EventHandlerResult::OK;
  }
  /*
    Start by using software state on its own.
    If hardware state is found to be unreliable, we'll just keep using software state.
  */
  if (mappedKey == Key_CapsLock) {
    if (keyToggledOff(keyState) ) {
      /*
        The keyToggledOff keyState indicates that the key was released in the last cycle.
        Use that keyState value as a trigger to toggle software state.
      */
      syncCapsLock(!CapsLock_::swCapsLockState);
    }
  }
  return kaleidoscope::EventHandlerResult::OK;
}

/*
  this will paint the preferred color onto letter keys as long as capsState is true.
  When capsState is set to false, reset the color mode to its previous state.
*/
kaleidoscope::EventHandlerResult CapsLock_::afterEachCycle() {
  bool capsState = getCapsLockState();
  if (capsState) {
    capsCleanupDone = false;
    for (uint8_t r = 0; r < ROWS; r++) {
      for (uint8_t c = 0; c < COLS; c++) {
        KeyAddr key_addr = KeyAddr(r, c);
        Key k = Layer.lookupOnActiveLayer(key_addr);

        cRGB shiftColor = highlightShiftKeys == 2 ? breath_compute(shiftHue) : hsvToRgb(shiftHue, 255, 255);

        if ((k.raw >= Key_A.raw) && (k.raw <= Key_Z.raw)) {
          ::LEDControl.setCrgbAt(key_addr, color);
        } else if (highlightShiftKeys && (k == Key_LeftShift || k == Key_RightShift)) {
          ::LEDControl.setCrgbAt(key_addr, shiftColor);
        } else {
          ::LEDControl.refreshAt(key_addr);
        }
      }
    }
    return kaleidoscope::EventHandlerResult::OK;
  } else {
    if (!capsCleanupDone) {
      // Some keys seem to get "stuck" in the painted color. Reset current mode to unstick them.
      ::LEDControl.set_mode(::LEDControl.get_mode_index());
      capsCleanupDone = true;
    }
    return kaleidoscope::EventHandlerResult::OK;
  }
}

CapsLock_ CapsLock;
