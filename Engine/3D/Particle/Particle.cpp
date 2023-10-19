#include "Particle.h"
#include "2D/ImGuiManager.h"

void Particle::Initialize(Vector3 pos, Vector3 rotate, Vector3 scale, Vector3 velocity, Vector4 color, float lifeTime) {
	pos_ = pos;
	rotate_ = rotate;
	scale_ = scale;
	velocity_ = velocity;
	lifeTime_ = lifeTime;
	scaleMinus_ = 0.01f;
}

void Particle::Update() {

	//今回はxyz全部大きさ同じなので、xから大きさを取るよ
	scale_.x -= scaleMinus_;
	scale_.y -= scaleMinus_;
	scale_.z -= scaleMinus_;
	if (scale_.x < 0.0f || scale_.y < 0.0f || scale_.z < 0.0f) {
		scale_.x = 0.0f;
		scale_.y = 0.0f;
		scale_.z = 0.0f;
		isDead_ = true;
	}

	pos_ = Add(pos_, velocity_);
}