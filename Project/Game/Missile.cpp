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
	worldTransform_[1].translation_.y = RandomTY();
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
	for (int i = 0; i < 2; i++)
	{
		if (isAlive_[i] == 0)
		{
			isAlive_[i] = 1;
		}
	}

	if (isAlive_[0] == 1)
	{
		worldTransform_[0].translation_.x -= missileMoveSpeed_;
		worldTransform_[0].UpdateMatrix();
		if (worldTransform_[0].translation_.x <= -9.0f)
		{
			isAlive_[0] = 0;
			worldTransform_[0].translation_.x = 8.0f;
			worldTransform_[0].translation_.y = RandomTY();
		}
	}

	if (isAlive_[1] == 1)
	{
		worldTransform_[1].translation_.x += missileMoveSpeed_;
		worldTransform_[1].UpdateMatrix();
		if (worldTransform_[1].translation_.x >= 9.0f)
		{
			worldTransform_[1].translation_.x = -8.0f;
			worldTransform_[1].translation_.y = RandomTY();
		}
	}


	/*Missile::ApplyGlobalVariables();*/

	ImGui::Begin("Missile");
	ImGui::Text("translationX %f", worldTransform_[0].translation_.x);
	ImGui::Text("translationY %f", worldTransform_[0].translation_.y);
	ImGui::Text("translationZ %f", worldTransform_[0].translation_.z);
	ImGui::Text("isAlive %d", isAlive_[0]);
	ImGui::End();
}

void Missile::Draw(const ViewProjection viewProjection)
{
	for (int i = 0; i < 2; i++)
	{
		if (isAlive_[i] == 1)
		{
			model_[i]->Draw(worldTransform_[i], viewProjection, textureHandle_);
		}
	}
}

//void Missile::ApplyGlobalVariables()
//{
//	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
//	const char* groupName = "Missile";
//	missileMoveSpeed_ = globalVariables->GetFloatValue(groupName, "missileMoveSpeed_");
//}

float Missile::RandomTY()
{
	float min = -5.0f;
	float max = 5.0f;

	// srandを使って乱数のシードを設定する
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// 指定された範囲内でランダムな浮動小数点数を生成
	float random_fraction = static_cast<float>(std::rand()) / RAND_MAX;
	return random_fraction * (max - min) + min;
}

