#include "missile.h"
#include "Utility/GlobalVariables.h"

void Missile::Initialize()
{
	model_[0].reset(Model::CreateFromOBJ("Resources", "sphere.obj"));
	model_[1].reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/white.png");

	input_ = Input::GetInstance();

	worldTransform_[0].translation_.x = 8.0f;
	worldTransform_[0].translation_.y = RandomTY(-1.3f, 1.8f);
	worldTransform_[0].translation_.z = 10.0f;
	worldTransform_[0].scale_ = { 0.3f,0.3f,0.3f };

	worldTransform_[1].translation_.x = -8.0f;
	worldTransform_[1].translation_.y = RandomTY(-1.3f, 1.8f);
	worldTransform_[1].translation_.z = 10.0f;
	worldTransform_[1].scale_ = { 0.3f,0.3f,0.3f };

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Missile";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "missileMoveSpeed", missileMoveSpeed_);

	Missile::ApplyGlobalVariables();
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
			worldTransform_[0].translation_.y = RandomTY(-1.3f, 0.35f);
		}
	}

	if (isAlive_[1] == 1)
	{
		worldTransform_[1].translation_.x += missileMoveSpeed_;
		worldTransform_[1].UpdateMatrix();
		if (worldTransform_[1].translation_.x >= 9.0f)
		{
			worldTransform_[1].translation_.x = -8.0f;
			worldTransform_[1].translation_.y = RandomTY(0.35f, 2.0f);
		}
	}

	Missile::ApplyGlobalVariables();

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

void Missile::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Missile";
	missileMoveSpeed_ = globalVariables->GetFloatValue(groupName, "missileMoveSpeed");
}

float Missile::RandomTY(float min_value, float max_value)
{
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_real_distribution<float> dis(min_value, max_value); 

	return dis(gen); // ランダムな浮動小数点数を生成して返す
}

