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

	textureHandle_ = TextureManager::Load("Resources/uvChecker.png");

	model1_ = std::make_unique<Model>();
	model1_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	// 自キャラの生成
	player_ = std::make_unique<Player>();

	// 自キャラの初期化
	player_->Initialize(model1_.get(), textureHandle_);
};

void GameScene::Update(GameManager* gameManager) {
	player_->Update();

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

	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);
	

	Sprite::PostDraw();
};