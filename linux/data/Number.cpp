#include"Number.h"

namespace NumberLib
{
    Number::Number() {
        this->value = 0;
    }
    Number::Number(double value) {
        this->value = value;
    }
    const double Number::get_value() {
        return this->value;
    }
    Number Number::add(Number v1, Number v2){
        return Number(v1.get_value() + v2.get_value());
    }

    Number Number::subtract(Number v1, Number v2){
        return Number(v1.get_value()-v2.get_value());
    }

    Number Number::multiply(Number v1, Number v2){
        return Number(v1.get_value() * v2.get_value());
    }

    Number Number::divide(Number v1, Number v2){
        return Number(v1.get_value() / v2.get_value());
    }
}
