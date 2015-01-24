#include <arduino.h>
#include "qs301.h"

const int QS301Module::kNumBits[11] = {
  0x0,    // OFF
  1,      // 0
  1 << 9, // 1
  1 << 8, // 2
  1 << 7, // 3
  1 << 6, // 4
  1 << 5, // 5
  1 << 4, // 6
  1 << 3, // 7
  1 << 2, // 8
  1 << 1, // 9
};

void QS301Module::init(int din, int oe, int stcp, int shcp) {
  kDin_  = din;
  kOE_   = oe;
  kStcp_ = stcp;
  kShcp_ = shcp;

  color_  = COLOR_OFF;
  number_ = NUM_OFF;
  colon_  = COLON_OFF;

  pinMode(kDin_, OUTPUT);
  pinMode(kOE_, OUTPUT);
  pinMode(kStcp_, OUTPUT);
  pinMode(kShcp_, OUTPUT);

  digitalWrite(kDin_, LOW);
  digitalWrite(kOE_, LOW);
  digitalWrite(kStcp_, LOW);
  digitalWrite(kShcp_, LOW);
}

void QS301Module::setNumber(int number) {
  int bits;
  if (number < 0) {
    number = -1; // off
  }

  if (number > 9) {
    number = -1; // error
  }

  number_ = kNumBits[number + 1];
}

void QS301Module::flushBits(int bits, int n) {
  for (int i = 0; i < n; ++i) {
    digitalWrite(kDin_, (bits >> (n - i - 1)) & 0x1);
    digitalWrite(kShcp_, HIGH);
    digitalWrite(kShcp_, LOW);
  }
}

void QS301Module::commit() {
  digitalWrite(kStcp_, HIGH);
  digitalWrite(kStcp_, LOW);
}

void QS301Module::flush() {
  flushBits(color_, 3);
  flushBits(number_, 10);
  flushBits(colon_, 3);
}
