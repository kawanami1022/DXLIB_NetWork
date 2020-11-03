#include <valarray>
#include <cstring>
#include "../Constraint/cast.h"
#include "../Vector2/Vector2.h"
#include "math.h"


template<class T>
Matrix3x3Tmp<T> operator* (Matrix3x3Tmp<T> mat1, Matrix3x3Tmp<T> mat2)
{
	Matrix3x3Tmp<T> tmp;
	// Zero clear.
	std::memset(&tmp, 0, sizeof(tmp));

	for (int row = 0; row < 3; row++)
		for (int column = 0; column < 3; column++)
			for (int idx = 0; idx < 3; idx++)
				tmp.index[row][column] += mat1.index[row][idx] * mat2.index[idx][column];

	return tmp;
}

template<class T>
inline Vector2Tmp<T> operator*(Vector2Tmp<T> vec, Matrix3x3Tmp<T> mat1)
{
	return { vec.x * mat1.index[0][0] + vec.y * mat1.index[0][1] + mat1.index[0][2],
		vec.x * mat1.index[1][0] + vec.y * mat1.index[1][1] + mat1.index[1][2] };
}

template<class T>
inline Vector2Tmp<T> operator*(Matrix3x3Tmp<T> mat1, Vector2Tmp<T> vec)
{
	return { vec.x * mat1.index[0][0] + vec.y * mat1.index[0][1] + mat1.index[0][2],
		vec.x * mat1.index[1][0] + vec.y * mat1.index[1][1] + mat1.index[1][2] };
}

template<class T>
inline Matrix3x3Tmp<T> IdentityMat()
{
	return { { STCT(1),STCT(0),STCT(0) },
			{ STCT(0),STCT(1),STCT(0) },
			{ STCT(0),STCT(0),STCT(1) } };
}

template<class T>
inline Matrix3x3Tmp<T> TranslateMatrix(T x, T y)
{
	Matrix3x3Tmp<T> tmpMat =
	{ STCT(1.0),STCT(0.0),x ,
		STCT(0.0),STCT(1.0),y ,
		STCT(0.0),STCT(0.0),STCT(1.0) };

	return tmpMat;
}

/// <param name="angle">	input degrees</param>
/// <returns>rotateMatrix</returns>
template<class T>
inline Matrix3x3Tmp<T> RotateMatrix(T angle)
{
	return { STCT(std::cos(DEGREE_TO_RADIOUS(angle))),STCT(-std::sin(DEGREE_TO_RADIOUS(angle))),STCT(0.0),
			STCT(std::sin(DEGREE_TO_RADIOUS(angle))),STCT(std::cos(DEGREE_TO_RADIOUS(angle))),STCT(0.0) ,
			STCT(0.0),STCT(0.0),STCT(1.0) };
}


template<class T>
inline Matrix3x3Tmp<T> ScaleMatrix(T sx, T sy)
{
	return { sx,STCT(0.0),STCT(0.0),
			STCT(0.0),sy,STCT(0.0) ,
			STCT(0.0),STCT(0.0),STCT(1.0) };
}

template<class T>
inline Vector2Tmp<T> TranslatePosition(Vector2Tmp<T> position, Vector2Tmp<T> offset)
{
	return position * TranslateMatrix(offset.x, offset.y);
}

template<class T>
inline Vector2Tmp<T> RotateCenterPositon(Vector2Tmp<T> Position, Vector2Tmp<T> centerPos, T angle)
{
	return	Position * TranslateMatrix(-centerPos.x, -centerPos.y) * RotateMatrix(angle) * TranslateMatrix(centerPos.x, centerPos.y);
}

template<class T>
inline Vector2Tmp<T> ScaleCenterPosition(Vector2Tmp<T> Position, Vector2Tmp<T> centerPos, Vector2Tmp<T> magnification)
{
	return Position * TranslateMatrix(-centerPos.x, -centerPos.y) * ScaleMatrix(magnification.x, magnification.y) * TranslateMatrix(centerPos.x, centerPos.y);
}

template struct Matrix3x3Tmp<char>;
template struct Matrix3x3Tmp<wchar_t>;
template struct Matrix3x3Tmp<char16_t>;
template struct Matrix3x3Tmp<char32_t>;
template struct Matrix3x3Tmp<unsigned char>;
template struct Matrix3x3Tmp<short>;
template struct Matrix3x3Tmp<unsigned short>;
template struct Matrix3x3Tmp<int>;
template struct Matrix3x3Tmp<__int8>;
template struct Matrix3x3Tmp<__int16>;
template struct Matrix3x3Tmp<__int32>;
template struct Matrix3x3Tmp<__int64>;
template struct Matrix3x3Tmp<unsigned int>;
template struct Matrix3x3Tmp<long>;
template struct Matrix3x3Tmp<long long>;
template struct Matrix3x3Tmp<float>;
template struct Matrix3x3Tmp<double>;
template struct Matrix3x3Tmp<long double>;
