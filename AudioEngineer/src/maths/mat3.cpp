#include "mat3.h"

namespace audioengineer {
	namespace maths {

		mat3::mat3()
		{
			for (int i = 0; i < 9; i++) elements[i] = 0.f;
		}

		mat3::mat3(float elements[9])
		{
			for (int i = 0; i < 9; i++) this->elements[i] = elements[i];
		}

		mat3::mat3(std::initializer_list<float> elements)
		{
			const float* ptr = elements.begin();
			for (int i = 0; i < 9; i++) this->elements[i] = *ptr++;
		}

		float mat3::at(uint i, uint j) const
		{
			return elements[i * 3 + j];
		}

		void mat3::transpose()
		{
			for (int i = 1; i < 3; i++) {
				for (int j = 0; j < i; j++) {
					float temp = elements[i * 3 + j];
					elements[i * 3 + j] = elements[j * 3 + i];
					elements[j * 3 + i] = temp;
				}
			}
		}

		mat3 mat3::transposed() const
		{
			mat3 result(*this);
			result.transpose();
			return result;
		}

		float& mat3::operator()(uint i, uint j)
		{
			return elements[i * 3 + j];
		}

		mat3 mat3::operator+(const mat3& right) const
		{
			mat3 result;
			for (int i = 0; i < 9; i++) result.elements[i] = elements[i] + right.elements[i];
			return result;
		}

		mat3 mat3::operator-(const mat3& right) const
		{
			mat3 result;
			for (int i = 0; i < 9; i++) result.elements[i] = elements[i] - right.elements[i];
			return result;
		}

		mat3 mat3::operator|(const mat3& right) const
		{
			mat3 result;
			for (int i = 0; i < 9; i++) result.elements[i] = elements[i] * right.elements[i];
			return result;
		}

		mat3 mat3::operator*(const mat3& right) const
		{
			mat3 result;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 3; k++) {
						result(i, j) += at(i, k) * right.at(k, j);
					}
				}
			}
			return result;
		}

		mat3 operator*(const mat3& left, float right)
		{
			mat3 result;
			for (int i = 0; i < 9; i++) result.elements[i] = left.elements[i] * right;
			return result;
		}

		mat3 operator/(const mat3& left, float right)
		{
			mat3 result;
			for (int i = 0; i < 9; i++) result.elements[i] = left.elements[i] / right;
			return result;
		}

		mat3 operator*(float left, const mat3& right)
		{
			mat3 result;
			for (int i = 0; i < 9; i++) result.elements[i] = left * right.elements[i];
			return result;
		}

		mat3& mat3::operator+=(const mat3& right)
		{
			for (int i = 0; i < 9; i++) elements[i] += right.elements[i];
			return *this;
		}

		mat3& mat3::operator-=(const mat3& right)
		{
			for (int i = 0; i < 9; i++) elements[i] -= right.elements[i];
			return *this;
		}

		mat3& mat3::operator|=(const mat3& right)
		{
			for (int i = 0; i < 9; i++) elements[i] *= right.elements[i];
			return *this;
		}

		mat3& mat3::operator*=(const mat3& right)
		{
			float new_elements[9];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					new_elements[i * 3 + j] = 0.f;
					for (int k = 0; k < 3; k++) {
						new_elements[i * 3 + j] += elements[i * 3 + k] * right.elements[k * 3 + j];
					}
				}
			}
			for (int i = 0; i < 9; i++) elements[i] = new_elements[i];
			return *this;
		}

		mat3& mat3::operator^=(const mat3& right)
		{
			float new_elements[9];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					new_elements[i * 3 + j] = 0.f;
					for (int k = 0; k < 3; k++) {
						new_elements[i * 3 + j] += right.elements[i * 3 + k] * elements[k * 3 + j];
					}
				}
			}
			for (int i = 0; i < 9; i++) elements[i] = new_elements[i];
			return *this;
		}

		mat3& mat3::operator*=(float right)
		{
			for (int i = 0; i < 9; i++) elements[i] *= right;
			return *this;
		}

		mat3& mat3::operator/=(float right)
		{
			for (int i = 0; i < 9; i++) elements[i] /= right;
			return *this;
		}

		std::ostream& operator<<(std::ostream& os, const mat3& mat)
		{
			return os
				<< "[" << mat.elements[0] << ", " << mat.elements[1] << ", " << mat.elements[2] << "]"
				<< "[" << mat.elements[3] << ", " << mat.elements[4] << ", " << mat.elements[5] << "]"
				<< "[" << mat.elements[6] << ", " << mat.elements[7] << ", " << mat.elements[8] << "]";
		}
	}
}