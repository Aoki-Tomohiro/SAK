#pragma once
#include "Utility/MathFunction.h"

class Particle {
public:
	//パーティクル構造体
	struct ParticleData {
		Vector3 translation;
		Vector3 rotate;
		Vector3 scale;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
	};

	void Initialize(const ParticleData& particleData);

	void Update();

	const ParticleData& GetParticleData() { return particleData_; };

	bool IsDead() { return isDead_; };

private:
	ParticleData particleData_{};

	bool isDead_ = false;

	float scaleMinus_ = 0.01f;
};