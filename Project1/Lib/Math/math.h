#pragma once
#include <cmath>
#define PI  3.14159265358979323846264338327950288

#define DEGREE_TO_RADIOUS(angle) angle * PI / 180
#define RADIOUS_TO_DEGREE(radious)	radious*180/PI

template<class T>
class Vector2Tmp;

template<class T>
struct Matrix3x3Tmp
{
	T index[3][3];		// [row][column]
};

//
template<class T>
Matrix3x3Tmp<T> operator*(Matrix3x3Tmp<T> mat1, Matrix3x3Tmp<T> mat2);

template<class T>
Vector2Tmp<T> operator*(Vector2Tmp<T> vec, Matrix3x3Tmp<T> mat1);

template<class T>
Vector2Tmp<T> operator*( Matrix3x3Tmp<T> mat1, Vector2Tmp<T> vec);

// return IdentityMatrix
template<class T>
Matrix3x3Tmp<T> IdentityMat();

// return parallel transfer matrix
//@param x axis moving amount
//@param y axis moving amount
template<class T>
Matrix3x3Tmp<T> TranslateMatrix(T x,T y);

// return rotate matrix
//@param angle: rotation degree
template<class T>
Matrix3x3Tmp<T> RotateMatrix(T angle);

// return rotate matrix
//@param angle: rotation degree
template<class T>
Matrix3x3Tmp<T> ScaleMatrix(T sx, T sy);

template<class T>
Vector2Tmp<T> TranslatePosition(Vector2Tmp<T>, Vector2Tmp<T>);

template<class T>
Vector2Tmp<T> RotateCenterPositon(Vector2Tmp<T> Position, Vector2Tmp<T> centerPos, T angle);

template<class T>
Vector2Tmp<T> ScaleCenterPosition(Vector2Tmp<T> Position, Vector2Tmp<T> centerPos, Vector2Tmp<T> magnification);

