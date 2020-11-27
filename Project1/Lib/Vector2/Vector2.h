//-------------------------------------------------
//--------------Project by ------------------------
//----------------------koshiro kawanami-----------
//-------------------------------------------------d
#pragma once
#include <array>
#include <cmath>
template<class T>
class Vector2Tmp;

template<class T>
class Vector2Tmp
{
public:
	Vector2Tmp();
	Vector2Tmp(T x,T y);

	double Angle(T x, T y);			// 角度を求める
	double Sin(T angle);				// y軸のベクトルを求める
	double Cos(T angle);				// x軸のベクトルを求める

	T x, y;								// 座標格納用変数
	Vector2Tmp& operator=(const Vector2Tmp& vec2);
	Vector2Tmp& operator+=(Vector2Tmp& vec2);
	Vector2Tmp& operator-=(Vector2Tmp& vec2);
	Vector2Tmp& operator*=(Vector2Tmp& vec2);
	Vector2Tmp& operator/=(Vector2Tmp& vec2);

	//Vector2Tmp& operator=(const T K);
	//Vector2Tmp& operator+=(T K);
	//Vector2Tmp& operator-=(T K);
	Vector2Tmp& operator*=(const T& K);
	Vector2Tmp& operator/=(const T& K);

	bool operator==(Vector2Tmp& vec2);
	bool operator!=(Vector2Tmp& vec2);
	bool operator<(Vector2Tmp& vec2);
	bool operator<=(Vector2Tmp& vec2);
};
template<class T>
Vector2Tmp<T> operator+(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_);
template<class T>
Vector2Tmp<T> operator-(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_);
template<class T>
Vector2Tmp<T> operator*(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_);
template<class T>
Vector2Tmp<T> operator/(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_);
template<class T>
Vector2Tmp<T> operator%(const Vector2Tmp<T>& vec2, Vector2Tmp<T> vec2_);

template<class T>
Vector2Tmp<T> operator+(const Vector2Tmp<T>& vec2, T K);
template<class T>
Vector2Tmp<T> operator-(const Vector2Tmp<T>& vec2, T K);
template<class T>
Vector2Tmp<T> operator*(const Vector2Tmp<T>& vec2, T K);
template<class T>
Vector2Tmp<T> operator/(const Vector2Tmp<T>& vec2, T K);
template<class T>
Vector2Tmp<T> operator%(const Vector2Tmp<T>& vec2, T K);


template<class T>
bool operator==(const Vector2Tmp<T>& vec2, T k);
template<class T>
bool operator!=(Vector2Tmp<T>& vec2, T k);
template<class T>
bool operator<(Vector2Tmp<T>& vec2, T k);
template<class T>
bool operator<=(Vector2Tmp<T>& vec2, T k);

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
inline bool operator==(const Vector2Tmp<T>& vec2, T k)
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


using Vector2 = Vector2Tmp<int>;
using Vector2l = Vector2Tmp<long>;
using Vector2f = Vector2Tmp<float>;
using Vector2d = Vector2Tmp<double>;
using Vector2s = Vector2Tmp<short>;

using Position2=Vector2;
using Position2f=Vector2f;

