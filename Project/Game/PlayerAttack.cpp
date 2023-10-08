#include "PlayerAttack.h"

void PlayerAttack::Initialize()
{
	model_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/white.png");

	input_ = Input::GetInstance();

	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = -3.3f;
	worldTransform_.translation_.z = 10.0f;
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
}

void PlayerAttack::Update()
{
	if (input_->IsPushKey(DIK_RETURN))
	{
		count_++;
	}
	
	if(input_->IsPushKeyExit(DIK_RETURN))
	{
		IsUP = true;
	}

	if (IsUP == true && count_ < 80)
	{
		attackTimer--;
		worldTransform_.translation_.y += attackSpeed_;

		if (attackTimer <= 0)
		{
			count_ = 0;
			IsUP = false;
			IsDown = true;
		}
	}

	if (IsUP == true && count_ >= 80)
	{
		attackTimer--;
		worldTransform_.translation_.y += upAttackSpeed_;

		if (attackTimer <= 0)
		{
			count_ = 0;
			IsUP = false;
			IsDown = true;
		}
	}

	if (IsDown == true)
	{
		attackTimer = 60;
		worldTransform_.translation_.y -= attackSpeed_;
		if (worldTransform_.translation_.y <= -3.3f)
		{
			worldTransform_.translation_.y = -3.3f;
			IsDown = false;
		}
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("PlayerAttack");
	ImGui::Text("translationY %f", worldTransform_.translation_.y);
	ImGui::Text("attackSpeed %f", attackSpeed_);
	ImGui::Text("attackCount %d", count_);
	ImGui::Text("attackTimer %d", attackTimer);
	ImGui::End();
}

void PlayerAttack::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
