#include "ParticleEmitter.h"
#define _USE_MATH_DEFINES
#include <math.h>

ParticleEmitter::ParticleEmitter()
{
}

ParticleEmitter::~ParticleEmitter()
{
	for(Particle* particle : particles_){
		delete particle;
	}
}

void ParticleEmitter::Initialize()
{
}

void ParticleEmitter::Update()
{
	for (Particle * particle : particles_) {
		particle->Update();
	}

	particles_.remove_if([](Particle* particle) {
		if (particle->GetIsDead()) {

			delete particle;
			return true;
		}
		return false;
		}
	);

	nowFrame++;

	if (nowFrame > deleteFrame) {
		isDead = true;
	}

}

void ParticleEmitter::Draw(const ViewProjection viewProjection)
{
	for (Particle* particle : particles_) {
		particle->Draw(viewProjection);
	}
}

void ParticleEmitter::Pop(int popCount)
{
	for (int i = 0; i < popCount; i++) {
		Particle* newParticle = new Particle();

		float scale = RandomF(scale_.min, scale_.max);
		float speed = RandomF(speed_.min, speed_.max);

		float angle = RandomF(0.0f, 360.0f);
		float radian = angle * float(M_PI / 180.0f);
		//Z軸を含めた回転ってどうやんの！？
		float zAngle = RandomF(0.0f, 180.0f);
		float zRadian = zAngle * float(M_PI / 180.0f);

		Vector3 speedVec3 = { 
			speed * cos(radian),
			speed * sin(radian),
			speed * cos(zRadian),
		};

		newParticle->Initialize(worldTransform_.translation_, speedVec3, scale, scaleMinus_);

		particles_.push_back(newParticle);
	}
}

void ParticleEmitter::ApplyGlobalVariables()
{
}

float ParticleEmitter::RandomF(float min, float max)
{
	float diff = max - min;

	int randNumInt = int(min * 100.0f) + rand() % int(diff * 100.0f);
	float randNum = randNumInt / 100.0f;

	return randNum;
}

