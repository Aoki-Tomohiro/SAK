#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include "Utility/CollisionManager/Collider.h"

class Weapon : public Collider
{
public:

	static int InvincibleTime;

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

	void ApplyGlobalVariables();

	void OnCollision(uint32_t collisionAttribute, float damage) override;

	Vector3 GetWorldPosition() override;

	WorldTransform& GetWeaponWorldTransform() { return weaponWorldTransform_; }

	bool GetIsCharge() { return IsCharge_; }

	bool GetIsAttack() { return IsAttack_; }

	bool GetIsHit() { return IsHit_; };

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> weaponModel_ = nullptr;
	std::unique_ptr<Model> involvedMissile_ = nullptr;

	WorldTransform weaponWorldTransform_;
	WorldTransform involvedMissileWorldTransform_;

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
	bool IsHit_ = false;

	//体力
	float Hp_ = 3.0f;

	//無敵時間
	bool invincibleFlag_ = false;
	int invincibleTimer_ = 0;

	//攻撃時のダメージ
	float attackDamage_[4] = { 1.0f,2.0f,3.0f,4.0f };

	//ミサイルを巻き込んでいるか
	bool isInvolvedMissile_ = false;
	int involvedCount_ = 0;
	Vector4 missileColor_[5]{
		{1.0f,1.0f,1.0f,1.0f},
		{1.0f,0.0f,0.0f,1.0f},
		{0.0f,1.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f,0.0f},
	};
};

