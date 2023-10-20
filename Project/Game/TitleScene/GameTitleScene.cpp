#include "GameTitleScene.h"
#include "GameManager.h"
#include "GameSelectScene.h"
#include "Components/PostProcess.h"
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
	//パーティクルモデルの作成
	particleModel_.reset(ParticleModel::CreateFromOBJ("Resources/particlePop", "particlePop.obj"));
	//ポストプロセスの有効化
	PostProcess::GetInstance()->SetIsPostProcessActive(true);
	PostProcess::GetInstance()->SetIsBloomActive(true);
};

void GameTitleScene::Update(GameManager* gameManager) 
{
	if (input_->IsPushKeyEnter(DIK_SPACE))
	{
		gameManager->ChangeScene(new GameSelectScene);
	}

	if (input_->IsPushKeyEnter(DIK_P)) {
		ParticleEmitter* newParticleEmitter = EmitterBuilder()
			.SetModel(particleModel_.get())
			.SetParticleType(ParticleEmitter::ParticleType::kScale)
			.SetMaxInstance(100)
			.SetTranslation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
			.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
			.SetScale({ 0.1f, 0.1f,0.1f }, { 0.15f ,0.15f ,0.15f })
			.SetAzimuth(0.0f, 360.0f)
			.SetElevation(90.0f, 90.0f)
			.SetVelocity({ 0.02f ,0.02f ,0.02f }, { 0.04f ,0.04f ,0.04f })
			.SetColor({ 1.0f ,1.0f ,1.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f })
			.SetLifeTime(0.1f, 1.0f)
			.SetCount(100)
			.SetFrequency(4.0f)
			.SetDeleteTime(3.0f)
			.Build();
		particleEmitters_.push_back(std::unique_ptr<ParticleEmitter>(newParticleEmitter));
	}

	//パーティクルエミッターデリーと
	particleEmitters_.remove_if([](std::unique_ptr<ParticleEmitter>& particleEmitter) {
		if (particleEmitter->IsDead()) {
			particleEmitter.reset();
			return true;
		}
		return false;
		}
	);

	for (std::unique_ptr<ParticleEmitter>& particleEmitter : particleEmitters_) {
		particleEmitter->Update();
	}

	viewProjection_.UpdateMatrix();

	ImGui::Begin("Game Title");
	ImGui::Text("push Space : Game Select");
	ImGui::End();
};

void GameTitleScene::Draw(GameManager* gameManager)
{
	PostProcess::GetInstance()->PreDraw();

	//モデルの描画
	Model::PreDraw();


	Model::PostDraw();

	//パーティクルの描画
	ParticleModel::PreDraw();

	//エミッターの描画
	for (std::unique_ptr<ParticleEmitter>& particleEmitter : particleEmitters_) {
		particleEmitter->Draw(viewProjection_);
	}

	ParticleModel::PostDraw();

	PostProcess::GetInstance()->PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);


	Sprite::PostDraw();
};