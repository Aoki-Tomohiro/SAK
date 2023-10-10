#pragma once
#include "Utility/MathFunction.h"

/// <summary>
/// ビュープロジェクション
/// </summary>
struct ViewProjection {

	/// <summary>
	/// ビュー行列の更新
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// プロジェクション行列の更新
	/// </summary>
	void UpdateProjectionMatrix();

	/// <summary>
	/// 行列の計算
	/// </summary>
	void UpdateMatrix();

	//角度
	Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	//座標
	Vector3 translation_ = { 0.0f,0.0f,-20.0f };
	//ビュー行列
	Matrix4x4 matView_{};
	//プロジェクション行列
	Matrix4x4 matProjection_{};
	//視野角
	float fov_ = 0.45f;
	//アスペクト比
	float aspectRatio_ = 1280.0f / 720.0f;
	//近平面への距離
	float nearClip_ = 0.1f;
	//遠平面への距離
	float farClip_ = 100.0f;

};