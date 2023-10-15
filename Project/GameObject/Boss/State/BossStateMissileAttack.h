#pragma once
#include "IBossState.h"

class BossStateMissileAttack : public IBossState{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossStateMissileAttack() override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pBoss"></param>
	void Initialize(Boss* pBoss) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pBoss"></param>
	void Update(Boss* pBoss) override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pBoss"></param>
	/// <param name="viewProjection"></param>
	void Draw(Boss* pBoss, const ViewProjection& viewProjection)override;

private:


};

