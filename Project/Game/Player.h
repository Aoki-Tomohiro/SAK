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

	//プレイヤーのスピード
	float playerMoveSpeed_ = 0.05f;

	//チャージ時のスピード
	float chargeSpeed_ = 0.05f;

	//攻撃時のスピード
	float attackSpeed_[4] = { 0.03f ,0.085f ,0.15f , 0.2f };

	//チャージ時のカウント
	int chargeCount_ = 0;

	//キーを押しているときのカウント
	int pushCount_ = 0;

	bool IsCharge_ = false;
	bool IsAttack_ = false;
};



