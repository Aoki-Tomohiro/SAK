#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Utility/GlobalVariables.h"

Player::~Player()
{
	delete hpSprite_;
}

void Player::Initialize(Weapon* weapon)
{
  
	playerModelDummy_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	platformModel_.reset(Model::CreateFromOBJ("Resources/Platform", "Platform.obj"));
  
	weapon_ = weapon;

	textureHandle_ = TextureManager::Load("Resources/uvChecker.png");

	input_ = Input::GetInstance();

	//Player
	playerWorldTransform_.translation_.x = 0.0f;
	playerWorldTransform_.translation_.y = -3.3f;
	playerWorldTransform_.scale_ = { 0.8f,0.8f,0.8f };

	playerWorldTransform_.UpdateMatrix();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "playerMoveSpeed", playerMoveSpeed_);


	//Motion
	platformMotion_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
	};

	NormalScale_ = { 1.0f,1.0f,1.0f };

	platformMotionMove_ = {
		0.1f,
		20,
		0,
		1,
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
	};

	ModelMotion();

	heartUI_ = {
		true,
		TextureManager::Load("Resources/heart.png"),
		{0.0f,0.0f},
		0.0f,
		{1.0f,1.0f},
	};

	hpSprite_->Sprite::Create(textureHandle_,{ 0.0f,0.0f});

}

void Player::Update() 
{
	//プレイヤーの左右移動
	if (input_->IsPushKey(DIK_A) && weapon_->GetIsAttack() == false)
	{
		playerWorldTransform_.translation_.x -= playerMoveSpeed_;

		if (playerWorldTransform_.translation_.x <= -7.3f)
		{
			playerWorldTransform_.translation_.x = -7.3f;
		}
	}

	if (input_->IsPushKey(DIK_D) && weapon_->GetIsAttack() == false)
	{
		playerWorldTransform_.translation_.x += playerMoveSpeed_;

		if (playerWorldTransform_.translation_.x >= 7.3f)
		{
			playerWorldTransform_.translation_.x = 7.3f;
		}
	}

	playerWorldTransform_.UpdateMatrix();
	ModelMotion();
	prePlayerTranslation_ = playerWorldTransform_.translation_;
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
	platformModel_->Draw(platformMotionWorldTransform_, viewProjection);

	//playerModelDummy_->Draw(playerWorldTransform_, viewProjection, textureHandle_);
}

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	playerMoveSpeed_ = globalVariables->GetFloatValue(groupName, "playerMoveSpeed");
}

void Player::ModelMotion()
{

	//MoveMotion
	{

		motionMode_ = Stay;

		if (prePlayerTranslation_.x != playerWorldTransform_.translation_.x) {
			motionMode_ = Move;
		}





		switch (motionMode_)
		{
		case Stay:
		default:

			platformMotion_.translation_ = {0.0f,0.0f ,0.0f };
			platformMotion_.rotation_ = { 0.0f,0.0f ,0.0f };
			platformMotion_.scale_ = { 1.0f,1.0f ,1.0f };

			break;
		case Move:
			//
			platformMotionMove_.time += platformMotionMove_.timePlus;

			if (platformMotionMove_.time >= platformMotionMove_.duration || platformMotionMove_.time <= 0) {
				platformMotionMove_.time = std::clamp(platformMotionMove_.time, 0.0f, platformMotionMove_.duration);
				platformMotionMove_.timePlus *= -1;
			}

			float t = platformMotionMove_.time / platformMotionMove_.duration;
			float ta = t < 0.5 ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2;

			platformMotionMove_.flex_.x = NormalScale_.x + platformMotionMove_.maxFlex_ * ta - platformMotionMove_.maxFlex_ * 0.5f;
			platformMotionMove_.flex_.y = NormalScale_.y + platformMotionMove_.maxFlex_ * (1.0f - ta) - platformMotionMove_.maxFlex_ * 0.5f;
			platformMotionMove_.flexPos_.y = (platformMotionMove_.maxFlex_ * ta) * -0.25f;

			platformMotion_.scale_ = platformMotionMove_.flex_;
			platformMotion_.translation_ = platformMotionMove_.flexPos_;

			break;
		}
	}

	platformMotionWorldTransform_.translation_ = Add(platformMotion_.translation_, playerWorldTransform_.translation_);
	platformMotionWorldTransform_.scale_ = Multiply(platformMotion_.scale_, playerWorldTransform_.scale_);
	platformMotionWorldTransform_.rotation_ = Add(platformMotion_.rotation_, playerWorldTransform_.rotation_);
	platformModel_->GetMaterial()->SetColor(platformMotion_.color_);

	platformMotionWorldTransform_.UpdateMatrix();
}

void Player::DrawSprite()
{
	//hpSprite_->Draw();
}

