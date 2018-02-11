#include "Kaleidoscope-CapsLock.h"
#include "LEDUtils.h"
#include "Kaleidoscope.h"
#include "layers.h"

bool CapsLock_::capsCleanupDone = true;
bool CapsLock_::capsState = false;
cRGB active_mode_color = CRGB(255, 0, 0);

void CapsLock_::begin(void) {
  capsState = !!(kaleidoscope::hid::getKeyboardLEDs() & LED_CAPS_LOCK);
  Kaleidoscope.useLoopHook(loopHook);
  Kaleidoscope.useEventHandlerHook(eventHandlerHook);
}

/*
  loopHook will paint the active_mode_color onto letter keys as long as capsState is true.
  When capsState is set to false, reset the color mode to its previous state.
*/
void CapsLock_::loopHook(bool postClear) {
  if (!postClear)
    return;

  capsCleanupDone = false;
  
  LEDControl.set_mode(LEDControl.get_mode_index());

  if (capsState) {
    for (uint8_t r = 0; r < ROWS; r++) {
      for (uint8_t c = 0; c < COLS; c++) {
        Key k = Layer.lookupOnActiveLayer(r, c);

        cRGB breathColor = breath_compute();

        if ((k.raw >= Key_A.raw) && (k.raw <= Key_Z.raw)) {
          LEDControl.setCrgbAt(r, c, active_mode_color);
        } else if (k == Key_LeftShift || k == Key_RightShift) {
          LEDControl.setCrgbAt(r, c, breathColor);
        } else {
          LEDControl.refreshAt(r, c);
        }
      }
    }
  } else {
    if (!capsCleanupDone) {
      // Some keys seem to get "stuck" in the painted color. Reset current mode to unstick them.
      LEDControl.set_mode(LEDControl.get_mode_index());
      capsCleanupDone = true;
    }
    return;
  }
}

/*
  eventHandlerHook listens for "presses" of the mapped CapsLock key.
  When that key is toggled on, flip the capsState bit.
*/
Key CapsLock_::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (key_state & INJECTED)
    return mapped_key;

  if (mapped_key == Key_CapsLock) {
    if (keyToggledOn(key_state)) {
      // LED_CAPS_LOCK isn't ever toggled off after being set. Track state ourselves.
      capsState = !capsState;
    }
  }
  
  return mapped_key;
}

CapsLock_ CapsLock;
