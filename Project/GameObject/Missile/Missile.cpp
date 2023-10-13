#include "missile.h"
#include "Utility/GlobalVariables.h"

void Missile::Initialize(const Vector3& position, const float& speed)
{
	model_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/white.png");

	input_ = Input::GetInstance();

	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 0.3f,0.3f,0.3f };

	missileMoveSpeed_ = speed;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeMissile);
	SetCollisionMask(kCollisionMaskMissile);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabb = { { -worldTransform_.scale_.x,-worldTransform_.scale_.y,-worldTransform_.scale_.z}, { worldTransform_.scale_.x,worldTransform_.scale_.y,worldTransform_.scale_.z} };
	SetAABB(aabb);
}

void Missile::Update()
{
	if (isAlive_) 
	{
		worldTransform_.translation_.x += missileMoveSpeed_;
		worldTransform_.UpdateMatrix();
	}

	if (worldTransform_.translation_.x < -13.0f || worldTransform_.translation_.x > 13.0f || worldTransform_.translation_.y > 13.0f) {
		isAlive_ = false;
	}

	ImGui::Begin("Missile");
	ImGui::Text("translationX %f", worldTransform_.translation_.x);
	ImGui::Text("translationY %f", worldTransform_.translation_.y);
	ImGui::Text("translationZ %f", worldTransform_.translation_.z);
	ImGui::Text("isAlive %d", isAlive_);
	ImGui::Text("IsFollowingWeapon : %d", IsFollowingWeapon_);
	ImGui::End();
}

void Missile::Draw(const ViewProjection viewProjection)
{
	if (isAlive_) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
}

void Missile::OnCollision(uint32_t collisionAttribute, float damage)
{
	isAlive_ = false;

	ImGui::Begin("Collision");
	ImGui::Text("MissileHit");
	ImGui::Text("IsFollowingWeapon %d", IsFollowingWeapon_);
	ImGui::End();
}

Vector3 Missile::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}
