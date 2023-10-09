#include "Player.h"
#include "Utility/GlobalVariables.h"

void Player::Initialize()
{
	playerModel_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));
	weaponModel_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/uvChecker.png");

	input_ = Input::GetInstance();

	//Player
	playerWorldTransform_.translation_.x = 0.0f;
	playerWorldTransform_.translation_.y = -3.3f;
	playerWorldTransform_.translation_.z = 10.0f;
	playerWorldTransform_.scale_ = { 0.8f,0.8f,0.8f };

	//Weapon
	weaponWorldTransform_.translation_.x = 0.0f;
	weaponWorldTransform_.translation_.y = -2.3f;
	weaponWorldTransform_.translation_.z = 10.0f;
	weaponWorldTransform_.scale_ = { 0.4f,0.4f,0.4f };

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
		playerWorldTransform_.translation_.x -= playerMoveSpeed_;
		weaponWorldTransform_.translation_.x -= playerMoveSpeed_;
	}

	if (input_->IsPushKey(DIK_D))
	{
		playerWorldTransform_.translation_.x += playerMoveSpeed_;
		weaponWorldTransform_.translation_.x += playerMoveSpeed_;
	}

	if (input_->IsPushKey(DIK_RETURN) && weaponWorldTransform_.translation_.y <= -2.3f)
	{
		attackCount_++;
	}

	if (input_->IsPushKeyExit(DIK_RETURN) && weaponWorldTransform_.translation_.y <= -2.3f)
	{
		IsUP_ = true;
	}

	if (IsUP_ == true && attackCount_ < 40)
	{
		attackTimer--;
		weaponWorldTransform_.translation_.y += attackSpeed_[0];

		if (attackTimer <= 0)
		{
			IsUP_ = false;
			IsDown_ = true;
		}
	}

	if (IsUP_ == true && attackCount_ > 40 && attackCount_ < 80)
	{
		attackTimer--;
		weaponWorldTransform_.translation_.y += attackSpeed_[1];

		if (attackTimer <= 0)
		{
			IsUP_ = false;
			IsDown_ = true;
		}
	}

	if (IsUP_ == true && attackCount_ >= 80)
	{
		attackTimer--;
		weaponWorldTransform_.translation_.y += attackSpeed_[2];

		if (attackTimer <= 0)
		{
			IsUP_ = false;
			IsDown_ = true;
		}
	}

	if (IsDown_ == true)
	{
		attackTimer = 60;
		weaponWorldTransform_.translation_.y -= coolTimeSpeed_;
		if (weaponWorldTransform_.translation_.y <= -2.3f)
		{
			weaponWorldTransform_.translation_.y = -2.3f;
			attackCount_ = 0;
			IsDown_ = false;
		}
	}

	playerWorldTransform_.UpdateMatrix();
	weaponWorldTransform_.UpdateMatrix();

	Player::ApplyGlobalVariables();

	ImGui::Begin("Player");
	ImGui::Text("translationX %f", playerWorldTransform_.translation_.x);
	ImGui::Text("translationY %f", playerWorldTransform_.translation_.y);
	ImGui::Text("translationZ %f", playerWorldTransform_.translation_.z);
	ImGui::Text("A : moveLeft  D : moveRight");
	ImGui::End();

	ImGui::Begin("PlayerWeapon");
	ImGui::Text("translationY %f", weaponWorldTransform_.translation_.y);
	ImGui::Text("attackSpeed %f", attackSpeed_);
	ImGui::Text("attackCount %d", attackCount_);
	ImGui::Text("attackTimer %d", attackTimer);
	ImGui::End();
}

void Player::Draw(const ViewProjection viewProjection)
{
	playerModel_->Draw(playerWorldTransform_, viewProjection, textureHandle_);

	weaponModel_->Draw(weaponWorldTransform_, viewProjection);
}

void Player::ApplyGlobalVariables() 
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	playerMoveSpeed_ = globalVariables->GetFloatValue(groupName, "playerMoveSpeed");
}