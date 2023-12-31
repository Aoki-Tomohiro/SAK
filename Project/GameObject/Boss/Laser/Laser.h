#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Utility/CollisionManager/Collider.h"
#include "Utility/CollisionManager/CollisionConfig.h"
#include "3D/Particle/ParticleSystem.h"

class Laser : public Collider {
public:
	//レーザーが消えるまでの時間
	static int deadTime;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	void Initialize(const Vector3& position, const Vector3& scale);

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
	void OnCollision(uint32_t collisionAttribute, float damage) override;

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// パーティクルシステムを設定
	/// </summary>
	/// <param name="particleSystem"></param>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }

private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//死亡フラグ
	bool isDead_ = false;
	int deadTimer_ = 0;
	//レーザーのスケール
	Vector3 laserScale_ = { 1.0f,10.0f,1.0f };
	//レーザーの速さ
	float laserSpeed_ = 2.0f;
	//パーティクルシステム
	ParticleSystem* particleSystem_ = nullptr;
};

