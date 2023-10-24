#pragma once
#include "IScene.h"
#include "Base/TextureManager.h"
#include "Components/Audio.h"
#include "Components/Input.h"
#include "2D/ImGuiManager.h"
#include "3D/DebugCamera/DebugCamera.h"

#include "3D/Model/Model.h"
#include "3D/Model/ParticleModel.h"
#include "2D/Sprite.h"
#include "3D/Matrix/WorldTransform.h"
#include "3D/Matrix/ViewProjection.h"
#include "3D/Particle/ParticleSystem.h"

#include "../GameObject/Player/Player.h"
#include "../GameObject/Weapon/Weapon.h"
#include "../BackGround.h"

#include <memory>

class GameTitleScene : public IScene
{
public:
	//トランジションの時間
	static const int kTransitionTime = 60;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameTitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameTitleScene();

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
  
	//パーティクルモデルの作成
	uint32_t textureHandle_ = 0;
	std::unique_ptr<ParticleModel> particleModel_ = nullptr;
	//パーティクル
	std::unique_ptr<ParticleSystem> particleSystem_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_{};

	//サウンド
	uint32_t soundHandle_ = 0u;

	int soundCount_ = 0;

	//モデル
	std::unique_ptr<Model> playerModel_ = nullptr;
	std::unique_ptr<Model> weaponModel_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform playerWorldTransform_{};
	WorldTransform weaponWorldTransform_{};

	std::unique_ptr<BackGround> backGround_ = nullptr;

	//自機の横移動スピード
	float playerMoveSpeed_ = 0.05f;

	//武器の横移動スピード
	float weaponMoveSpeed_ = 0.05f;

	//タイトル用のスプライト
	std::unique_ptr<Sprite>  titleSprite_ = nullptr;
	//タイトルのテクスチャ
	uint32_t titleTextureHandle_ = 0;

	//トランジション用のスプライト
	std::unique_ptr<Sprite> transitionSprite_ = nullptr;
	//トランジションのテクスチャ
	uint32_t transitionTextureHandle_ = 0;
	//トランジションの色
	Vector4 transitionColor_ = { 0.0f,0.0f,0.0f,1.0f };
	//トランジションのフラグ
	bool isTransition_ = false;
	bool isTransitionEnd_ = false;
	//トランジションのタイマー
	float transitionTimer_ = 0;
};

