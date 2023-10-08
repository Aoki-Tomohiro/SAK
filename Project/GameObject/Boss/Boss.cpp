#include "Boss.h"
#include "State/BossStateNormal.h"

void Boss::Initialize() {
	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.y = 3.0f;
	worldTransform_.translation_.z = 10.0f;
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

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Boss::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void Boss::ChangeState(IBossState* state) {
	state_.reset(state);
	state_->Initialize(this);
}

void Boss::Move(const Vector3& velocity) {
	//移動処理
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
}

void Boss::OnCollision() {

}

Vector3 Boss::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}