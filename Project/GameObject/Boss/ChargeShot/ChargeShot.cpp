#include "ChargeShot.h"

void ChargeShot::Initialize() {
	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.x = -7.0f;
	worldTransform_.translation_.y = -1.0f;

	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 3.7f;
	worldTransform_.scale_.z = 0.5f;

	////衝突属性を設定
	//SetCollisionAttribute(kCollisionAttributeEnemy);
	//SetCollisionMask(kCollisionMaskEnemy);
	//SetCollisionPrimitive(kCollisionPrimitiveAABB);

	//AABB aabbSize{ {-scale.x,-scale.y,-scale.z},{scale.x,scale.y,scale.z} };
	//SetAABB(aabbSize);
}

void ChargeShot::Update() {
	worldTransform_.translation_.x += chargeShotSpeed_;


	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();


	//死亡フラグを立てる
	if (worldTransform_.translation_.x >= 5.8f) {
		isDead_ = true;
	}
}

void ChargeShot::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void ChargeShot::OnCollision(uint32_t collisionAttribute, float damage) {

}

Vector3 ChargeShot::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}