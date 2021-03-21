#pragma once
#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif

#include"Number.h"

__declspec(dllexport) class Vector {
public:
	NumberLib::Number x;
	NumberLib::Number y;
	__declspec(dllexport) Vector(NumberLib::Number x, NumberLib::Number y);
	__declspec(dllexport) static const Vector add(Vector& v1, Vector& v2);
	__declspec(dllexport) NumberLib::Number get_polar_radius();
	__declspec(dllexport) NumberLib::Number get_polar_angle();
};