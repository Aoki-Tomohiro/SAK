#include "Weapon.h"
#include "Utility/GlobalVariables.h"

void Weapon::Initialize()
{
	weaponModel_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));

	input_ = Input::GetInstance();

	//Weapon
	weaponWorldTransform_.translation_.x = 0.0f;
	weaponWorldTransform_.translation_.y = 1.8f;
	weaponWorldTransform_.scale_ = { 0.4f,0.4f,0.4f };

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Weapon";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "attackSpeedNormal", attackSpeed_[0]);
	globalVariables->AddItem(groupName, "attackSpeedLevel1", attackSpeed_[1]);
	globalVariables->AddItem(groupName, "attackSpeedLevel2", attackSpeed_[2]);
	globalVariables->AddItem(groupName, "chargeSpeed", chargeSpeed_);
}

void Weapon::Update()
{
	//プレイヤーの左右移動
	if (input_->IsPushKey(DIK_A))
	{
		weaponWorldTransform_.translation_.x -= weaponMoveSpeed_;

		if (weaponWorldTransform_.translation_.x <= -7.3f)
		{
			weaponWorldTransform_.translation_.x = -7.3f;
		}
	}

	if (input_->IsPushKey(DIK_D))
	{
		weaponWorldTransform_.translation_.x += weaponMoveSpeed_;

		if (weaponWorldTransform_.translation_.x >= 7.3f)
		{
			weaponWorldTransform_.translation_.x = 7.3f;
		}
	}


	//攻撃処理
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

	weaponWorldTransform_.UpdateMatrix();

	Weapon::ApplyGlobalVariables();

	ImGui::Begin("PlayerWeapon");
	ImGui::Text("translationY %f", weaponWorldTransform_.translation_.y);
	ImGui::Text("attackSpeed %f", attackSpeed_);
	ImGui::Text("attackCount %d", chargeCount_);
	ImGui::Text("pushCount %d", pushCount_);
	ImGui::End();
}

void Weapon::Draw(const ViewProjection viewProjection)
{
	weaponModel_->Draw(weaponWorldTransform_, viewProjection);
}

void Weapon::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Weapon";
	attackSpeed_[0] = globalVariables->GetFloatValue(groupName, "attackSpeedNormal");
	attackSpeed_[1] = globalVariables->GetFloatValue(groupName, "attackSpeedLevel1");
	attackSpeed_[2] = globalVariables->GetFloatValue(groupName, "attackSpeedLevel2");
	chargeSpeed_ = globalVariables->GetFloatValue(groupName, "chargeSpeed");
}