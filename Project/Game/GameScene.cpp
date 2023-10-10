#include "GameScene.h"
#include "GameManager.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {
	delete debugCamera_;
	for (ModelTester* mdoelTester : modelTester_) {
		delete mdoelTester;
	}
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

	
	ModelTester* newModelTester = new ModelTester();
	newModelTester->Initialize();

	modelTester_.push_back(newModelTester);

};

void GameScene::Update(GameManager* gameManager) {

	for (ModelTester* modelTester : modelTester_) {
		modelTester->Update();
	}

	//デバッグカメラの更新
	debugCamera_->Update();
	//デバッグカメラの切り替え
	if (input_->IsPushKeyEnter(DIK_D)) {
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
};

void GameScene::Draw(GameManager* gameManager) {

	//モデルの描画
	Model::PreDraw();

	for (ModelTester* modelTester : modelTester_) {
		mdoelTester->Draw(viewProjection_);
	}


	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);
	

	Sprite::PostDraw();
};