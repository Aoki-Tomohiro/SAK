#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include "Utility/CollisionManager/Collider.h"
#include "../GameObject/Weapon/Weapon.h"

class Player : public Collider
{
public:

	void Initialize(Weapon* weapon);

	void Update();

	void Draw(const ViewProjection viewProjection);

	void ApplyGlobalVariables();

	void OnCollision() override;

	void OnCollision(float damage) override;

	Vector3 GetWorldPosition() override;

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> playerModel_ = nullptr;

	WorldTransform playerWorldTransform_;

	uint32_t textureHandle_ = 0u;

	Weapon* weapon_;

	//自機の横移動スピード
	float playerMoveSpeed_ = 0.05f;
};



