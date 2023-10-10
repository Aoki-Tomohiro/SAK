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

void ParticleEmitter::Pop(int popCount,float minAngle,float maxAngle)
{
	for (int i = 0; i < popCount; i++) {
		Particle* newParticle = new Particle();

		float scale = RandomF(popScale_.min, popScale_.max);
		float speed = RandomF(popSpeed_.min, popSpeed_.max);

		float angle = RandomF(minAngle, maxAngle);
		float radian = angle * float(M_PI / 180.0f);
		//Z軸を含めた回転ってどうやんの！？
		float zAngle = RandomF(0.0f, 180.0f);
		float zRadian = zAngle * float(M_PI / 180.0f);

		Vector3 speedVec3 = { 
			speed * cos(radian),
			speed * sin(radian),
			speed * cos(zRadian),
		};

		newParticle->PopInitialize(worldTransform_.translation_, speedVec3, scale, popScaleMinus_);

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

