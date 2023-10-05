#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"

/// <summary>
/// ボス
/// </summary>
class Boss {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// グローバル変数の適応
	/// </summary>
	void ApplyGlobalVariables();

private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//スピード
	float moveDirection_ = 1.0f;
	float moveSpeed_ = 0.04f;
};