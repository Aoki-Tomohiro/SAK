#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"

class Missile
{
public:

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

	/*void ApplyGlobalVariables();*/

	float RandomTY();

	float RandomTY2();

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> model_[2];

	WorldTransform worldTransform_[2];

	uint32_t textureHandle_ = 0u;

	float missileMoveSpeed_ = 0.05f;
};

