#pragma once
#include "Particle.h"
#include <list>
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"


class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

	void Pop(int popCount);

	void ApplyGlobalVariables();

	bool GetIsDead() { return isDead; };
	
	float RandomF(float min, float max);

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	std::list<Particle*> particles_;

	const int deleteFrame = 180;

	int nowFrame = 0;

	bool isDead = false;

	struct minmaxStruct {
		float min;
		float max;
	};

	float scaleMinus_ = 0.01f;

	//それぞれの最小最大ランダム
	minmaxStruct scale_ = { 0.1f,0.15f };
	minmaxStruct speed_ = { 0.02f,0.04f };

};