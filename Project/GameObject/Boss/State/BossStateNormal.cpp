#include "BossStateNormal.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateLaserAttack.h"
#include "2D/ImGuiManager.h"
#include "Utility/GlobalVariables.h"

//実態定義
int BossStateNormal::moveDirection_ = 1;
int BossStateNormal::AttackInterval = 300;

void BossStateNormal::Initialize(Boss* pBoss) {
	//攻撃間隔の初期化
	nextAttackTimer_ = AttackInterval;

	//グローバル変数
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Boss";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "moveSpeed", moveSpeed_);
	globalVariables->AddItem(groupName, "AttackInterval", AttackInterval);
}

void BossStateNormal::Update(Boss* pBoss) {
	//グローバル変数の適応
	BossStateNormal::ApplyGlobalVariables();

	//移動処理
	moveSpeed_ *= moveDirection_;
	pBoss->Move(Vector3{ moveSpeed_,0.0f,0.0f });

	//画面端まで移動したら移動方向を変える
	if (pBoss->GetWorldTransform()->translation_.x <= -7.0f || pBoss->GetWorldTransform()->translation_.x >= 7.0f) {
		moveDirection_ *= -1;
	}

	//レーザー攻撃状態に変更
	if (--nextAttackTimer_ < 0) {
		isLaserAttack_ = true;
	}

	if (isLaserAttack_) {
		pBoss->ChangeState(new BossStateLaserAttack());
	}
}

void BossStateNormal::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Boss";
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "moveSpeed");
	AttackInterval = globalVariables->GetIntValue(groupName, "AttackInterval");
}