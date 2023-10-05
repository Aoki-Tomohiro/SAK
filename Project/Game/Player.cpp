#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();

	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = -3.3f;
	worldTransform_.translation_.z = 10.0f;
}

void Player::Update()
{
	if (input_->IsPushKey(DIK_A))
	{
		worldTransform_.translation_.x -= 0.05f;
	}

	if (input_->IsPushKey(DIK_D))
	{
		worldTransform_.translation_.x += 0.05f;
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::Text("translationX %f", worldTransform_.translation_.x);
	ImGui::Text("translationY %f", worldTransform_.translation_.y);
	ImGui::Text("translationZ %f", worldTransform_.translation_.z);
	ImGui::End();
}

void Player::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
