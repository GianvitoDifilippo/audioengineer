#include "vec2.h"

namespace audioengineer {
	namespace maths {

		vec2::vec2()
		{
			x = 0.f;
			y = 0.f;
		}

		vec2::vec2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		float vec2::length() const
		{
			return sqrt(x * x + y * y);
		}

		float vec2::length2() const
		{
			return x * x + y * y;
		}

		void vec2::normalize()
		{
			float l_inv = 1.f / length();
			x *= l_inv;
			y *= l_inv;
		}

		vec2 vec2::normalized() const
		{
			float l_inv = 1.f / length();
			return vec2(x * l_inv, y * l_inv);
		}

		vec2 vec2::operator+(const vec2& right) const
		{
			return vec2(x + right.x, y + right.y);
		}

		vec2 vec2::operator-(const vec2& right) const
		{
			return vec2(x - right.x, y - right.y);
		}

		vec2 vec2::operator|(const vec2& right) const
		{
			return vec2(x * right.x, y * right.y);
		}

		vec2 operator*(const vec2& left, float right)
		{
			return vec2(left.x * right, left.y * right);
		}

		vec2 operator/(const vec2& left, float right)
		{
			return vec2(left.x / right, left.y / right);
		}

		vec2 operator*(float left, const vec2& right)
		{
			return vec2(left * right.x, left * right.y);
		}

		float vec2::operator*(const vec2& right) const
		{
			return x * right.x + y * right.y;
		}

		vec2& vec2::operator+=(const vec2& right)
		{
			x += right.x;
			y += right.y;
			return *this;
		}

		vec2& vec2::operator-=(const vec2& right)
		{
			x -= right.x;
			y -= right.y;
			return *this;
		}

		vec2& vec2::operator|=(const vec2& right)
		{
			x *= right.x;
			y *= right.y;
			return *this;
		}

		vec2& vec2::operator*=(float right)
		{
			x *= right;
			y *= right;
			return *this;
		}

		vec2& vec2::operator/=(float right)
		{
			x /= right;
			y /= right;
			return *this;
		}

		std::ostream& operator<<(std::ostream& os, const vec2& vec)
		{
			return os << "(" << vec.x << ", " << vec.y << ")";
		}
	}
}