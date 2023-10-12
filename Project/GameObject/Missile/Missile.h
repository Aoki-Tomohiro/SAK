#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include <random>
#include "Utility/CollisionManager/Collider.h"
#include "Utility/CollisionManager/CollisionConfig.h"
#include "../GameObject/Weapon/Weapon.h"

class Missile : public Collider
{
public:

	void Initialize(const Vector3& position, const float& speed);

	void Update();

	void Draw(const ViewProjection viewProjection);

	bool IsAlive() { return isAlive_; };

	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; };

	void OnCollision() override;

	Vector3 GetWorldPosition() override;

	void SetWeapon(Weapon* weapon) { weapon_ = weapon; }

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	Weapon* weapon_;

	Matrix4x4 localMatrix_;

	float missileMoveSpeed_ = 0.05f;

	float missileFollowingSpeed_ = 0.2f;

	bool isAlive_ = true;

	bool IsFollowingWeapon_ = false;
};

