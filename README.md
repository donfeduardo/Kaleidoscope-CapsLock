# Kaleidoscope-CapsLock

Are you ever ANGRY?

Do you ever encounter people who are WRONG on the INTERNET?

Do you write code with CONSTANTS?

Do you adhere to a style guide in which things like movie titles are written in ALL CAPS?

If any or all of those are true, Kaleidoscope-CapsLock is the plugin for you!

This is a plugin for [Kaleidoscope](https://github.com/keyboardio/Kaleidoscope) that 
enables traditional Caps Lock functionality on your keyboard, along with LED effects to
indicate when this mode is in effect.

## How to use:

Put it in your include directives:

```sh
...
// Support for LED Caps Lock mode
#include "Kaleidoscope-CapsLock.h"
...
```

Add Key_CapsLock somewhere in your keymap (e.g. in the FUNCTION keymap in the same
positions as the shift keys in your regular keymap):

```sh

  [FUNCTION] =  KEYMAP_STACKED
  (___,      Key_F1,           Key_F2,      Key_F3,     Key_F4,        Key_F5,           XXX,
   Key_Tab,  ___,              Key_mouseUp, ___,        Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE,
   Key_Home, Key_mouseL,       Key_mouseDn, Key_mouseR, Key_mouseBtnL, Key_mouseWarpNW,
   Key_End,  Key_PrintScreen,  Key_Insert,  ___,        Key_mouseBtnM, Key_mouseWarpSW,  Key_mouseWarpSE,
   ___, Key_Delete, ___, Key_CapsLock,
   ___,

   Consumer_ScanPreviousTrack, Key_F6,                 Key_F7,                   Key_F8,                   Key_F9,          Key_F10,          Key_F11,
   Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,    Key_LeftBracket, Key_RightBracket, Key_F12,
                               Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,              Key_RightArrow,  ___,              ___,
   Key_PcApplication,          Consumer_Mute,          Consumer_VolumeDecrement, Consumer_VolumeIncrement, ___,             Key_Backslash,    Key_Pipe,
   Key_CapsLock, ___, Key_Enter, ___,
   ___)


```

And then put it in the Kaleidoscope.use() directive in setup():

```sh
    ...
    // The breathe effect slowly pulses all of the LEDs on your keyboard
    &LEDBreatheEffect,

    // The CapsLock plugin lights up CAPS LOCK mode in a similar way to numpad mode
    &CapsLock,
    ...
```

Once you have compiled and flashed your firmware, you can activate Caps Lock by pressing
fn-shift (or whatever key binding you choose to map to Key_CapsLock). It will remain
active until you repeat the same keypress. Kaleidoscope-CapsLock is not affected by the
state of the unmodified shift key. The behavior of the shift modifier while Caps Lock is
active will be OS dependent.

## Alternatives:

Because Kaleidoscope-CapsLock relies on the host operating system for implementation,
it may not do exactly what *you* want in all cases. One distinction is that it will not
affect the shift state of number keys. If that's the behavior you have in mind, check
out [Kaleidoscope-OneShot](https://github.com/keyboardio/Kaleidoscope-OneShot).
Kaleidoscope-OneShot mimics holding down a specific modifier key by hand (e.g. the shift
key), and thus it affects the state of every key pressed while the OneShot modifier is
active.

## Known Issues:

There seems to be an issue with the LED_CAPS_LOCK state tracked by the keyboard (the
observed behavior is that LED_CAPS_LOCK gets turned on the first time Key_CapsLock is
pressed, and then is not turned off on subsequent presses. As a result, after 
initialization Kaleidoscope-CapsLock tracks its own state. There's a small possibility
this will result in inconsistent state, where the OS thinks Caps Lock is off but the
keyboard LEDs will indicate that it's on. Unplug the keyboard and plug it back in to
resolve this if it happens to you.