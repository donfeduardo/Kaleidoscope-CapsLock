#include "Kaleidoscope-CapsLock.h"
//#include "LEDUtils.h"
#include "Kaleidoscope.h"

bool CapsLock_::capsCleanupDone = true;
bool CapsLock_::swCapsLockState = false;
bool CapsLock_::useHwState = false;
uint8_t CapsLock_::highlightShiftKeys = 2;
cRGB CapsLock_::color = CRGB(255, 0, 0);
uint8_t CapsLock_::shiftHue = 170;
uint8_t CapsLock_::cycleCount = 0;

kaleidoscope::EventHandlerResult CapsLock_::onSetup(void) {
  swCapsLockState = !!(kaleidoscope::Runtime.hid().keyboard().getKeyboardLEDs() & LED_CAPS_LOCK);
  return kaleidoscope::EventHandlerResult::OK;
}

static bool getCapsLockState() {
  if (CapsLock_::useHwState) {
    return !!(kaleidoscope::Runtime.hid().keyboard().getKeyboardLEDs() & LED_CAPS_LOCK);
  } else {
    return CapsLock_::swCapsLockState;
  }
}

static void syncCapsLock (bool state) {
  bool hwState = !!(kaleidoscope::Runtime.hid().keyboard().getKeyboardLEDs() & LED_CAPS_LOCK);
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
  Key &mappedKey, KeyAddr keyAddr, uint8_t keyState) {
  if (CapsLock_::useHwState) {
    /*
      If hardware state is found to be reliable, we'll use that.
      If hardware state fails to change in syncCapsLock subroutine, this will be skipped.
    */
    syncCapsLock(kaleidoscope::Runtime.hid().keyboard().getKeyboardLEDs() & LED_CAPS_LOCK);
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
    for (uint8_t r = 0; r < kaleidoscope::Device::KeyScannerProps::matrix_rows; r++) {
      for (uint8_t c = 0; c < kaleidoscope::Device::KeyScannerProps::matrix_columns; c++) {
        Key k = Layer.lookupOnActiveLayer(KeyAddr(r, c));

        cRGB shiftColor = highlightShiftKeys == 2 ? breath_compute(shiftHue) : hsvToRgb(shiftHue, 255, 255);

        if ((k.getRaw() >= Key_A.getRaw()) && (k.getRaw() <= Key_Z.getRaw())) {
          ::LEDControl.setCrgbAt(KeyAddr(r, c), color);
        } else if (highlightShiftKeys && (k == Key_LeftShift || k == Key_RightShift)) {
          ::LEDControl.setCrgbAt(KeyAddr(r, c), shiftColor);
        } else {
          ::LEDControl.refreshAt(KeyAddr(r, c));
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
