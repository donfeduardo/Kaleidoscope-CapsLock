#include "Kaleidoscope-CapsLock.h"

namespace kaleidoscope {
bool CapsLock_::capsCleanupDone = true;
cRGB activeModeColor = CRGB(255, 0, 0);

void CapsLock_::begin(void) {
  Kaleidoscope.useLoopHook(capsLockLoopHook);
}

/*
  loopHook will paint the activeModeColor onto letter keys as long as capsState is true.
  When capsState is set to false, reset the color mode to its previous state.
*/
void CapsLock_::capsLockLoopHook(bool postClear) {
  if (!postClear)
    return;

  bool capsState = !!(kaleidoscope::hid::getKeyboardLEDs() & LED_CAPS_LOCK);

  if (capsState) {
    capsCleanupDone = false;
    for (uint8_t r = 0; r < ROWS; r++) {
      for (uint8_t c = 0; c < COLS; c++) {
        Key k = Layer.lookupOnActiveLayer(r, c);

        cRGB breathColor = breath_compute();

        if ((k.raw >= Key_A.raw) && (k.raw <= Key_Z.raw)) {
          ::LEDControl.setCrgbAt(r, c, activeModeColor);
        } else if (k == Key_LeftShift || k == Key_RightShift) {
          ::LEDControl.setCrgbAt(r, c, breathColor);
        } else {
          ::LEDControl.refreshAt(r, c);
        }
      }
    }
  } else {
    if (!capsCleanupDone) {
      // Some keys seem to get "stuck" in the painted color. Reset current mode to unstick them.
      ::LEDControl.set_mode(::LEDControl.get_mode_index());
      capsCleanupDone = true;
    }
    return;
  }
}

}

kaleidoscope::CapsLock_ CapsLock;
