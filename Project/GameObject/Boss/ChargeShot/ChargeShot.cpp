#include "ChargeShot.h"

int ChargeShot::deadTime = 400;

void ChargeShot::Initialize() {
	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.x = -7.0f;
	worldTransform_.translation_.y = 2.0f;

	//タイマーの初期化
	deadTimer_ = deadTime;

	////衝突属性を設定
	//SetCollisionAttribute(kCollisionAttributeEnemy);
	//SetCollisionMask(kCollisionMaskEnemy);
	//SetCollisionPrimitive(kCollisionPrimitiveAABB);

	//AABB aabbSize{ {-scale.x,-scale.y,-scale.z},{scale.x,scale.y,scale.z} };
	//SetAABB(aabbSize);
}

void ChargeShot::Update() {
	worldTransform_.translation_.x += 0.05f;
	worldTransform_.scale_.y = 8.0f;


	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();


	//死亡フラグを立てる
	if (--deadTimer_ < 0) {
		isDead_ = true;
	}
}

void ChargeShot::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void ChargeShot::OnCollision() {

}

void ChargeShot::OnCollision(float damage) {

}

Vector3 ChargeShot::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}