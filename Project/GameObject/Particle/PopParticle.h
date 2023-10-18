#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"

class PopParticle
{
public:
	void Initialize(Vector3 startPos, Vector3 velocity, float scale, float scaleMinus);

	void Update();

	void Draw(const ViewProjection viewProjection);

	bool GetIsDead() { return isDead_; }

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	float scaleMinus_ = 0.0f;

	bool isDead_ = false;
};

