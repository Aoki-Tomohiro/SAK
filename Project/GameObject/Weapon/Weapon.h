#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include "Utility/CollisionManager/Collider.h"

class Weapon : public Collider
{
public:

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

	void ApplyGlobalVariables();

	void OnCollision() override;

	Vector3 GetWorldPosition() override;

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> weaponModel_ = nullptr;

	WorldTransform weaponWorldTransform_;

	uint32_t textureHandle_ = 0u;

	//武器の横移動スピード
	float weaponMoveSpeed_ = 0.05f;

	//チャージ時のスピード
	float chargeSpeed_ = 0.05f;

	//攻撃時のスピード
	float attackSpeed_[4] = { 0.03f ,0.085f ,0.15f , 0.2f };

	//クールダウン中のスピード
	float coolDownSpeed_ = 0.05f;

	//チャージ時のカウント
	int chargeCount_ = 0;

	//キーを押しているときのカウント
	int pushCount_ = 0;

	//クールダウン中のタイマー
	int coolDownTimer_ = 60;

	bool IsCharge_ = false;
	bool IsAttack_ = false;
	bool IsCoolDown_ = false;
};

