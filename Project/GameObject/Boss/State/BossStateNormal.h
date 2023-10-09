#pragma once
#include "IBossState.h"
#include "3D/Matrix/WorldTransform.h"

class BossStateNormal : public IBossState {
public:
	//攻撃間隔
	static int AttackInterval;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossStateNormal() override;

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
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};

	//スピード
	static int moveDirection_;
	float moveSpeed_ = 0.06f;

	//レーザー攻撃のタイマー
	int nextAttackTimer_ = 0;

};