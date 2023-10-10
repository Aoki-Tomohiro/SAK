#include "Player.h"
#include "Utility/GlobalVariables.h"

void Player::Initialize()
{
	playerModel_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/uvChecker.png");

	input_ = Input::GetInstance();

	//Player
	playerWorldTransform_.translation_.x = 0.0f;
	playerWorldTransform_.translation_.y = -3.3f;
	playerWorldTransform_.scale_ = { 0.8f,0.8f,0.8f };

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "playerMoveSpeed", playerMoveSpeed_);

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
}

void Player::Update() 
{
	//プレイヤーの左右移動
	if (input_->IsPushKey(DIK_A))
	{
		playerWorldTransform_.translation_.x -= playerMoveSpeed_;

		if (playerWorldTransform_.translation_.x <= -7.3f)
		{
			playerWorldTransform_.translation_.x = -7.3f;
		}
	}

	if (input_->IsPushKey(DIK_D))
	{
		playerWorldTransform_.translation_.x += playerMoveSpeed_;

		if (playerWorldTransform_.translation_.x >= 7.3f)
		{
			playerWorldTransform_.translation_.x = 7.3f;
		}
	}

	playerWorldTransform_.UpdateMatrix();

	Player::ApplyGlobalVariables();

	ImGui::Begin("Player");
	ImGui::Text("translationX %f", playerWorldTransform_.translation_.x);
	ImGui::Text("translationY %f", playerWorldTransform_.translation_.y);
	ImGui::Text("translationZ %f", playerWorldTransform_.translation_.z);
	ImGui::Text("A : moveLeft  D : moveRight");
	ImGui::End();
}

void Player::Draw(const ViewProjection viewProjection)
{
	playerModel_->Draw(playerWorldTransform_, viewProjection, textureHandle_);
}

void Player::ApplyGlobalVariables() 
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	playerMoveSpeed_ = globalVariables->GetFloatValue(groupName, "playerMoveSpeed");
}

void Player::OnCollision() {
	ImGui::Begin(" ");
	ImGui::Text("PlayerHit");
	ImGui::End();
}

Vector3 Player::GetWorldPosition() {
	Vector3 pos;
	pos.x = playerWorldTransform_.matWorld_.m[3][0];
	pos.y = playerWorldTransform_.matWorld_.m[3][1];
	pos.z = playerWorldTransform_.matWorld_.m[3][2];
	return pos;
}