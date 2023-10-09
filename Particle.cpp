#include "Particle.h"

Particle::Particle()
{
}

void Particle::Initialize(Vector3 startPos, Vector3 velocity,float scale,float scaleMinus)
{
	model_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	textureHandle_ = TextureManager::Load("Resources/uvChecker.png");

	input_ = Input::GetInstance();

	isDead = false;

	worldTransform_.translation_ = startPos;
	worldTransform_.scale_ = { scale,scale,scale };
	velocity_ = velocity;
	scaleMinus_ = scaleMinus;

	worldTransform_.UpdateMatrix();
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
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
