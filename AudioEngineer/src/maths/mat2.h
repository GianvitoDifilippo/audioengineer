#pragma once

#include <iostream>
#include <initializer_list>
#include "..\defines.h"

namespace audioengineer {
	namespace maths {

		struct mat2
		{
			float elements[4];

			mat2();
			mat2(float elements[4]);
			mat2(std::initializer_list<float> elements);

			float at(uint i, uint j) const;
			void transpose();
			mat2 transposed() const;

			float& operator()(uint i, uint j);
			mat2 operator+(const mat2& right) const;
			mat2 operator-(const mat2& right) const;
			mat2 operator|(const mat2& right) const;
			mat2 operator*(const mat2& right) const;
			friend mat2 operator*(const mat2& left, float right);
			friend mat2 operator/(const mat2& left, float right);
			friend mat2 operator*(float left, const mat2& right);
			mat2& operator+=(const mat2& right);
			mat2& operator-=(const mat2& right);
			mat2& operator|=(const mat2& right);
			mat2& operator*=(const mat2& right);
			mat2& operator^=(const mat2& right);
			mat2& operator*=(float right);
			mat2& operator/=(float right);
			friend std::ostream& operator<<(std::ostream& os, const mat2& mat);
		};
	}
}