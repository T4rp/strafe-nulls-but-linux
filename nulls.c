#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <unistd.h>

int is_down(Display *disp, char returnKeys[32], KeyCode keycode) {
  int i = keycode / 8;
  int r = keycode % 8;
  int mask = 1 << r;

  return returnKeys[i] & mask;
}

int main() {
  Display *display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "could not open displayu\n");
    return 1;
  }

  KeySym aSym = XStringToKeysym("a");
  KeyCode aKey = XKeysymToKeycode(display, aSym);

  KeySym dSym = XStringToKeysym("d");
  KeyCode dKey = XKeysymToKeycode(display, dSym);

  char returnKeys[32];

  int wasADown = 0;
  int wasDDown = 0;

  while (1) {
    XQueryKeymap(display, returnKeys);

    int isADown = is_down(display, returnKeys, aKey);
    int isDDown = is_down(display, returnKeys, dKey);

    if (!(isADown && isDDown)) {
      if (isADown) {
        XTestFakeKeyEvent(display, dKey, False, 0);
      }
      if (isDDown) {
        XTestFakeKeyEvent(display, aKey, False, 0);
      }
    } else {
      if (wasADown && !wasDDown) {
        XTestFakeKeyEvent(display, aKey, False, 0);
      }

      if (wasDDown && !wasADown) {
        XTestFakeKeyEvent(display, dKey, False, 0);
      }
    }

    wasADown = isADown;
    wasDDown = isDDown;

    XFlush(display);
    usleep(50 * 1000);
  }
}
