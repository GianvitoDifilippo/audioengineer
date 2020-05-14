#pragma once

#include <iostream>
#include <initializer_list>
#include "..\defines.h"

namespace audioengineer {
	namespace maths {

		struct mat4
		{
			float elements[16];

			mat4();
			mat4(float elements[16]);
			mat4(std::initializer_list<float> elements);

			float at(uint i, uint j) const;
			void transpose();
			mat4 transposed() const;

			float& operator()(uint i, uint j);
			mat4 operator+(const mat4& right) const;
			mat4 operator-(const mat4& right) const;
			mat4 operator|(const mat4& right) const;
			mat4 operator*(const mat4& right) const;
			friend mat4 operator*(const mat4& left, float right);
			friend mat4 operator/(const mat4& left, float right);
			friend mat4 operator*(float left, const mat4& right);
			mat4& operator+=(const mat4& right);
			mat4& operator-=(const mat4& right);
			mat4& operator|=(const mat4& right);
			mat4& operator*=(const mat4& right);
			mat4& operator^=(const mat4& right);
			mat4& operator*=(float right);
			mat4& operator/=(float right);
			friend std::ostream& operator<<(std::ostream& os, const mat4& mat);
		};
	}
}