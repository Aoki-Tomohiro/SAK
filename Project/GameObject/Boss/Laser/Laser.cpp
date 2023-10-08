#include "Laser.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"

void Laser::Initialize(const Vector3& position) {
	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Cube", "cube.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_ = position;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);
}

void Laser::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Laser::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void Laser::OnCollision() {}

Vector3 Laser::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}