#include "GameOverScene.h"
#include "GameManager.h"
#include "GameTitleScene.h"
#include <cassert>

GameOverScene::GameOverScene() {};

GameOverScene::~GameOverScene() {};

void GameOverScene::Initialize(GameManager* gameManager)
{
	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();
  
	playerModel_.reset(Model::CreateFromOBJ("Resources/Platform", "Platform.obj"));
	weaponModel_.reset(Model::CreateFromOBJ("Resources/Head", "Head.obj"));
	bossModel_.reset(Model::CreateFromOBJ("Resources/Boss", "Boss.obj"));

	playerWorldTransform_.translation_ = { -4.5f,-2.0f,9.0f };
	playerWorldTransform_.rotation_.z = 1.6f;
	playerWorldTransform_.scale_ = { 1.5f,1.5f,1.5f };

	weaponWorldTransform_.translation_ = { -5.5f,-2.0f,9.0f };
	weaponWorldTransform_.rotation_.z = 1.6f;
	weaponWorldTransform_.scale_ = { 1.5f,1.5f,1.5f };

	bossWorldTransform_.translation_ = { 4.5f,-2.2f,0.0f };
	bossWorldTransform_.rotation_.z = 0.6f;
	bossWorldTransform_.scale_ = { 1.5f,1.5f,1.5f };

	//背景の生成
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	viewProjection_.UpdateMatrix();
  
	//スプライトの生成
	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });
};

void GameOverScene::Update(GameManager* gameManager)
{
	//背景の更新
	backGround_->Update();

	if (input_->IsPushKeyEnter(DIK_SPACE))
	{
		if (isTransitionEnd_) {
			isTransition_ = true;
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
			gameManager->ChangeScene(new GameTitleScene());
		}
	}

	ImGui::Begin("Game Over");
	ImGui::Text("push SPACE : Title");
	ImGui::End();

	playerWorldTransform_.UpdateMatrix();
	weaponWorldTransform_.UpdateMatrix();
	bossWorldTransform_.UpdateMatrix();

	viewProjection_.UpdateMatrix();
};

void GameOverScene::Draw(GameManager* gameManager)
{
	//モデルの描画
	Model::PreDraw();

	//背景の描画
	backGround_->Draw(viewProjection_);

	playerModel_->Draw(playerWorldTransform_, viewProjection_);

	weaponModel_->Draw(weaponWorldTransform_, viewProjection_);

	bossModel_->Draw(bossWorldTransform_, viewProjection_);

	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);

	transitionSprite_->Draw();

	Sprite::PostDraw();
};