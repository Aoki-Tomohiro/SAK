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
#include "3D/Particle/EmitterBuilder.h"

#include <memory>

class GameTitleScene : public IScene
{
public:
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
	std::unique_ptr<ParticleModel> particleModel_ = nullptr;
	//パーティクル
	std::list<std::unique_ptr<ParticleEmitter>> particleEmitters_{};
	//ビュープロジェクション
	ViewProjection viewProjection_{};
};

