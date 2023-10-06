#include "Player.h"
#include "Utility/GlobalVariables.h"

void Player::Initialize()
{
	model_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/uvChecker.png");

	input_ = Input::GetInstance();

	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = -3.3f;
	worldTransform_.translation_.z = 10.0f;

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "playerMoveSpeed", playerMoveSpeed_);
}

void Player::Update()
{
	if (input_->IsPushKey(DIK_A))
	{
		worldTransform_.translation_.x -= playerMoveSpeed_;
	}

	if (input_->IsPushKey(DIK_D))
	{
		worldTransform_.translation_.x += playerMoveSpeed_;
	}

	worldTransform_.UpdateMatrix();

	Player::ApplyGlobalVariables();

	ImGui::Begin("Player");
	ImGui::Text("translationX %f", worldTransform_.translation_.x);
	ImGui::Text("translationY %f", worldTransform_.translation_.y);
	ImGui::Text("translationZ %f", worldTransform_.translation_.z);
	ImGui::Text("A : moveLeft  D : moveRight");
	ImGui::End();
}

void Player::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Player::ApplyGlobalVariables() 
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	playerMoveSpeed_ = globalVariables->GetFloatValue(groupName, "playerMoveSpeed");
}