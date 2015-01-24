#ifndef QS301_H_
#define QS301_H_

#define COLOR_WHITE  0x0
#define COLOR_PURPLE 0x1  // purple
#define COLOR_CYAN   0x2  // cyan
#define COLOR_BLUE   0x3
#define COLOR_YELLOW 0x4
#define COLOR_RED    0x5
#define COLOR_GREEN  0x6
#define COLOR_OFF    0x7

#define NUM_OFF     0x0

#define COLON_OFF   0x0
#define COLON_DOT   0x2
#define COLON_TOP   0x4

class QS301Module {

public:
  void init(int din, int oe, int stcp, int shcp);

  void setColor(int color) {
    color_ = color;
  }

  void setNumber(int number);

  void setColon(int colon) {
    colon_ = colon;
  }
  void flush();

  void commit();
private:
  void flushBits(int bits, int n);

  int color_;
  int number_;
  int colon_;

  int kDin_;
  int kOE_;
  int kStcp_;
  int kShcp_;

  static const int kNumBits[11];
};

#endif // QS301_H_
