#pragma once

#include <iostream>
#include "..\defines.h"

namespace audioengineer {
	namespace maths {

		struct vec3
		{
			float x, y, z;

			vec3();
			vec3(float x, float y, float z);

			float length() const;
			float length2() const;

			void normalize();
			vec3 normalized() const;

			vec3 operator+(const vec3& right) const;
			vec3 operator-(const vec3& right) const;
			vec3 operator|(const vec3& right) const;
			friend vec3 operator*(const vec3& left, float right);
			friend vec3 operator/(const vec3& left, float right);
			friend vec3 operator*(float left, const vec3& right);
			float operator*(const vec3& right) const;
			vec3& operator+=(const vec3& right);
			vec3& operator-=(const vec3& right);
			vec3& operator|=(const vec3& right);
			vec3& operator*=(float right);
			vec3& operator/=(float right);
			friend std::ostream& operator<<(std::ostream& os, const vec3& vec);
		};

	}
}