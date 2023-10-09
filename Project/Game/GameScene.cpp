#include "GameScene.h"
#include "GameManager.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include <cassert>
#include <time.h>

GameScene::GameScene() {};

GameScene::~GameScene() {
	delete debugCamera_;

	for (ParticleEmitter* particleEmitter : particleEmitters_) {
		delete particleEmitter;
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

	//particle_ = std::make_unique<Particle>();

	//particle_->Initialize();

	ParticleEmitter* newParticleEmitter = new ParticleEmitter();
	newParticleEmitter->Pop(10);

	particleEmitters_.push_back(newParticleEmitter);
};

void GameScene::Update(GameManager* gameManager) {

	//particle_->Update();

	for (ParticleEmitter* particleEmitter : particleEmitters_) {
		particleEmitter->Update();
	}

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

	//particle_->Draw(viewProjection_);

	for (ParticleEmitter* particleEmitter : particleEmitters_) {
		particleEmitter->Draw(viewProjection_);
	}

	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);
	

	Sprite::PostDraw();
};