#include "Weapon.h"
#include <Math.h>
#include "../GameObject/Player/Player.h"
#include "Utility/GlobalVariables.h"

//実体定義
int Weapon::InvincibleTime = 60;

Weapon::~Weapon()
{
	for (int i = 0; i < MaxHp_; i++) {
		delete heartUI_[i].sprite_;
	}
}

void Weapon::Initialize()
{
  
	weaponModelDummy_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));

	weaponModel_.reset(Model::CreateFromOBJ("Resources/Head", "Head.obj"));
	weaponRodModel_.reset(Model::CreateFromOBJ("Resources/Rod", "rod.obj"));

	involvedMissile_.reset(Model::CreateFromOBJ("Resources/Missile", "Missile.obj"));
  

	input_ = Input::GetInstance();

	//Weapon
	weaponWorldTransform_.translation_.x = 0.0f;
	weaponWorldTransform_.translation_.y = 1.8f;
	weaponWorldTransform_.scale_ = { 0.4f,0.4f,0.4f };
	
	//Missile
	involvedMissileWorldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	involvedMissileWorldTransform_.scale_ = { 0.3f,0.3f,0.3f };
	involvedMissileWorldTransform_.rotation_.z = 1.57f;

	weaponWorldTransform_.UpdateMatrix();

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
	globalVariables->AddItem(groupName, "InvincibleTime", InvincibleTime);

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetRadius(weaponWorldTransform_.scale_.x);

	//モーション

	weaponMotion_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
		{ 0.0f,-0.1f,0.0f },
		{ 1.7f,1.7f,1.7f },

	};

	weaponRodMotion_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
		{ 0.0f,0.0f,0.0f },
		{ 1.7f,(3.3f + 2.2f)  * 0.5f,1.7f },
	};


	chargeRotateSpeed_ = 0.2f;
	for (int i = 0; i < 4; i++) {
		attackRotateSpeed_[i] = i * 0.2f;;
	}

	ModelMotion();

	Hp_ = MaxHp_;


	//HP表示の隙間；
	float hpSpace = 16.0f;
	float hpSpriteSize = 64.0f;

	for (int i = 0; i < MaxHp_; i++) {
		heartUI_[i] = {
			true,
			TextureManager::Load("Resources/Images/heart.png"),
			{ hpSpace + (hpSpace + hpSpriteSize)* i ,float(WinApp::GetInstance()->kClientHeight) - hpSpace - hpSpriteSize},
			0.0f,
			{1.0f,1.0f},
			nullptr,
		};

		heartUI_[i].sprite_ = Sprite::Create(heartUI_[i].textureHandle_, heartUI_[i].position_);
	}
}

void Weapon::Update()
{

	//HP描画
	for (int i = 0; i < MaxHp_; i++) {
		heartUI_[i].isDraw_ = false;
	}
	for (int i = 0; i < Hp_; i++) {
		heartUI_[i].isDraw_ = true;
	}


	//プレイヤーの左右移動
	if (input_->IsPushKey(DIK_A) && IsAttack_ == false)
	{
		weaponWorldTransform_.translation_.x -= weaponMoveSpeed_;

		if (weaponWorldTransform_.translation_.x <= -7.3f)
		{
			weaponWorldTransform_.translation_.x = -7.3f;
		}
	}

	if (input_->IsPushKey(DIK_D) && IsAttack_ == false)
	{
		weaponWorldTransform_.translation_.x += weaponMoveSpeed_;

		if (weaponWorldTransform_.translation_.x >= 7.3f)
		{
			weaponWorldTransform_.translation_.x = 7.3f;
		}
	}

	//攻撃処理
	if (input_->IsPushKey(DIK_SPACE) && IsAttack_ == false && IsCoolDown_ == false)
	{
		pushCount_++;
		IsCharge_ = true;
	}

	if (input_->IsPushKeyExit(DIK_SPACE) && IsCoolDown_ == false)
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
		SetDamage(attackDamage_[0] + involvedCount_ * 2.0f);

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
		SetDamage(attackDamage_[1] + involvedCount_ * 2.0f);

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
		SetDamage(attackDamage_[2] + involvedCount_ * 2.0f);

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
		SetDamage(attackDamage_[3] + involvedCount_ * 2.0f);

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
			IsHit_ = false;
			/*isInvolvedMissile_ = false;*/
			/*involvedCount_ = 0;*/
			involvedMissile_->GetMaterial()->SetColor(missileColor_[involvedCount_]);
		}
	}

	//巻き込んだミサイルの処理
	if (isInvolvedMissile_)
	{
		involvedMissileWorldTransform_.translation_ = {
				weaponWorldTransform_.translation_.x,
				weaponWorldTransform_.translation_.y + 0.5f,
				weaponWorldTransform_.translation_.z,
		};
		involvedMissile_->GetMaterial()->SetColor(missileColor_[involvedCount_ - 1]);
	}

	weaponWorldTransform_.UpdateMatrix();
  
	ModelMotion();
  
	involvedMissileWorldTransform_.UpdateMatrix();

	//無敵時間の処理
	if (invincibleFlag_) {
		invincibleTimer_--;
		if (invincibleTimer_ < 0) {
			invincibleFlag_ = false;
		}
	}

  
	Weapon::ApplyGlobalVariables();

	ImGui::Begin("PlayerWeapon");
	ImGui::Text("translationY %f", weaponWorldTransform_.translation_.y);
	ImGui::Text("attackSpeed %f", attackSpeed_);
	ImGui::Text("attackCount %d", chargeCount_);
	ImGui::Text("pushCount %d", pushCount_);
	ImGui::Text("attackDamage : %f", GetDamage());
	ImGui::Text("attackDamage0 : %f", attackDamage_[0]);
	ImGui::Text("attackDamage1 : %f", attackDamage_[1]);
	ImGui::Text("attackDamage2 : %f", attackDamage_[2]);
	ImGui::Text("attackDamage3 : %f", attackDamage_[3]);
	ImGui::Text("HP : %d", Hp_);
	ImGui::Text("InvolvedMissileCount : %d", involvedCount_);
	ImGui::End();
}

