#include "Weapon.h"
#include "../GameObject/Player/Player.h"
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
	globalVariables->AddItem(groupName, "attackSpeedLevel3", attackSpeed_[3]);
	globalVariables->AddItem(groupName, "attackDamageNormal", attackDamage_[0]);
	globalVariables->AddItem(groupName, "attackDamageLevel1", attackDamage_[1]);
	globalVariables->AddItem(groupName, "attackDamageLevel2", attackDamage_[2]);
	globalVariables->AddItem(groupName, "attackDamageLevel3", attackDamage_[3]);
	globalVariables->AddItem(groupName, "chargeSpeed", chargeSpeed_);

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetRadius(weaponWorldTransform_.scale_.x);
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
	if (input_->IsPushKey(DIK_RETURN) && IsAttack_ == false && IsCoolDown_ == false)
	{
		pushCount_++;
		IsCharge_ = true;
	}

	if (input_->IsPushKeyExit(DIK_RETURN) && IsCoolDown_ == false)
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

		if (weaponWorldTransform_.translation_.y <= -2.3f)
		{
			weaponWorldTransform_.translation_.y = -2.3f;
		}
	}

	if (IsAttack_ == true && chargeCount_ < 20)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[0]);

		if (weaponWorldTransform_.translation_.y >= 2.2f)
		{
			weaponWorldTransform_.translation_.y = 2.2f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
		}
	}

	//チャージ1
	if (IsAttack_ == true && chargeCount_ >= 20 && chargeCount_ < 50)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[1]);

		if (weaponWorldTransform_.translation_.y >= 2.9f)
		{
			weaponWorldTransform_.translation_.y = 2.9f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
		}
	}

	//チャージ2
	if (IsAttack_ == true && chargeCount_ >= 50 && chargeCount_ < 90)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[2]);

		if (weaponWorldTransform_.translation_.y >= 3.5f)
		{
			weaponWorldTransform_.translation_.y = 3.5f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
		}
	}

	//チャージ3
	if (IsAttack_ == true && chargeCount_ >= 90)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[3]);

		if (weaponWorldTransform_.translation_.y >= 5.0f)
		{
			weaponWorldTransform_.translation_.y = 5.0f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
		}
	}

	//攻撃後のクールダウン
	if (IsCoolDown_ == true)
	{
		coolDownTimer_--;
		weaponWorldTransform_.translation_.y -= coolDownSpeed_;
		if (weaponWorldTransform_.translation_.y <= 1.8f)
		{
			weaponWorldTransform_.translation_.y = 1.8f;
			coolDownTimer_ = 60;
			IsCoolDown_ = false;
		}
	}

	weaponWorldTransform_.UpdateMatrix();

	Weapon::ApplyGlobalVariables();

	ImGui::Begin("PlayerWeapon");
	ImGui::Text("translationY %f", weaponWorldTransform_.translation_.y);
	ImGui::Text("attackSpeed %f", attackSpeed_);
	ImGui::Text("attackCount %d", chargeCount_);
	ImGui::Text("pushCount %d", pushCount_);
	ImGui::Text("attackDamage : %f", GetDamage());
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
	attackSpeed_[3] = globalVariables->GetFloatValue(groupName, "attackSpeedLevel3");
	attackDamage_[0] = globalVariables->GetFloatValue(groupName, "attackDamageNormal");
	attackDamage_[1] = globalVariables->GetFloatValue(groupName, "attackDamageLevel1");
	attackDamage_[2] = globalVariables->GetFloatValue(groupName, "attackDamageLevel2");
	attackDamage_[3] = globalVariables->GetFloatValue(groupName, "attackDamageLevel3");
	chargeSpeed_ = globalVariables->GetFloatValue(groupName, "chargeSpeed");
}

void Weapon::OnCollision() 
{

}

void Weapon::OnCollision(float damage)
{
	//プレイヤーにダメージを与える
	player_->OnCollision(damage);
}

Vector3 Weapon::GetWorldPosition()
{
	Vector3 pos;
	pos.x = weaponWorldTransform_.matWorld_.m[3][0];
	pos.y = weaponWorldTransform_.matWorld_.m[3][1];
	pos.z = weaponWorldTransform_.matWorld_.m[3][2];
	return pos;
}