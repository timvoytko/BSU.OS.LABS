#include"pch.h"
#include"Vector.h"
#include<cmath>
Vector::Vector(NumberLib::Number x,NumberLib::Number y){
	this->x = NumberLib::Number(x.get_value());
	this->y = NumberLib::Number(y.get_value());
}

const Vector Vector::add(Vector& v1, Vector& v2){
	Vector sum(NumberLib::Number::add(v1.x, v2.x),
		NumberLib::Number::add(v1.y, v2.y));
	return sum;
}

NumberLib::Number Vector::get_polar_radius(){
	return NumberLib::Number(std::sqrt(std::pow(this->x.get_value(), 2)
		+ std::pow(this->y.get_value(), 2)));
}

NumberLib::Number Vector::get_polar_angle(){
	const double pi = 3.14159265358979323846; 
	if (this->x.get_value() > 0) {
		if (this->y.get_value() >= 0) {
			return NumberLib::Number(std::atan(this->y.get_value() / this->x.get_value()));
		}
		else {
			return NumberLib::Number(std::atan(this->y.get_value() / this->x.get_value()) + 2 * pi);
		}
	}
	else if (this->x.get_value() < 0) {
		return NumberLib::Number(std::atan(this->y.get_value() / this->x.get_value()) + pi);
	}
	else {
		if (this->y.get_value() > 0) {
			return NumberLib::Number(pi / 2);
		}
		else if (this->y.get_value() < 0) {
			return  NumberLib::Number(3 * pi / 2);
		}
		else {
			return NumberLib::Number(0);
		}
	}
}
