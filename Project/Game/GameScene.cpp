#include "GameScene.h"
#include "GameManager.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {
	delete debugCamera_;
};

void GameScene::Initialize(GameManager* gameManager) {

	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera();
  
	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize();

	// 自キャラの生成
	weapon_ = std::make_unique<Weapon>();
	// 自キャラの初期化
 	weapon_->Initialize();
	//自キャラを設定
	weapon_->SetPlayer(player_.get());
  
	//ミサイルの生成
	missileManager_ = std::make_unique<MissileManager>();
	// ミサイルの初期化
	missileManager_->Initialize();
  
	//ボスの作成
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();

	//衝突マネージャーの生成
	collisionManager_ = std::make_unique<CollisionManager>();
};

void GameScene::Update(GameManager* gameManager) {

	player_->Update();

	weapon_->Update();

  	//ボスの更新
	boss_->Update();

	missileManager_->Update();

	//衝突判定
	collisionManager_->ClearColliderList();
	collisionManager_->SetColliderList(player_.get());
	collisionManager_->SetColliderList(boss_.get());
	const std::list<std::unique_ptr<Laser>>& lasers = boss_->GetLaser();
	for (const std::unique_ptr<Laser>& laser : lasers) {
		collisionManager_->SetColliderList(laser.get());
	}
	const std::unique_ptr<Missile>& leftMissile = missileManager_->GetLeftMissile();
	const std::unique_ptr<Missile>& rightMissile = missileManager_->GetRightMissile();
	collisionManager_->SetColliderList(leftMissile.get());
	collisionManager_->SetColliderList(rightMissile.get());
	collisionManager_->SetColliderList(weapon_.get());
	collisionManager_->CheckAllCollisions();


	//デバッグカメラの更新
	debugCamera_->Update();
	//デバッグカメラの切り替え
	if (input_->IsPushKeyEnter(DIK_1)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		}
		else {
			isDebugCameraActive_ = false;
		}
	}

	if (input_->IsPushKeyEnter(DIK_1))
	{
		gameManager->ChangeScene(new GameClearScene);
	}

	if (input_->IsPushKeyEnter(DIK_2))
	{
		gameManager->ChangeScene(new GameOverScene);
	}

	ImGui::Begin("Game Play");
	ImGui::Text("push 1 : Game Clear");
	ImGui::Text("push 2 : Game Over");
	ImGui::End();

	viewProjection_.UpdateMatrix();
};

void GameScene::Draw(GameManager* gameManager) {

	//モデルの描画
	Model::PreDraw();

	player_->Draw(viewProjection_);

	weapon_->Draw(viewProjection_);

	//ボスの描画
	boss_->Draw(viewProjection_);
  
	missileManager_->Draw(viewProjection_);

	Model::PostDraw();

	//スプライトの描画
	Sprite::PreDraw(Sprite::kBlendModeNormal);
	

	Sprite::PostDraw();
};