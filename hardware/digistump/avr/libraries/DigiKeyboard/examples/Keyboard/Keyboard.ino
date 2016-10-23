#include <DigiKeyboard.h>

void setup() {
  //NOTE! - this is a breaking change from older version, 
  //begin is now required to start the USB stuff, or renumerate - 
  //this library should now also be boot compatible
  DigiKeyboard.begin();
  //while (DigiKeyboard.isConnected() == 0); // optional - use to wait to run code until connected
  //DigiKeyboard.autoPoll(); //optional - if you call this then you do not need to poll using DigiKeyboard.delay, update(), or poll() 
  //- it will poll USB every 10ms automatically - may mess up other interupts 
}


void loop() {

  
  // Type out this string letter by letter on the computer (assumes US-style
  // keyboard)
  DigiKeyboard.println("Hello Digispark!");

  //Note new format of keycode defines, old format works too
  //press key
  DigiKeyboard.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_1); //can send up to 6 keycodes by adding them as additional parameters - ie pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_1, KEYCODE_A)
  //release key
  DigiKeyboard.pressKey(0, 0);

  //press and release
  DigiKeyboard.sendKeyStroke(KEYCODE_ENTER); //sendKeyStroke(keycode, modifiers) also works for backward compatability

  if(DigiKeyboard.capsLock() == 1){ //numLock() and scrollLock() also work - getLEDState() returns the HID report for LED state
    DigiKeyboard.println("Caps Lock On");
  }
  
  // It's better to use DigiKeyboard.delay() over the regular Arduino delay()
  // if doing keyboard stuff because it keeps talking to the computer to make
  // sure the computer knows the keyboard is alive and connected - you could also call update() or poll() every 10ms
  DigiKeyboard.delay(5000);
}
