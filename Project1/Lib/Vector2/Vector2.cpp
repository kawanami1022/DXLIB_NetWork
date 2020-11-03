#pragma once
#include "../Math/math.h"
#include "Vector2.h"

template<class T> Vector2Tmp<T>::Vector2Tmp()
{
	x = 0;
	y = 0;
}

template<class T> Vector2Tmp<T>::Vector2Tmp(T x, T y)
{
	this->x = x;
	this->y = y;
}


// 目的:デカルト座標系での角度を求める
//入力	:x	x座標
//		:y	y座標
template<class T>double Vector2Tmp<T>::calcAngle(T x, T y)
{
	// 0°～90°の時
	if (0 < x && 0 < y)
		return std::atan(static_cast<double>(y / x)) * 180.0 / PI;

	// 90°～180°の時  180°～270°の時
	if (x < 0 && 0 < y || x < 0.0 && y < 0.0)
		return 180.0 + std::atan(static_cast<double>(y / x)) * 180.0 / PI;
	// 270°～360°の時
	else { return 360.0 + std::atan(static_cast<double>(y / x)) * 180.0 / PI; }
}

//目的	:
//入力	:angle
template<class T>double Vector2Tmp<T>::calcSin(T angle)
{
	return std::sin(angle * (PI / 180));
}

//目的	:
//入力	:angle
template<class T>double Vector2Tmp<T>::calcCos(T angle)
{
	return std::cos(angle * (PI / 180));
}


template<class T>
Vector2Tmp<T>& Vector2Tmp<T>::operator=(const Vector2Tmp& vec2)
{
	// TODO: return ステートメントをここに挿入します
	x = vec2.x;
	y = vec2.y;
	return (*this);
}

template<class T>
Vector2Tmp<T>& Vector2Tmp<T>::operator+=(Vector2Tmp& vec2)
{
	// TODO: return ステートメントをここに挿入します
	this->x += vec2.x;
	this->y += vec2.y;
	return (*this);
}

template<class T>
Vector2Tmp<T>& Vector2Tmp<T>::operator-=(Vector2Tmp& vec2)
{
	// TODO: return ステートメントをここに挿入します
	this->x -= vec2.x;
	this->y -= vec2.y;
	return (*this);
}

template<class T>
Vector2Tmp<T>& Vector2Tmp<T>::operator*=(Vector2Tmp& vec2)
{
	// TODO: return ステートメントをここに挿入します
	this->x *= vec2.x;
	this->y *= vec2.y;
	return (*this);
}

template<class T>
Vector2Tmp<T>& Vector2Tmp<T>::operator/=(Vector2Tmp& vec2)
{
	// TODO: return ステートメントをここに挿入します
	this->x /= vec2.x;
	this->y /= vec2.y;
	return (*this);
}

template<class T>
inline bool Vector2Tmp<T>::operator==(Vector2Tmp& vec2)
{
	return ((this->x == vec2.x) && (this->y == vec2.y));
}
template<class T>
inline bool Vector2Tmp<T>::operator!=(Vector2Tmp& vec2)
{
	return((this->x != vec2.x) || (this->y != vec2.y));;
}
template<class T>
inline bool Vector2Tmp<T>::operator<(Vector2Tmp& vec2)
{
	return false;
}

template<class T>
inline bool Vector2Tmp<T>::operator<=(Vector2Tmp& vec2)
{
	return((this->x <= vec2.x) || (this->y <= vec2.y));;
}

template<class T>
Vector2Tmp<T> operator+(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_)
{
	// TODO: return ステートメントをここに挿入します
	return { vec2.x + vec2_.x, vec2.y + vec2_.y };
}

template<class T>
Vector2Tmp<T> operator-(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_)
{
	return { vec2.x - vec2_.x, vec2.y - vec2_.y };
}

template<class T>
inline Vector2Tmp<T> operator*(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_)
{
	return { vec2.x * vec2_.x, vec2.y * vec2_.y };
}

template<class T>
inline Vector2Tmp<T> operator/(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_)
{
	return { vec2.x / vec2_.x, vec2.y / vec2_.y };
}

template<class T>
inline Vector2Tmp<T> operator%(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_)
{
	return { vec2.x % vec2_.x, vec2.y % vec2_.y };
}

template<class T>
inline Vector2Tmp<T> operator+(const Vector2Tmp<T>& vec2, T K)
{
	return { vec2.x + K, vec2.y + K };
}

template<class T>
inline Vector2Tmp<T> operator-(const Vector2Tmp<T>& vec2, T K)
{
	return { vec2.x - K, vec2.y - K };
}

template<class T>
inline Vector2Tmp<T> operator*(const Vector2Tmp<T>& vec2, T K)
{
	return { vec2.x * K, vec2.y * K };
}

template<class T>
inline Vector2Tmp<T> operator/(const Vector2Tmp<T>& vec2, T K)
{
	return { vec2.x / K, vec2.y / K };
}

template<class T>
inline Vector2Tmp<T> operator%(const Vector2Tmp<T>& vec2, T K)
{
	return { vec2.x % K, vec2.y % K };
}

template<class T>
inline bool operator==(Vector2Tmp<T>& vec2, T k)
{
	return (vec2.x == k && vec2.y == k);
}

template<class T>
inline bool operator!=(Vector2Tmp<T>& vec2, T k)
{
	return false;
}

template<class T>
inline bool operator<(Vector2Tmp<T>& vec2, T k)
{
	return (vec2.x < k || vec2.y < k);
}

template<class T>
inline bool operator<=(Vector2Tmp<T>& vec2, T k)
{
	return (vec2.x <= k && vec2.y <= k);
}


template class Vector2Tmp<char>;
template class Vector2Tmp<wchar_t>;
template class Vector2Tmp<char16_t>;
template class Vector2Tmp<char32_t>;
template class Vector2Tmp<unsigned char>;
template class Vector2Tmp<short>;
template class Vector2Tmp<unsigned short>;
template class Vector2Tmp<int>;
template class Vector2Tmp<__int8>;
template class Vector2Tmp<__int16>;
template class Vector2Tmp<__int32>;
template class Vector2Tmp<__int64>;
template class Vector2Tmp<unsigned int>;
template class Vector2Tmp<long>;
template class Vector2Tmp<long long>;
template class Vector2Tmp<float>;
template class Vector2Tmp<double>;
template class Vector2Tmp<long double>;

