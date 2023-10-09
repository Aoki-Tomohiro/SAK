#include "Particle.h"

Particle::Particle()
{
}

void Particle::Initialize()
{
	model_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/uvChecker.png");

	input_ = Input::GetInstance();

	time_t currentTime = time(nullptr);
	srand((unsigned int)currentTime);

	int randNumint = rand() % 10 - 5;
	float randNum = randNumint / 10.0f;
	worldTransform_.translation_.x = randNum;

	randNumint = rand() % 50 - 25;
	randNum = randNumint / 10.0f;
	worldTransform_.translation_.y = randNum;

	randNumint = rand() % 50 - 25;
	randNum = randNumint / 10.0f;
	worldTransform_.translation_.z = randNum;

	worldTransform_.UpdateMatrix();
}

void Particle::Update()
{
	
	worldTransform_.UpdateMatrix();

}

void Particle::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
