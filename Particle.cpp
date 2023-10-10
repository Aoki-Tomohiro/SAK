#include "Particle.h"

Particle::Particle()
{
}

void Particle::GeneralInitialize()
{
	isDead = false;
	input_ = Input::GetInstance();
	worldTransform_.UpdateMatrix();
}

void Particle::PopInitialize(Vector3 startPos, Vector3 velocity,float scale,float scaleMinus)
{
	model_.reset(Model::CreateFromOBJ("Resources/particlePop", "particlePop.obj"));

	model_->GetMaterial()->SetColor({1.0f,1.0f,1.0f,0.5f});

	worldTransform_.translation_ = startPos;
	worldTransform_.scale_ = { scale,scale,scale };
	velocity_ = velocity;
	scaleMinus_ = scaleMinus;

	GeneralInitialize();
}

void Particle::Update()
{

	//今回はxyz全部大きさ同じなので、xから大きさを取るよ
	float scale = worldTransform_.scale_.x;
	scale -= scaleMinus_;
	if (scale < 0.0f) {
		scale = 0.0f;
		isDead = true;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.scale_ = { scale,scale,scale };

	worldTransform_.UpdateMatrix();

}

void Particle::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
