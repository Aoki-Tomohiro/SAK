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

	time_t currentTime = time(nullptr);
	srand((unsigned int)currentTime);
	
};

void GameScene::Update(GameManager* gameManager) {

	//P押すとパーティクル
	if (input_->IsPushKeyEnter(DIK_P))
	{
		ParticleEmitter* newParticleEmitter = new ParticleEmitter();
		newParticleEmitter->Pop(10,0.0f,360.0f);

		particleEmitters_.push_back(newParticleEmitter);
	}
	//パーティクルエミッターUpdate
	for (ParticleEmitter* particleEmitter : particleEmitters_) {
		particleEmitter->Update();
	}
	//パーティクルエミッターデリーと
	particleEmitters_.remove_if([](ParticleEmitter* particleEmitter) {
		if (particleEmitter->GetIsDead()) {

			delete particleEmitter;
			return true;
		}
		return false;
		}
	);

	//デバッグカメラの更新
	debugCamera_->Update();
	//デバッグカメラの切り替え
	if (input_->IsPushKeyEnter(DIK_3)) {
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

	//パーティクルDraw
	for (ParticleEmitter* particleEmitter : particleEmitters_) {
		particleEmitter->Draw(viewProjection_);
	}

	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);
	

	Sprite::PostDraw();
};