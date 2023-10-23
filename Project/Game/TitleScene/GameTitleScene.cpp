#include "GameTitleScene.h"
#include "GameManager.h"
#include "GameSelectScene.h"
#include "Components/PostProcess.h"
#include <cassert>
#include <algorithm>

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

	soundHandle_ = audio_->SoundLoadWave("Resources/Sounds/Selection.wav");

	playerModel_.reset(Model::CreateFromOBJ("Resources/Platform", "Platform.obj"));
	weaponModel_.reset(Model::CreateFromOBJ("Resources/Head", "Head.obj"));

	playerWorldTransform_.translation_.x = 0.0f;
	playerWorldTransform_.translation_.y = -3.3f;
	playerWorldTransform_.scale_ = { 0.8f,0.8f,0.8f };

	weaponWorldTransform_.translation_.x = 0.0f;
	weaponWorldTransform_.translation_.y = -2.5f;
	weaponWorldTransform_.scale_ = { 0.8f,0.8f,0.8f };

	//背景の生成
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	viewProjection_.UpdateMatrix();

	//スプライトの生成
	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });
};

void GameTitleScene::Update(GameManager* gameManager) 
{
	//背景の更新
	backGround_->Update();

	playerWorldTransform_.translation_.x -= playerMoveSpeed_;
	weaponWorldTransform_.translation_.x -= weaponMoveSpeed_;

	if (playerWorldTransform_.translation_.x <= -7.3f)
	{
		playerMoveSpeed_ = -0.05f;
		weaponMoveSpeed_ = -0.05f;
	}

	if (playerWorldTransform_.translation_.x >= 7.3f)
	{
		playerMoveSpeed_ = 0.05f;
		weaponMoveSpeed_ = 0.05f;
	}


	if (input_->IsPushKeyEnter(DIK_SPACE))
	{
		if (isTransitionEnd_) {
			isTransition_ = true;
			if (soundCount_ == 0)
			{
				soundCount_ = 1;
				audio_->SoundPlayWave(soundHandle_, false);
			}
		}
	}

	if (isTransitionEnd_ == false) {
		transitionTimer_ += 1.0f / kTransitionTime;
		transitionColor_.w = Lerp(transitionColor_.w, 0.0f, transitionTimer_);
		transitionSprite_->SetColor(transitionColor_);

		if (transitionColor_.w <= 0.0f) {
			isTransitionEnd_ = true;
			transitionTimer_ = 0.0f;
		}
	}

	if (isTransition_) {
		transitionTimer_ += 1.0f / kTransitionTime;
		transitionColor_.w = Lerp(transitionColor_.w, 1.0f, transitionTimer_);
		transitionSprite_->SetColor(transitionColor_);

		if (transitionColor_.w >= 1.0f) {
			gameManager->ChangeScene(new GameSelectScene());
		}
	}

	if (input_->IsPushKeyEnter(DIK_P)) {
    //設定したい項目だけ.Setを呼び出す
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
	playerWorldTransform_.UpdateMatrix();
	weaponWorldTransform_.UpdateMatrix();

	viewProjection_.UpdateMatrix();

	ImGui::Begin("Game Title");
	ImGui::Text("push Space : Game Select");
	ImGui::Text("%f", transitionColor_.w);
	ImGui::End();
};

void GameTitleScene::Draw(GameManager* gameManager)
{
	PostProcess::GetInstance()->PreDraw();

	//モデルの描画
	Model::PreDraw();

	//背景の描画
	backGround_->Draw(viewProjection_);

	playerModel_->Draw(playerWorldTransform_, viewProjection_);

	weaponModel_->Draw(weaponWorldTransform_, viewProjection_);

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

	transitionSprite_->Draw();

	Sprite::PostDraw();
};