#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"

class Particle
{
public:

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

	void ApplyGlobalVariables();

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

};