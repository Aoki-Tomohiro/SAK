#include "ParticleEmitter.h"

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
	/*Particle* newParticle = new Particle();
	newParticle->Initialize();

	particles_.push_back(newParticle);*/
}

void ParticleEmitter::Update()
{
	for (Particle * particle : particles_) {
		particle->Update();
	}

	ImGui::Begin("ParticleCount");
	ImGui::Text("count %d",count);
	ImGui::End();
}

void ParticleEmitter::Draw(const ViewProjection viewProjection)
{
	for (Particle* particle : particles_) {
		particle->Draw(viewProjection);
	}
}

void ParticleEmitter::Pop(int popCount)
{
	count = 0;
	for (int i = 0; i < popCount; i++) {
		Particle* newParticle = new Particle();
		newParticle->Initialize();

		particles_.push_back(newParticle);
		count++;
	}
}

void ParticleEmitter::ApplyGlobalVariables()
{
}
