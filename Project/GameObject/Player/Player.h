#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include "Utility/CollisionManager/Collider.h"

class Player : public Collider
{
public:

	static int InvincibleTime;

	void Initialize();

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

	//自機の横移動スピード
	float playerMoveSpeed_ = 0.05f;

	//体力
	float Hp_ = 3.0f;
	//無敵時間
	bool invincibleFlag_ = false;
	int invincibleTimer_ = 0;
};



