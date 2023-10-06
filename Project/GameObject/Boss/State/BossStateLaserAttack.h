#pragma once
#include "IBossState.h"

class BossStateLaserAttack : public IBossState{
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
	//攻撃終了までのタイマー
	int endTimer_ = 60;

};