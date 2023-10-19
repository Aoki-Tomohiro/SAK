#include "GameScene.h"
#include "GameManager.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {
	delete debugCamera_;
	delete sprite_;
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
	weapon_ = std::make_unique<Weapon>();
	// 自キャラの初期化
 	weapon_->Initialize();
  
	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(weapon_.get());
  
	//ボスの作成
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();
	boss_->SetWeapon(weapon_.get());

	//衝突マネージャーの生成
	collisionManager_ = std::make_unique<CollisionManager>();

	//背景の生成
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	tex = TextureManager::Load("Resources/Images/heart.png");

	sprite_->Sprite::Create(tex,{0,0});
};

void GameScene::Update(GameManager* gameManager) {

	player_->Update();

	weapon_->Update();

  	//ボスの更新
	boss_->Update();


	//衝突判定
	collisionManager_->ClearColliderList();
	collisionManager_->SetColliderList(boss_.get());
	const std::list<std::unique_ptr<Laser>>& lasers = boss_->GetLaser();
	for (const std::unique_ptr<Laser>& laser : lasers) {
		collisionManager_->SetColliderList(laser.get());
	}
	const std::list<std::unique_ptr<Missile>>& missiles = boss_->GetMissiles();
	for (const std::unique_ptr<Missile>& missile : missiles) {
		collisionManager_->SetColliderList(missile.get());
	}
	collisionManager_->SetColliderList(weapon_.get());
	const std::list<std::unique_ptr<ChargeShot>>& chargeShots = boss_->GetChargeShot();
	for (const std::unique_ptr<ChargeShot>& chargeShot : chargeShots) {
		collisionManager_->SetColliderList(chargeShot.get());
	}
	collisionManager_->CheckAllCollisions();

	//背景の更新
	backGround_->Update();

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

	//背景の描画
	backGround_->Draw(viewProjection_);

	Model::PostDraw();

	//スプライトの描画
	Sprite::PreDraw(Sprite::kBlendModeNormal);
	
	//player_->DrawSprite();
	sprite_->Draw();

	Sprite::PostDraw();
};