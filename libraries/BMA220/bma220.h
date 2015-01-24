#ifndef BMA220_H_
#define BMA220_H_

#include <Wire.h>

class BMA220 {

public:
  void init (uint8_t range);

  void getMonitor3(int8_t *ax, int8_t *ay, int8_t *az);

  int getDivi() { return divi_; }
private:
  int divi_;
  uint8_t version_[3];
};

#endif // BMA220_H_
