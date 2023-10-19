#include "Particle.h"
#include "2D/ImGuiManager.h"

void Particle::Initialize(const ParticleData& particleData) {
	particleData_ = particleData;
	particleData_.scale = { particleData_.scale.x,particleData_.scale.x ,particleData_.scale.x };
	scaleMinus_ = 0.01f;
}

void Particle::Update() {

	float scale = particleData_.scale.x;
	scale -= scaleMinus_;
	if (scale < 0.0f) {
		scale = 0.0f;
		isDead_ = true;
	}
	particleData_.translation = Add(particleData_.translation, particleData_.velocity);
	particleData_.scale = { scale,scale,scale };
}