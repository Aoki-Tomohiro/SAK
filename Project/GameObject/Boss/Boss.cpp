#include "Boss.h"
#include "State/BossStateNormal.h"
#include "State/BossStateLaserAttack.h"
#include "State/BossStateChargeShot.h"
#include "2D/ImGuiManager.h"

void Boss::Initialize() {

	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.y = 3.3f;
	//ボスの行動パターンの初期化
	state_ = std::make_unique<BossStateNormal>();
	state_->Initialize(this);

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);
}

void Boss::Update() {

	//状態の更新
	state_->Update(this);

	////死亡フラグの立ったレーザーをリストから削除
	//lasers_.remove_if([](std::unique_ptr<Laser>& laser) {
	//	if (laser->IsDead()) {
	//		laser.reset();
	//		return true;
	//	}
	//	return false;
	//});

	////レーザーの更新
	//for (std::unique_ptr<Laser>& laser : lasers_) {
	//	laser->Update();
	//}

	//死亡フラグの立ったチャージショットをリストから削除
	chargeShot_.remove_if([](std::unique_ptr<ChargeShot>& chargeShot) {
		if (chargeShot->IsDead()) {
			chargeShot.reset();
			return true;
		}
		return false;
	});

	//チャージショットの更新
	for (std::unique_ptr<ChargeShot>& chargeShot : chargeShot_) {
		chargeShot->Update();
	}

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("Boss");
	ImGui::Text("HP : %f", Hp_);
	ImGui::End();
}

void Boss::Draw(const ViewProjection& viewProjection) {

	//ボスのモデルの描画
	model_->Draw(worldTransform_, viewProjection);

	//状態の描画
	state_->Draw(this, viewProjection);

	////レーザーの描画
	//for (std::unique_ptr<Laser>& laser : lasers_) {
	//	laser->Draw(viewProjection);
	//}

	//レーザーの描画
	for (std::unique_ptr<ChargeShot>& chargeShot : chargeShot_) {
		chargeShot->Draw(viewProjection);
	}
}

void Boss::ChangeState(IBossState* state) {

	state_.reset(state);
	state_->Initialize(this);
}

void Boss::AddLaser(Laser* laser) {

	//レーザーをリストに追加
	lasers_.push_back(std::unique_ptr<Laser>(laser));
}

void Boss::AddChargeShot(ChargeShot* chargeShot) {

	//レーザーをリストに追加
	chargeShot_.push_back(std::unique_ptr<ChargeShot>(chargeShot));
}


void Boss::OnCollision() {

}

void Boss::OnCollision(float damage) {
	//体力を減らす
	Hp_ -= damage;
}

Vector3 Boss::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}