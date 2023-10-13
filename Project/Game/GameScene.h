#pragma once
#include "IScene.h"
#include "Base/TextureManager.h"
#include "Components/Audio.h"
#include "Components/Input.h"
#include "2D/ImGuiManager.h"
#include "3D/DebugCamera/DebugCamera.h"

#include "3D/Model/Model.h"
#include "2D/Sprite.h"
#include "3D/Matrix/WorldTransform.h"
#include "3D/Matrix/ViewProjection.h"
#include "Utility/CollisionManager/CollisionManager.h"

#include "../GameObject/Boss/Boss.h"
#include "../GameObject/Player/Player.h"
#include "../GameObject/Missile/MissileManager.h"
#include "../GameObject/Weapon/Weapon.h"
#include "../GameObject/Boss/ChargeShot/ChargeShot.h"

#include <memory>

class GameScene : public IScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameManager* gameManager)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameManager* gameManager)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(GameManager* gameManager)override;

private:
	//TextureManager
	TextureManager* textureManager_ = nullptr;
	//Audio
	Audio* audio_ = nullptr;
	//Input
	Input* input_ = nullptr;
	//DebugCamera
	DebugCamera* debugCamera_ = nullptr;
	//DebugCameraの切り替え
	bool isDebugCameraActive_ = false;
	//カメラ
	ViewProjection viewProjection_{};

	//テクスチャ
	uint32_t textureHandle_ = 0;

	//スプライト
	std::unique_ptr<Sprite> sprite1_ = nullptr;
	std::unique_ptr<Sprite> sprite2_ = nullptr;
	Vector4 spriteColor1_{ 1.0f,1.0f,1.0f,1.0f };
	Vector4 spriteColor2_{ 1.0f,1.0f,1.0f,1.0f };

	//モデル
	std::unique_ptr<Model> model1_ = nullptr;
	std::unique_ptr<Model> model2_ = nullptr;
	Vector4 modelColor1_{ 1.0f,1.0f,1.0f,1.0f };
	Vector4 modelColor2_{ 1.0f,1.0f,1.0f,1.0f };

	//ワールドトランスフォーム
	WorldTransform worldTransform1_{};
	WorldTransform worldTransform2_{};

	//ボス
	std::unique_ptr<Boss> boss_ = nullptr;

	std::unique_ptr<Player> player_ = nullptr;

	std::unique_ptr<MissileManager> missileManager_ = nullptr;

	std::unique_ptr<ChargeShot> chargeShot_ = nullptr;

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	std::unique_ptr<Weapon> weapon_ = nullptr;
};