#include "PopParticle.h"

void PopParticle::Initialize(Vector3 startPos, Vector3 velocity, float scale, float scaleMinus)
{
	input_ = Input::GetInstance();

	model_.reset(Model::CreateFromOBJ("Resources/particlePop", "particlePop.obj"));

	model_->GetMaterial()->SetColor({ 1.0f,1.0f,1.0f,0.5f });

	worldTransform_.translation_ = startPos;
	worldTransform_.scale_ = { scale,scale,scale };
	velocity_ = velocity;
	scaleMinus_ = scaleMinus;

	isDead_ = false;
	worldTransform_.UpdateMatrix();
}

void PopParticle::Update()
{
	float scale = worldTransform_.scale_.x;
	scale -= scaleMinus_;

	if (scale < 0.0f) 
	{
		scale = 0.0f;
		isDead_ = true;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.scale_ = { scale,scale,scale };

	worldTransform_.UpdateMatrix();
}

void PopParticle::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}