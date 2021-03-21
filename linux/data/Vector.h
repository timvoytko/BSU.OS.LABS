#pragma once
#include"Number.h"

class Vector {
public:
  NumberLib::Number x;
  NumberLib::Number y;
  Vector(NumberLib::Number x, NumberLib::Number y);
  static const Vector add(Vector& v1, Vector& v2);
  NumberLib::Number get_polar_radius();
  NumberLib::Number get_polar_angle();
};
