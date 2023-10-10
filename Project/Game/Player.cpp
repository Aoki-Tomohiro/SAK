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
	playerWorldTransform_.scale_ = { 0.8f,0.8f,0.8f };

	//Weapon
	weaponWorldTransform_.translation_.x = 0.0f;
	weaponWorldTransform_.translation_.y = 1.8f;
	weaponWorldTransform_.scale_ = { 0.4f,0.4f,0.4f };

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "playerMoveSpeed", playerMoveSpeed_);
	globalVariables->AddItem(groupName, "attackSpeedNormal", attackSpeed_[0]);
	globalVariables->AddItem(groupName, "attackSpeedLevel1", attackSpeed_[1]);
	globalVariables->AddItem(groupName, "attackSpeedLevel2", attackSpeed_[2]);
	globalVariables->AddItem(groupName, "chargeSpeed", chargeSpeed_);
}

void Player::Update() 
{
	//プレイヤーの左右移動
	if (input_->IsPushKey(DIK_A))
	{
		playerWorldTransform_.translation_.x -= playerMoveSpeed_;
		weaponWorldTransform_.translation_.x -= playerMoveSpeed_;

		if (playerWorldTransform_.translation_.x <= -7.3f)
		{
			playerWorldTransform_.translation_.x = -7.3f;
			weaponWorldTransform_.translation_.x = -7.3f;
		}
	}

	if (input_->IsPushKey(DIK_D))
	{
		playerWorldTransform_.translation_.x += playerMoveSpeed_;
		weaponWorldTransform_.translation_.x += playerMoveSpeed_;

		if (playerWorldTransform_.translation_.x >= 7.3f)
		{
			playerWorldTransform_.translation_.x = 7.3f;
			weaponWorldTransform_.translation_.x = 7.3f;
		}
	}


	//プレイヤーの攻撃処理
	if (input_->IsPushKey(DIK_RETURN) && IsAttack_ == false)
	{
		pushCount_++;
		IsCharge_ = true;
	}

	if (input_->IsPushKeyExit(DIK_RETURN))
	{
		if (pushCount_ < 10)
		{
			IsCharge_ = false;
			IsAttack_ = true;
			pushCount_ = 0;
		}

		if (pushCount_ >= 10)
		{
			IsCharge_ = false;
			pushCount_ = 0;
		}
	}

	if (IsCharge_ == true)
	{
		chargeCount_++;
		weaponWorldTransform_.translation_.y -= chargeSpeed_;

		if (weaponWorldTransform_.translation_.y <= -1.8f)
		{
			weaponWorldTransform_.translation_.y = -1.8f;
		}
	}

	if (IsAttack_ == true && chargeCount_ < 45)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[0];

		if (weaponWorldTransform_.translation_.y >= 1.8f)
		{
			weaponWorldTransform_.translation_.y = 1.8f;
			chargeCount_ = 0;
			IsAttack_ = false;
		}
	}

	//チャージ1
	if (IsAttack_ == true && chargeCount_ >= 45 && chargeCount_ < 90)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[1];

		if (weaponWorldTransform_.translation_.y >= 1.8f)
		{
			weaponWorldTransform_.translation_.y = 1.8f;
			chargeCount_ = 0;
			IsAttack_ = false;
		}
	}

	//チャージ2
	if (IsAttack_ == true && chargeCount_ >= 90)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[2];

		if (weaponWorldTransform_.translation_.y >= 1.8f)
		{
			weaponWorldTransform_.translation_.y = 1.8f;
			chargeCount_ = 0;
			IsAttack_ = false;
		}
	}

	////チャージ3
	//if (IsDown_ == true && attackCount_ >= 90)
	//{
	//	weaponWorldTransform_.translation_.y += attackSpeed_[3];

	//	if (weaponWorldTransform_.translation_.y >= 2.0f)
	//	{
	//		weaponWorldTransform_.translation_.y = 2.0f;
	//		IsDown_ = false;
	//	}
	//}

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
	ImGui::Text("attackCount %d", chargeCount_);
	ImGui::Text("pushCount %d", pushCount_);
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
	attackSpeed_[0] = globalVariables->GetFloatValue(groupName, "attackSpeedNormal");
	attackSpeed_[1] = globalVariables->GetFloatValue(groupName, "attackSpeedLevel1");
	attackSpeed_[2] = globalVariables->GetFloatValue(groupName, "attackSpeedLevel2");
	chargeSpeed_ = globalVariables->GetFloatValue(groupName, "chargeSpeed");
}