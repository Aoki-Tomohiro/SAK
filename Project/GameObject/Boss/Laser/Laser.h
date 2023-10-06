#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"

class Laser{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	void Initialize(const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};

};

