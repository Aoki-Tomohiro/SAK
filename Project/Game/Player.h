#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"

class Player
{
public:

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(const ViewProjection viewProjection);

private:
	Input* input_ = nullptr;

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;
};



