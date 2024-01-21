#pragma once
#include <cmath>
#include <cassert>

struct Vector2 {
	float x, y;
};

struct Vector3 {
	float x, y, z;
};

struct Vector4 {
	float x, y, z, w;
};

struct Matrix4x4 {
	float m[4][4];
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

/// <summary>
/// ベクトルの足し算
/// </summary>
/// <param name="v1">一つ目のベクトル</param>
/// <param name="v2">二つ目のベクトル</param>
/// <returns>足した結果</returns>
Vector3 Add(const Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクトルの引き算
/// </summary>
/// <param name="v1">一つ目のベクトル</param>
/// <param name="v2">二つ目のベクトル</param>
/// <returns>引いた結果</returns>
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクトルの掛け算
/// </summary>
/// <param name="v1">一つ目のベクトル</param>
/// <param name="v2">二つ目のベクトル</param>
/// <returns>積</returns>
Vector3 Multiply(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 長さを計算
/// </summary>
/// <param name="v">ベクトル</param>
/// <returns>長さ</returns>
float Length(const Vector3& v);

/// <summary>
/// 行列の引き算
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 行列の掛け算
/// </summary>
/// <param name="m1">1つ目の行列</param>
/// <param name="m2">2つ目の行列</param>
/// <returns>行列の積</returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 逆行列の計算
/// </summary>
/// <param name="m">行列</param>
/// <returns>逆行列</returns>
Matrix4x4 Inverse(const Matrix4x4& m);

/// <summary>
/// 単位行列の作成
/// </summary>
/// <returns>単位行列</returns>
Matrix4x4 MakeIdentity4x4();

/// <summary>
/// 平行移動行列の作成
/// </summary>
/// <param name="translate">座標</param>
/// <returns>平行移動行列</returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// <summary>
/// スケール行列の作成
/// </summary>
/// <param name="scale">スケール</param>
/// <returns>スケール行列</returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// <summary>
/// X軸回転行列の作成
/// </summary>
/// <param name="radian">角度</param>
/// <returns>X軸回転行列</returns>
Matrix4x4 MakeRotateXMatrix(float radian);

/// <summary>
/// Y軸回転行列の作成
/// </summary>
/// <param name="radian">角度</param>
/// <returns>Y軸回転行列</returns>
Matrix4x4 MakeRotateYMatrix(float radian);

/// <summary>
/// Z軸回転行列の作成
/// </summary>
/// <param name="radian">角度</param>
/// <returns>Z軸回転行列</returns>
Matrix4x4 MakeRotateZMatrix(float radian);

/// <summary>
/// アフィン行列の作成
/// </summary>
/// <param name="scale">スケール</param>
/// <param name="rotate">角度</param>
/// <param name="translate">座標</param>
/// <returns>アフィン行列</returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

/// <summary>
/// 透視投影行列の作成
/// </summary>
/// <param name="fovY">視野角</param>
/// <param name="aspectRatio">アスペクト比</param>
/// <param name="nearClip">近平面への距離</param>
/// <param name="farClip">遠平面への距離</param>
/// <returns>透視投影行列</returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

/// <summary>
/// 平行投影行列の作成
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
/// <param name="nearClip">近平面</param>
/// <param name="farClip">遠平面</param>
/// <returns></returns>
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

/// <summary>
/// 座標変換
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">ワールド行列</param>
/// <returns>回転成分</returns>
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);

/// <summary>
/// 線形補間
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
float Lerp(const float& v1, const float& v2, float t);

/// <summary>
/// 線形補間
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

Vector3 Multiply(const float& v1, const Vector3& v2);

Vector3 Normalize(const Vector3& v);