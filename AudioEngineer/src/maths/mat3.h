#pragma once

#include <iostream>
#include <initializer_list>
#include "..\defines.h"

namespace audioengineer {
	namespace maths {

		struct mat3
		{
			float elements[9];

			mat3();
			mat3(float elements[9]);
			mat3(std::initializer_list<float> elements);

			float at(uint i, uint j) const;
			void transpose();
			mat3 transposed() const;

			float& operator()(uint i, uint j);
			mat3 operator+(const mat3& right) const;
			mat3 operator-(const mat3& right) const;
			mat3 operator|(const mat3& right) const;
			mat3 operator*(const mat3& right) const;
			friend mat3 operator*(const mat3& left, float right);
			friend mat3 operator/(const mat3& left, float right);
			friend mat3 operator*(float left, const mat3& right);
			mat3& operator+=(const mat3& right);
			mat3& operator-=(const mat3& right);
			mat3& operator|=(const mat3& right);
			mat3& operator*=(const mat3& right);
			mat3& operator^=(const mat3& right);
			mat3& operator*=(float right);
			mat3& operator/=(float right);
			friend std::ostream& operator<<(std::ostream& os, const mat3& mat);
		};
	}
}