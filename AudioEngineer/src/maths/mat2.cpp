#include "mat2.h"

namespace audioengineer {
	namespace maths {

		mat2::mat2()
		{
			for (int i = 0; i < 4; i++) elements[i] = 0.f;
		}

		mat2::mat2(float elements[4])
		{
			for (int i = 0; i < 4; i++) this->elements[i] = elements[i];
		}

		mat2::mat2(std::initializer_list<float> elements)
		{
			const float* ptr = elements.begin();
			for (int i = 0; i < 4; i++) this->elements[i] = *ptr++;
		}

		float mat2::at(uint i, uint j) const
		{
			return elements[i * 2 + j];
		}

		void mat2::transpose()
		{
			float temp = elements[1];
			elements[1] = elements[2];
			elements[2] = temp;
		}

		mat2 mat2::transposed() const
		{
			mat2 result(*this);
			result.transpose();
			return result;
		}

		float& mat2::operator()(uint i, uint j)
		{
			return elements[i * 2 + j];
		}

		mat2 mat2::operator+(const mat2& right) const
		{
			mat2 result;
			for (int i = 0; i < 4; i++) result.elements[i] = elements[i] + right.elements[i];
			return result;
		}

		mat2 mat2::operator-(const mat2& right) const
		{
			mat2 result;
			for (int i = 0; i < 4; i++) result.elements[i] = elements[i] - right.elements[i];
			return result;
		}

		mat2 mat2::operator|(const mat2& right) const
		{
			mat2 result;
			for (int i = 0; i < 4; i++) result.elements[i] = elements[i] * right.elements[i];
			return result;
		}

		mat2 mat2::operator*(const mat2& right) const
		{
			mat2 result;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					for (int k = 0; k < 2; k++) {
						result(i, j) += at(i, k) * right.at(k, j);
					}
				}
			}
			return result;
		}

		mat2 operator*(const mat2& left, float right)
		{
			mat2 result;
			for (int i = 0; i < 4; i++) result.elements[i] = left.elements[i] * right;
			return result;
		}

		mat2 operator/(const mat2& left, float right)
		{
			mat2 result;
			for (int i = 0; i < 4; i++) result.elements[i] = left.elements[i] / right;
			return result;
		}

		mat2 operator*(float left, const mat2& right)
		{
			mat2 result;
			for (int i = 0; i < 4; i++) result.elements[i] = left * right.elements[i];
			return result;
		}

		mat2& mat2::operator+=(const mat2& right)
		{
			for (int i = 0; i < 4; i++) elements[i] += right.elements[i];
			return *this;
		}

		mat2& mat2::operator-=(const mat2& right)
		{
			for (int i = 0; i < 4; i++) elements[i] -= right.elements[i];
			return *this;
		}

		mat2& mat2::operator|=(const mat2& right)
		{
			for (int i = 0; i < 4; i++) elements[i] *= right.elements[i];
			return *this;
		}

		mat2& mat2::operator*=(const mat2& right)
		{
			float new_elements[4];
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					new_elements[i * 2 + j] = 0.f;
					for (int k = 0; k < 2; k++) {
						new_elements[i * 2 + j] += elements[i * 2 + k] * right.elements[k * 2 + j];
					}
				}
			}
			for (int i = 0; i < 4; i++) elements[i] = new_elements[i];
			return *this;
		}

		mat2& mat2::operator^=(const mat2& right)
		{
			float new_elements[4];
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					new_elements[i * 2 + j] = 0.f;
					for (int k = 0; k < 2; k++) {
						new_elements[i * 2 + j] += right.elements[i * 2 + k] * elements[k * 2 + j];
					}
				}
			}
			for (int i = 0; i < 4; i++) elements[i] = new_elements[i];
			return *this;
		}

		mat2& mat2::operator*=(float right)
		{
			for (int i = 0; i < 4; i++) elements[i] *= right;
			return *this;
		}

		mat2& mat2::operator/=(float right)
		{
			for (int i = 0; i < 4; i++) elements[i] /= right;
			return *this;
		}

		std::ostream& operator<<(std::ostream& os, const mat2& mat)
		{
			return os
				<< "[" << mat.elements[0] << ", " << mat.elements[1] << "]"
				<< "[" << mat.elements[2] << ", " << mat.elements[3] << "]";
		}
	}
}