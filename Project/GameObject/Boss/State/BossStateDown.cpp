#include "BossStateDown.h"
#include "../GameObject/Boss/Boss.h"

BossStateDown::~BossStateDown() {

}

void BossStateDown::Initialize(Boss* pBoss) {
	//ワールドトランスフォームの初期化
	worldTransform_ = pBoss->GetWorldTransform();
	initTranslation_ = worldTransform_.translation_;
	initRotation_ = worldTransform_.rotation_;

	targetTranslation_.x = initTranslation_.x;
	targetTranslation_.z = initTranslation_.z;
}

void BossStateDown::Update(Boss* pBoss) {

	if (timer_ < 1.0f) {
		timer_ += 1.0f / kAnimationTime;
	}

	//移動処理
	worldTransform_.translation_ = Lerp(initTranslation_, targetTranslation_, timer_);

	//回転処理
	worldTransform_.rotation_ = Lerp(initRotation_, targetRotation_, timer_);

	//ワールドトランスフォームの更新
	pBoss->SetWorldTransform(worldTransform_);

	if (timer_ > 1.0f) {
		pBoss->SetIsDead();
	}
}

void BossStateDown::Draw(Boss* pBoss, const ViewProjection& viewProjection) {

}

void BossStateDown::ApplyGlobalVariables() {

}