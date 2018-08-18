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

```
...
// Support for LED Caps Lock mode
#include "Kaleidoscope-CapsLock.h"
...
```

If Key_CapsLock doesn't already exist somewhere in your keymap, add it. Since the first
version of this plugin was written, the default layout has changed to add this binding to
the FUNCTION keymap on the 'Any' key. Personally I like it better in the same positions
as the shift keys in your regular keymap. It seems more memorable to me that way:

```

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

Once you have verified the binding of Key_CapsLock, then add a line to your
KALEIDOSCOPE_INIT_PLUGINS() directive:

```
    ...
    // The breathe effect slowly pulses all of the LEDs on your keyboard
    LEDBreatheEffect,

    // The CapsLock plugin lights up CAPS LOCK mode in a similar way to numpad mode
    CapsLock,
    ...
```

Once you have compiled and flashed your firmware, you can activate Caps Lock by pressing
fn-shift (or whatever key binding you choose to map to Key_CapsLock). It will remain
active until you repeat the same keypress. Kaleidoscope-CapsLock is not affected by the
state of the unmodified shift key. The behavior of the shift modifier while Caps Lock is
active will be OS dependent.

## Options:

Based on user feedback the highlight color, shift key color, and shift key highlight mode
are all now configurable options. You may change the defaults by specifying new values in
your setup() directive:

```
  CapsLock.color = CRGB(255, 0, 0);
  // The default is red, but any valid CRGB value should work.

  CapsLock.highlightShiftKeys = 2;
  // The default value of 2 highlights the shift keys with an animated "breathe" effect.
  // Change this to 1 for a static highlight with no animation.
  // Change it to 0 to disable the highlighting of the shift keys.
  
  CapsLock.shiftHue = 170;
  // The default base hue for the shift keys is blue. It should accept any integer
  // between 0 and 255, but I haven't tested them all to be sure.
```

## Alternatives:

Because Kaleidoscope-CapsLock relies on the host operating system for implementation,
it may not do exactly what *you* want in all cases. One distinction is that it will not
affect the shift state of number keys. If that's the behavior you have in mind, check
out [Kaleidoscope-OneShot](https://github.com/keyboardio/Kaleidoscope-OneShot).
Kaleidoscope-OneShot mimics holding down a specific modifier key by hand (e.g. the shift
key), and thus it affects the state of every key pressed while the OneShot modifier is
active.

## Known Issues:

There's a small possibility that a rapid press of Key_CapsLock will result in an
inconsistent state, where the OS thinks Caps Lock is off but the keyboard LEDs will
indicate that it's on. With the current version of the plugin I have found that a second,
equally rapid press should resolve this issue; if it doesn't, simply disconnecting the
keyboard and then reconnecting it should take care of it.

There seems to be an issue with the LED_CAPS_LOCK state tracked by the keyboard. This
appears somehow to be dependent on the host operating system. On macOS the observed
behavior is that LED_CAPS_LOCK gets turned on the first time Key_CapsLock is pressed,
and then is not turned off on subsequent presses. At least one user has reported that he
is not affected by this issue, but I'm unable to duplicate his report on my system, as
much as I'd prefer for the code to depend solely on the hardware state.

Because of this issue, after initialization Kaleidoscope-CapsLock initially tracks its
own state. The newest version of the plugin has some code that tries to do the right
thing based on observed behavior. If the plugin is able to determine that the hardware
state is consistent, it should switch to using the hardware state instead of the software
state. If the hardware state is not consistent (and it's not on my system) the plugin
will continue to track its own state.

