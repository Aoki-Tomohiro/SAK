#include "missile.h"
#include "Utility/GlobalVariables.h"

void Missile::Initialize(const Vector3& position, const float& speed)
{
	model_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/white.png");

	input_ = Input::GetInstance();

	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 0.3f,0.3f,0.3f };
	/*worldTransform_.scale_ = { 1.0f,1.0f,1.0f };*/

	missileMoveSpeed_ = speed;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabb = { { -worldTransform_.scale_.x,-worldTransform_.scale_.y,-worldTransform_.scale_.z}, { worldTransform_.scale_.x,worldTransform_.scale_.y,worldTransform_.scale_.z} };
	SetAABB(aabb);

	//worldTransform_[0].translation_.x = 8.0f;
	//worldTransform_[0].translation_.y = RandomTY(-1.3f, 1.8f);
	//worldTransform_[0].translation_.z = 10.0f;
	//worldTransform_[0].scale_ = { 0.3f,0.3f,0.3f };

	//worldTransform_[1].translation_.x = -8.0f;
	//worldTransform_[1].translation_.y = RandomTY(-1.3f, 1.8f);
	//worldTransform_[1].translation_.z = 10.0f;
	//worldTransform_[1].scale_ = { 0.3f,0.3f,0.3f };
}

void Missile::Update()
{
	if (isAlive_) 
	{
		if (IsFollowingWeapon_ == true && weapon_->GetIsAttack() == true)
		{
			IsMove_ = true;
		}
		else {
			worldTransform_.translation_.x += missileMoveSpeed_;
			IsFollowingWeapon_ = false;
			worldTransform_.UpdateMatrix();
		}

		if (IsMove_ == true)
		{
			worldTransform_.translation_.y += missileFollowingSpeed_;
			worldTransform_.UpdateMatrix();
		}
	}

	

	if (worldTransform_.translation_.x < -13.0f || worldTransform_.translation_.x > 13.0f || worldTransform_.translation_.y > 13.0f) {
		isAlive_ = false;
		IsMove_ = false;
	}
	else {
		isAlive_ = true;
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

void Missile::OnCollision() 
{
	IsFollowingWeapon_ = true;

	ImGui::Begin("Collision");
	ImGui::Text("MissileHit");
	ImGui::Text("IsFollowingWeapon %d", IsFollowingWeapon_);
	ImGui::End();
}

Vector3 Missile::GetWorldPosition() {
	Vector3 pos{};
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}
