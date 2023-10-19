#include "GameTitleScene.h"
#include "GameManager.h"
#include "GameSelectScene.h"
#include <cassert>

GameTitleScene::GameTitleScene() {};

GameTitleScene::~GameTitleScene() {};

void GameTitleScene::Initialize(GameManager* gameManager)
{
	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();
};

void GameTitleScene::Update(GameManager* gameManager) 
{
	if (input_->IsPushKeyEnter(DIK_SPACE))
	{
		gameManager->ChangeScene(new GameSelectScene);
	}

	if (input_->IsPushKeyEnter(DIK_P)) {
		ParticleEmitter::EmitterData emitterData =
		{
			{ 0.0f,0.0f },//座標
			{ 0.0f,0.0f },//角度
			{ 0.1f, 0.15f },//スケール
			{ 0.02f ,0.04f },//速度
			{ 0.0f,360.0f },//方角
			{ 1.0f,1.0f },//色
			{ 60.0f,60.0f }//寿命
		};
		ParticleEmitter* newParticleEmitter = ParticleEmitter::CreateFromOBJ("Resources/particlePop", "particlePop.obj", 10, emitterData);
		particleSystem_.push_back(std::unique_ptr<ParticleEmitter>(newParticleEmitter));
	}

	//パーティクルエミッターデリーと
	particleSystem_.remove_if([](std::unique_ptr<ParticleEmitter>& particleEmitter) {
		if (particleEmitter->IsDead()) {
			particleEmitter.reset();
			return true;
		}
		return false;
		}
	);

	for (std::unique_ptr<ParticleEmitter>& particleEmitter : particleSystem_) {
		particleEmitter->Update();
	}

	viewProjection_.UpdateMatrix();

	ImGui::Begin("Game Title");
	ImGui::Text("push Space : Game Select");
	ImGui::End();
};

void GameTitleScene::Draw(GameManager* gameManager)
{
	//モデルの描画
	Model::PreDraw();


	Model::PostDraw();

	//パーティクルの描画
	ParticleEmitter::PreDraw();

	for (std::unique_ptr<ParticleEmitter>& particleEmitter : particleSystem_) {
		particleEmitter->Draw(viewProjection_);
	}

	ParticleEmitter::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);


	Sprite::PostDraw();
};