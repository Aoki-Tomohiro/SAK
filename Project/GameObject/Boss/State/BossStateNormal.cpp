#include "BossStateNormal.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateLaserAttack.h"
#include "BossStateChargeShot.h"
#include "Utility/GlobalVariables.h"

//実態定義
int BossStateNormal::moveDirection_ = 1;
int BossStateNormal::AttackInterval = 300;

BossStateNormal::~BossStateNormal() {

}

void BossStateNormal::Initialize(Boss* pBoss) {
	//ワールドトランスフォームの初期化
	worldTransform_ = pBoss->GetWorldTransform();
	//攻撃間隔の初期化
	nextAttackTimer_ = AttackInterval;
	lazerAttackTimer_ = AttackInterval;
	chargeShotTimer_ = AttackInterval;

	//グローバル変数
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "BossStateNormal";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "MoveSpeed", moveSpeed_);
	globalVariables->AddItem(groupName, "AttackInterval", AttackInterval);
}

void BossStateNormal::Update(Boss* pBoss) {

	//グローバル変数の適応
	BossStateNormal::ApplyGlobalVariables();

	//移動処理
	moveSpeed_ *= moveDirection_;
	worldTransform_.translation_ = Add(worldTransform_.translation_, Vector3{ moveSpeed_,0.0f,0.0f });

	//画面端まで移動したら移動方向を変える
	if (worldTransform_.translation_.x <= -7.0f || worldTransform_.translation_.x >= 7.0f) {
		moveDirection_ *= -1;
	}

	//ワールドトランスフォームを設定
	pBoss->SetWorldTransform(worldTransform_);

	//チャージショット状態に変更
	if (--nextAttackTimer_ < 0)
	{
		pBoss->ChangeState(new BossStateChargeShot());
	}

	////レーザー攻撃状態に変更
	//if (--lazerAttackTimer_ < 0) {
	//	pBoss->ChangeState(new BossStateLaserAttack());
	//}
}

void BossStateNormal::Draw(Boss* pBoss, const ViewProjection& viewProjection) {

}

void BossStateNormal::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "BossStateNormal";
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "MoveSpeed");
	AttackInterval = globalVariables->GetIntValue(groupName, "AttackInterval");
}