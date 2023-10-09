#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"

class Player
{
public:

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

	void ApplyGlobalVariables();

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> playerModel_ = nullptr;
	std::unique_ptr<Model> weaponModel_ = nullptr;

	WorldTransform playerWorldTransform_;
	WorldTransform weaponWorldTransform_;

	uint32_t textureHandle_ = 0u;

	float playerMoveSpeed_ = 0.05f;

	float attackSpeed_[3] = { 0.05f ,0.085f ,0.12f };
	float coolTimeSpeed_ = 0.05f;

	int attackCount_ = 0;
	int attackTimer = 60;

	bool IsUP_ = false;
	bool IsDown_ = false;
};



