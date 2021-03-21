#include"Vector.h"
#include"Number.h"
#include <iostream>

int main() {
  NumberLib::Number n1(1);
  NumberLib::Number n2(3);
  NumberLib::Number n3(5);
  NumberLib::Number n4(7);
  NumberLib::Number sum = NumberLib::Number::add(n1, n2);
  NumberLib::Number divide = NumberLib::Number::divide(n3, n4);
  NumberLib::Number sub = NumberLib::Number::subtract(n4, n1);
  NumberLib::Number mult = NumberLib::Number::multiply(n2, n3);

  std::cout << sum.get_value() << "\n";
  std::cout << sub.get_value() << "\n";
  std::cout << mult.get_value() << "\n";
  std::cout << divide.get_value() << "\n";
  Vector v1 = Vector(n1, n2);
  Vector v2 = Vector(n3, n4);
  Vector sumv = Vector::add(v1, v2);
  std::cout << sumv.x.get_value();
  std::cout << (v1.get_polar_angle()).get_value();
}
