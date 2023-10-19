#pragma once
#include "Utility/MathFunction.h"

class Particle {
public:
	void Initialize(Vector3 pos, Vector3 rotate, Vector3 scale, Vector3 velocity, Vector4 color, float lifeTime);

	void Update();

	bool IsDead() { return isDead_; };

	Vector3 GetPos() { return pos_; };

	Vector3 GetRotate() { return rotate_; };

	Vector3 GetScale() { return scale_; };

private:
	Vector3 pos_{};

	Vector3 rotate_{};

	Vector3 scale_{};

	Vector3 velocity_{};

	float lifeTime_ = 0;

	bool isDead_ = false;

	float scaleMinus_ = 0.01f;
};