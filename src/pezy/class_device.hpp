#ifndef CLASS_DEVICE
#define CLASS_DEVICE

//#include <pzc_vector.h>
#include <pzc_types.h>

class EpDev{
public:
  float x;
  float y;
  float z;
  float vx;
  float vy;
  float vz;
  unsigned int walk;
};

class ForceDev{
public:
  double x;
  double y;
  double z;
  double u;
  void clear(){
    x = y = z = u = 0.0;
  }
};

#endif
