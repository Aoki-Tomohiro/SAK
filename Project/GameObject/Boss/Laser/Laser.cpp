#include "Laser.h"

void Laser::Initialize(const Vector3& position) {
	//ワールドトランスフォームの初期化
	worldTransform_.translation_ = position;
}

void Laser::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Laser::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}