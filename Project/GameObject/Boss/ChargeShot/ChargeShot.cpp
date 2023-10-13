#include "ChargeShot.h"

int ChargeShot::chargeTime = 1200;

void ChargeShot::Initialize() {
	input_ = Input::GetInstance();

	//モデルの作成
	chargemodel_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	//ワールドトランスフォームの初期化
	chargeWorldTransform_.translation_.y = 2.0f;
	chargeWorldTransform_.scale_ = { 0.1f,0.1f,0.1f };

	//タイマーの初期化
	chargeTimer_ = chargeTime;

	//衝突属性を設定
	/*SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabbSize{ {-scale.x,-scale.y,-scale.z},{scale.x,scale.y,scale.z} };
	SetAABB(aabbSize);*/
}

void ChargeShot::Update() {

	/*if (input_->IsPushKey(DIK_C))
	{
		chargeWorldTransform_.scale_.x += 0.001f;
		chargeWorldTransform_.scale_.y += 0.001f;
		chargeWorldTransform_.scale_.z += 0.001f;
	}*/

	if (--chargeTimer_ > 0)
	{
		chargeWorldTransform_.scale_.x += 0.0008f;
		chargeWorldTransform_.scale_.y += 0.0008f;
		chargeWorldTransform_.scale_.z += 0.0008f;
	}

	/*if (chargeWorldTransform_.translation_.y < 0.0f) {
		chargeWorldTransform_.translation_.y = 0.0f;
	}*/

	//ワールドトランスフォームの更新
	chargeWorldTransform_.UpdateMatrix();

	ImGui::Begin("Game Play");
	ImGui::Text("chargeTimer : %d",chargeTimer_);
	ImGui::End();

	////死亡フラグを立てる
	//if (--deadTimer_ < 0) {
	//	isDead_ = true;
	//}
}

void ChargeShot::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	chargemodel_->Draw(chargeWorldTransform_, viewProjection);
}

void ChargeShot::OnCollision() {

}

void ChargeShot::OnCollision(float damage) {

}

Vector3 ChargeShot::GetWorldPosition() {
	Vector3 position{};
	position.x = chargeWorldTransform_.matWorld_.m[3][0];
	position.y = chargeWorldTransform_.matWorld_.m[3][1];
	position.z = chargeWorldTransform_.matWorld_.m[3][2];
	return position;
}