#include "Particle.h"
#include "Utility/GlobalVariables.h"
void Particle::Initialize()
{
	model_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/uvChecker.png");

	input_ = Input::GetInstance();

	worldTransform_.translation_.x = 0.0f;
	worldTransform_.translation_.y = 0.0f;
	worldTransform_.translation_.z = 0.0f;


	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Particle";
	//グループを追加
	globalVariables->CreateGroup(groupName);


}

void Particle::Update()
{
	
	worldTransform_.UpdateMatrix();

	Particle::ApplyGlobalVariables();
}

void Particle::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Particle::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Particle";
}