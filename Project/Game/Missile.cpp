#include "missile.h"

#include "Utility/GlobalVariables.h"

void Missile::Initialize()
{
	model_[0].reset(Model::CreateFromOBJ("Resources", "sphere.obj"));
	model_[1].reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/white.png");

	input_ = Input::GetInstance();

	worldTransform_[0].translation_.x = 8.0f;
	worldTransform_[0].translation_.y = RandomTY();
	worldTransform_[0].translation_.z = 10.0f;
	worldTransform_[0].scale_ = { 0.5f,0.5f,0.5f };

	worldTransform_[1].translation_.x = -8.0f;
	worldTransform_[1].translation_.y = RandomTY2();
	worldTransform_[1].translation_.z = 10.0f;
	worldTransform_[1].scale_ = { 0.5f,0.5f,0.5f };

	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	//const char* groupName = "Missile";
	////グループを追加
	//globalVariables->CreateGroup(groupName);
	//globalVariables->AddItem(groupName, "missileMoveSpeed", missileMoveSpeed_);
}

void Missile::Update()
{
	
	worldTransform_[0].translation_.x -= missileMoveSpeed_;
	if (worldTransform_[0].translation_.x <= -9.0f)
	{
		worldTransform_[0].translation_.x = 8.0f;
		worldTransform_[0].translation_.y = RandomTY();
	}

	worldTransform_[1].translation_.x += missileMoveSpeed_;
	if (worldTransform_[1].translation_.x >= 9.0f)
	{
		worldTransform_[1].translation_.x = -8.0f;
		worldTransform_[1].translation_.y = RandomTY2();
	}


	worldTransform_[0].UpdateMatrix();
	worldTransform_[1].UpdateMatrix();

	/*Missile::ApplyGlobalVariables();*/

	ImGui::Begin("Missile");
	ImGui::Text("translationX %f", worldTransform_[0].translation_.x);
	ImGui::Text("translationY %f", worldTransform_[0].translation_.y);
	ImGui::Text("translationZ %f", worldTransform_[0].translation_.z);
	ImGui::End();
}

void Missile::Draw(const ViewProjection viewProjection)
{
	model_[0]->Draw(worldTransform_[0], viewProjection, textureHandle_);

	model_[1]->Draw(worldTransform_[1], viewProjection, textureHandle_);
}

//void Missile::ApplyGlobalVariables()
//{
//	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
//	const char* groupName = "Missile";
//	missileMoveSpeed_ = globalVariables->GetFloatValue(groupName, "missileMoveSpeed_");
//}

float Missile::RandomTY()
{
	float min = 2.5f;
	float max = 0.0f;

	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

float Missile::RandomTY2()
{
	float min = 0.0f;
	float max = -1.8f;

	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}