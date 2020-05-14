#include "vec3.h"

namespace audioengineer {
	namespace maths {

		vec3::vec3()
		{
			x = 0.f;
			y = 0.f;
			z = 0.f;
		}

		vec3::vec3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		float vec3::length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		float vec3::length2() const
		{
			return x * x + y * y + z * z;
		}

		void vec3::normalize()
		{
			float l_inv = 1.f / length();
			x *= l_inv;
			y *= l_inv;
			z *= l_inv;
		}

		vec3 vec3::normalized() const
		{
			float l_inv = 1.f / length();
			return vec3(x * l_inv, y * l_inv, z * l_inv);
		}

		vec3 vec3::operator+(const vec3& right) const
		{
			return vec3(x + right.x, y + right.y, z + right.z);
		}

		vec3 vec3::operator-(const vec3& right) const
		{
			return vec3(x - right.x, y - right.y, z - right.z);
		}

		vec3 vec3::operator|(const vec3& right) const
		{
			return vec3(x * right.x, y * right.y, z * right.z);
		}

		vec3 operator*(const vec3& left, float right)
		{
			return vec3(left.x * right, left.y * right, left.z * right);
		}

		vec3 operator/(const vec3& left, float right)
		{
			return vec3(left.x / right, left.y / right, left.z / right);
		}

		vec3 operator*(float left, const vec3& right)
		{
			return vec3(left * right.x, left * right.y, left * right.z);
		}

		float vec3::operator*(const vec3& right) const
		{
			return x * right.x + y * right.y + z * right.z;
		}

		vec3& vec3::operator+=(const vec3& right)
		{
			x += right.x;
			y += right.y;
			z += right.z;
			return *this;
		}

		vec3& vec3::operator-=(const vec3& right)
		{
			x -= right.x;
			y -= right.y;
			z -= right.z;
			return *this;
		}

		vec3& vec3::operator|=(const vec3& right)
		{
			x *= right.x;
			y *= right.y;
			z *= right.z;
			return *this;
		}

		vec3& vec3::operator*=(float right)
		{
			x *= right;
			y *= right;
			z *= right;
			return *this;
		}

		vec3& vec3::operator/=(float right)
		{
			x /= right;
			y /= right;
			z /= right;
			return *this;
		}

		std::ostream& operator<<(std::ostream& os, const vec3& vec)
		{
			return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		}
	}
}