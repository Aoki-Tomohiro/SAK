#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include "Utility/CollisionManager/Collider.h"
#include "../UI.h"

class Weapon : public Collider
{
public:

	~Weapon();

	static int InvincibleTime;

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

	void ApplyGlobalVariables();

	int GetInvolvedMissileCount() { return involvedCount_; };

	void OnCollision(uint32_t collisionAttribute, float damage) override;

	Vector3 GetWorldPosition() override;


	void ModelMotion();
  
	WorldTransform& GetWeaponWorldTransform() { return weaponWorldTransform_; }

	bool GetIsCharge() { return IsCharge_; }

	bool GetIsAttack() { return IsAttack_; }

	bool GetIsHit() { return IsHit_; };

	int GetHP() { return Hp_; };
  
	void DrawSprite();

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> weaponModelDummy_ = nullptr;

	std::unique_ptr<Model> weaponModel_ = nullptr;
	std::unique_ptr<Model> weaponRodModel_ = nullptr;
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
	static const  int MaxHp_ = 3;
	int Hp_;

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

	//モデルとモーション
	enum {
		Stay,
		Charge,
		Attack,
	};

	int  motionMode_;

	WorldTransform weaponMotionWorldTransform_;
	WorldTransform weaponRodMotionWorldTransform_;

	struct weaponMotionStruct {
		Vector3 translation_; /*weaponWorldTransform_.translation_を基準としたLocal座標*/
		Vector3 rotation_;/*weaponWorldTransform_.rotation_を基準としたLocal回転*/
		Vector3 scale_;/*weaponWorldTransform_.scale_を基準としたLocalスケール*/
		Vector4 color_;/*色やんね*/

		Vector3 normalTransration_;//そもそものサイズ
		Vector3 normalScale_;		//そもそもの位置
	};


	float chargeRotateSpeed_;
	float attackRotateSpeed_[4];

	float playerPosY = -3.3f;

	weaponMotionStruct weaponMotion_;
	weaponMotionStruct weaponRodMotion_;
  

	//UI
	UIStruct heartUI_[MaxHp_];
};

