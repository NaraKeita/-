#include "MathUtilityForText.h"
#include <cmath>
#include <numbers>

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
	
    // 使用しないとエラーになるのでダミーにコピー
    Vector3 dm;
    dm = scale;
    dm = rot;

    //平行移動
	Matrix4x4 TranslateMat{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translate.x,
        translate.y,
        translate.z,
        1.0f
    };

   

    //回転
	Matrix4x4 RotateMatY{
        cosf(rot.y), 0, -sinf(rot.y), 0,
        0, 1, 0, 0,
        sinf(rot.y), 0, cosf(rot.y), 0,
        0, 0, 0, 1
    };

     // 回転
	Matrix4x4 RotateMatX{
        1, 0, 0, 0,
        0, cosf(rot.x), sinf(rot.x), 0,
        0, -sinf(rot.x), cosf(rot.x), 0,
        0, 0, 0, 1
    };
	// x軸回転*Y軸回転で回転行列を合成
	Matrix4x4 RotateMatAll = MatrixMultipty(RotateMatX, RotateMatY);
    //回転*平行移動だけをワールド変換行列に
	Matrix4x4 ansMat = MatrixMultipty(RotateMatAll, TranslateMat);

	return ansMat;
}

Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}

Matrix4x4 MatrixMultipty(Matrix4x4& m1, Matrix4x4& m2) { 
    Matrix4x4 result;
    
    result.m[0][0] = 
        m1.m[0][0] * m2.m[0][0] +
        m1.m[0][1] * m2.m[1][0] +
        m1.m[0][2] * m2.m[2][0] +
        m1.m[0][3] * m2.m[3][0];
	result.m[0][1] =
        m1.m[0][0] * m2.m[0][1] +
        m1.m[0][1] * m2.m[1][1] +
        m1.m[0][2] * m2.m[2][1] +
        m1.m[0][3] * m2.m[3][1];
	result.m[0][2] =
        m1.m[0][0] * m2.m[0][2] +
        m1.m[0][1] * m2.m[1][2] +
        m1.m[0][2] * m2.m[2][2] +
        m1.m[0][3] * m2.m[3][2];
	result.m[0][3] =
        m1.m[0][0] * m2.m[0][3] +
        m1.m[0][1] * m2.m[1][3] +
        m1.m[0][2] * m2.m[2][3] +
        m1.m[0][3] * m2.m[3][3];

    result.m[1][0] =
        m1.m[1][0] * m2.m[0][0] + 
        m1.m[1][1] * m2.m[1][0] + 
        m1.m[1][2] * m2.m[2][0] +
        m1.m[1][3] * m2.m[3][0];
	result.m[1][1] =
        m1.m[1][0] * m2.m[0][1] +
        m1.m[1][1] * m2.m[1][1] +
        m1.m[1][2] * m2.m[2][1] +
        m1.m[1][3] * m2.m[3][1];
	result.m[1][2] =
        m1.m[1][0] * m2.m[0][2] +
        m1.m[1][1] * m2.m[1][2] +
        m1.m[1][2] * m2.m[2][2] +
        m1.m[1][3] * m2.m[3][2];
	result.m[1][3] =
        m1.m[1][0] * m2.m[0][3] +
        m1.m[1][1] * m2.m[1][3] +
        m1.m[1][2] * m2.m[2][3] + 
        m1.m[1][3] * m2.m[3][3];

    result.m[2][0] =
        m1.m[2][0] * m2.m[0][0] +
        m1.m[2][1] * m2.m[1][0] + 
        m1.m[2][2] * m2.m[2][0] + 
        m1.m[2][3] * m2.m[3][0];
	result.m[2][1] =
        m1.m[2][0] * m2.m[0][1] +
        m1.m[2][1] * m2.m[1][1] +
        m1.m[2][2] * m2.m[2][1] +
        m1.m[2][3] * m2.m[3][1];
	result.m[2][2] =
        m1.m[2][0] * m2.m[0][2] +
        m1.m[2][1] * m2.m[1][2] +
        m1.m[2][2] * m2.m[2][2] + 
        m1.m[2][3] * m2.m[3][2];
	result.m[2][3] =
        m1.m[2][0] * m2.m[0][3] +
        m1.m[2][1] * m2.m[1][3] + 
        m1.m[2][2] * m2.m[2][3] + 
        m1.m[2][3] * m2.m[3][3];

    result.m[3][0] =
        m1.m[3][0] * m2.m[0][0] +
        m1.m[3][1] * m2.m[1][0] +
        m1.m[3][2] * m2.m[2][0] +
        m1.m[3][3] * m2.m[3][0];
	result.m[3][1] =
        m1.m[3][0] * m2.m[0][1] +
        m1.m[3][1] * m2.m[1][1] +
        m1.m[3][2] * m2.m[2][1] +
        m1.m[3][3] * m2.m[3][1];
	result.m[3][2] =
        m1.m[3][0] * m2.m[0][2] +
        m1.m[3][1] * m2.m[1][2] +
        m1.m[3][2] * m2.m[2][2] +
        m1.m[3][3] * m2.m[3][2];
	result.m[3][3] =
        m1.m[3][0] * m2.m[0][3] +
        m1.m[3][1] * m2.m[1][3] +
        m1.m[3][2] * m2.m[2][3] +
        m1.m[3][3] * m2.m[3][3];
    return result;
}

float EaseInOut(float x1, float x2, float t) {
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
    return Lerp(x1,x2,easedT);
}

float Lerp(float x1, float x2, float t) {return (1.0f - t) * x1 + t * x2;}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
    return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t));
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) { 
    Vector3 temp(v1);
	return temp += v2;
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
           (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
           (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);
}

Vector3& operator*=(Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}
