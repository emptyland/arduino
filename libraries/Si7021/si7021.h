#ifndef SI7021_H_
#define SI7021_H_

class Si7021 {
public:
  void init();

  void getMonitor2(double *temp, double *humidity);
};

#endif // SI7021_H_
