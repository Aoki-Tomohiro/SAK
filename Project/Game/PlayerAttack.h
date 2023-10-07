#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"

class PlayerAttack
{
public:

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	float attackSpeed_ = 0.05f;
	float upAttackSpeed_ = 3.0f;

	int count_ = 0;
};

