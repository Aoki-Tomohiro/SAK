#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Utility/CollisionManager/Collider.h"
#include "Utility/CollisionManager/CollisionConfig.h"

class ChargeShot : public Collider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
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
	/// 死亡フラグを取得
	/// </summary>
	/// <returns>死亡フラグ</returns>
	bool IsDead() { return isDead_; };

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="damage">ダメージ</param>
	void OnCollision(float damage) override;

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition() override;

private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//死亡フラグ
	bool isDead_ = false;
	int deadTimer_ = 0;
	//チャージショットのスケール
	Vector3 chargeShotScale_ = { 1.0f,1.0f,1.0f };
	//チャージショットの速さ
	float chargeShotSpeed_ = 0.05f;
};

