#pragma once
#include "IBossState.h"
#include "2D/ImGuiManager.h"
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"

class BossStateChargeShot : public IBossState
{
	//チャージ中の時間
	static int chargeTime;
	static int LaserAttackEndTime;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossStateChargeShot() override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pBoss">ボスのポインタ</param>
	void Initialize(Boss* pBoss) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pBoss">ボスのポインタ</param>
	void Update(Boss* pBoss) override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pBoss">ボスのポインタ</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(Boss* pBoss, const ViewProjection& viewProjection) override;

	/// <summary>
	/// グローバル変数の適応
	/// </summary>
	void ApplyGlobalVariables() override;

private:
	//モデル
	std::unique_ptr<Model> chargemodel_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform chargeWorldTransform_{};
	WorldTransform bossWorldTransform_{};
	//チャージ中の時間
	int chargeTimer_ = 0;
	//攻撃終了までのタイマー
	int endTimer_ = 0;
	//チャージショットのスケール
	Vector3 chargeShotScale_ = { 1.0f,10.0f,1.0f };

	bool IsMove_ = false;
};