void Weapon::Draw(const ViewProjection viewProjection)
{
  
	weaponModel_->Draw(weaponMotionWorldTransform_, viewProjection);
	weaponRodModel_->Draw(weaponRodMotionWorldTransform_, viewProjection);
	//weaponModelDummy_->Draw(weaponWorldTransform_, viewProjection);

	//ミサイルを巻き込んでいるときに描画する
	if (isInvolvedMissile_) {
		involvedMissile_->Draw(involvedMissileWorldTransform_, viewProjection);
	}
  
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
	InvincibleTime = globalVariables->GetIntValue(groupName, "InvincibleTime");
}

void Weapon::OnCollision(uint32_t collisionAttribute, float damage)
{
	//衝突相手がボスの場合
	if (collisionAttribute & kCollisionAttributeEnemy)
	{
		IsHit_ = true;
		isInvolvedMissile_ = false;
		involvedCount_ = 0;
	}
	//ボス以外の場合
	else
	{
		//攻撃中の場合
		if (IsAttack_)
		{
			//衝突相手がミサイルの場合カウントを増やす
			if (collisionAttribute & kCollisionAttributeMissile) 
			{
				isInvolvedMissile_ = true;
				if (involvedCount_ < 5) {
					involvedCount_++;
				}
				involvedMissileWorldTransform_.translation_ = {
				weaponWorldTransform_.translation_.x,
				weaponWorldTransform_.translation_.y + 0.5f,
				weaponWorldTransform_.translation_.z,
				};
				involvedMissileWorldTransform_.UpdateMatrix();
			}
			//ミサイル以外の場合はダメージを食らう
			else {
				//無敵状態でなければダメージを食らう
				if (invincibleFlag_ == false)
				{
					invincibleFlag_ = true;
					invincibleTimer_ = InvincibleTime;
					Hp_ -= int(damage);
				}
			}
		}
		//攻撃中ではない場合
		else
		{
			//無敵状態でなければダメージを食らう
			if (invincibleFlag_ == false)
			{
				invincibleFlag_ = true;
				invincibleTimer_ = InvincibleTime;
				Hp_ -= int(damage);
			}
		}
	}
}

Vector3 Weapon::GetWorldPosition()
{
	Vector3 pos;
	pos.x = weaponWorldTransform_.matWorld_.m[3][0];
	pos.y = weaponWorldTransform_.matWorld_.m[3][1];
	pos.z = weaponWorldTransform_.matWorld_.m[3][2];
	return pos;
}

void Weapon::ModelMotion()
{

	motionMode_ = Stay;

	if (IsCharge_ == true && chargeCount_< 90) {
		motionMode_ = Charge;
	}else if (IsAttack_ == true) {
		motionMode_ = Attack;
	}

	switch (motionMode_)
	{
	default:
		case  Stay:

		break;
		case  Charge:
			weaponMotion_.rotation_.y += chargeRotateSpeed_;
			break;
		case  Attack:

			if (chargeCount_ < 20)
			{
				weaponMotion_.rotation_.y += attackRotateSpeed_[0];
			}
			else if (chargeCount_ >= 20 && chargeCount_ < 50)
			{
				weaponMotion_.rotation_.y += attackRotateSpeed_[1];
			}
			else if (chargeCount_ >= 50 && chargeCount_ < 90)
			{
				weaponMotion_.rotation_.y += attackRotateSpeed_[2];
			}
			else if (chargeCount_ >= 90 )
			{
				weaponMotion_.rotation_.y += attackRotateSpeed_[3];
			}
			break;
	}

	//ヘッドとプラットフォームまでの距離
	float distance = weaponWorldTransform_.translation_.y -playerPosY;
	float distanceHalf = distance * 0.5f;

	weaponRodMotion_.translation_.y = -distanceHalf;

	weaponRodMotion_.scale_.y = distance;

	weaponMotionWorldTransform_.translation_ = Add(Add(weaponMotion_.translation_, weaponWorldTransform_.translation_), weaponMotion_.normalTransration_);
	weaponMotionWorldTransform_.scale_ = Multiply(Multiply(weaponMotion_.scale_, weaponWorldTransform_.scale_), weaponMotion_.normalScale_);
	weaponMotionWorldTransform_.rotation_ = Add(weaponMotion_.rotation_, weaponWorldTransform_.rotation_);
	weaponModel_->GetMaterial()->SetColor(weaponMotion_.color_);

	weaponRodMotionWorldTransform_.translation_ = Add(Add(weaponRodMotion_.translation_, weaponWorldTransform_.translation_), weaponRodMotion_.normalTransration_);
	weaponRodMotionWorldTransform_.scale_ = Multiply(Multiply(weaponRodMotion_.scale_, weaponWorldTransform_.scale_), weaponRodMotion_.normalScale_);
	weaponRodMotionWorldTransform_.rotation_ = Add(weaponRodMotion_.rotation_, weaponWorldTransform_.rotation_);
	weaponRodModel_->GetMaterial()->SetColor(weaponRodMotion_.color_);

	weaponMotionWorldTransform_.UpdateMatrix();
	weaponRodMotionWorldTransform_.UpdateMatrix();
}

void Weapon::DrawSprite()
{
	for (int i = 0; i < MaxHp_; i++) {
		if (heartUI_[i].isDraw_ == true) {
			heartUI_[i].sprite_->Draw();
		}
	}
}

