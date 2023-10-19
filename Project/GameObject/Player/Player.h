#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include "Utility/CollisionManager/Collider.h"
#include "../GameObject/Weapon/Weapon.h"
#include "../GameObject/Missile/Missile.h"

class Player
{
public:

	void Initialize(Weapon* weapon);

	void Update();

	void Draw(const ViewProjection viewProjection);

	void ApplyGlobalVariables();

	void ModelMotion();

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> playerModelDummy_ = nullptr;

	std::unique_ptr<Model> platformModel_ = nullptr;

	WorldTransform playerWorldTransform_;

	uint32_t textureHandle_ = 0u;

	Weapon* weapon_;

	//自機の横移動スピード
	float playerMoveSpeed_ = 0.05f;

	//モデルとモーション
	enum {
		Stay,
		Move,
	};

	int  motionMode_;
	Vector3 prePlayerTranslation_;

	WorldTransform platformMotionWorldTransform_;

	struct platformMotionStruct {
		Vector3 translation_; /*playerWorldTransform_.translation_を基準としたLocal座標*/
		Vector3 rotation_;/*playerWorldTransform_.rotation_を基準としたLocal回転*/
		Vector3 scale_;/*playerWorldTransform_.scale_を基準としたLocalスケール*/
		Vector4 color_;/*色やんね*/
	};

    platformMotionStruct platformMotion_;

	//横移動
	struct platformMotionMoveStruct {
		float maxFlex_;/*伸び具合*/
		float duration;/*伸びるのにかかる時間*/
		float time;
		float timePlus;
		Vector3 flex_;
		Vector3 flexPos_;
	};

	//そもそものサイズ
	Vector3 NormalScale_;

	platformMotionMoveStruct platformMotionMove_;
};



