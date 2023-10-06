#pragma once
#include "IBossState.h"

class BossStateNormal : public IBossState {
public:
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
	/// グローバル変数の適応
	/// </summary>
	void ApplyGlobalVariables() override;

private:
	//スピード
	static int moveDirection_;
	float moveSpeed_ = 0.06f;

	//レーザー攻撃のフラグ
	static int AttackInterval;
	int nextAttackTimer_ = AttackInterval;
	bool isLaserAttack_ = false;

};