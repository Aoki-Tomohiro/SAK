#include "ChargeShot.h"

int ChargeShot::chargeShotEndTime = 240;

void ChargeShot::Initialize(Vector3 position, float chargeShotpeed) {
	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y = -1.0f;

	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 3.7f;
	worldTransform_.scale_.z = 0.5f;

	chargeShotSpeed_ = chargeShotpeed;
	deadTimer_ = chargeShotEndTime;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeLaser);
	SetCollisionMask(kCollisionMaskLaser);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabbSize{ {-worldTransform_.scale_.x,-worldTransform_.scale_.y,-worldTransform_.scale_.z},{worldTransform_.scale_.x,worldTransform_.scale_.y,worldTransform_.scale_.z} };
	SetAABB(aabbSize);
}

void ChargeShot::Update() {
	worldTransform_.translation_.x -= chargeShotSpeed_;


	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();


	//死亡フラグを立てる
	if (--deadTimer_ <= 0) 
	{
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