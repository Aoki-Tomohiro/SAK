#include "GameClearScene.h"
#include "GameManager.h"
#include "GameTitleScene.h"
#include <cassert>

GameClearScene::GameClearScene() {};

GameClearScene::~GameClearScene() {};

void GameClearScene::Initialize(GameManager* gameManager)
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

	playerWorldTransform_.translation_ = { 0.0f,-1.7f,2.0f };
	playerWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };

	weaponWorldTransform_.translation_ = { 0.0f,-0.8f,2.0f };
	weaponWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };

	bossWorldTransform_.translation_ = { 0.0f,-3.5f,2.0f };
	bossWorldTransform_.rotation_.x = 1.35f;
	bossWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };

	//背景の生成
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	viewProjection_.UpdateMatrix();


	//スプライトの生成
	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });
};

void GameClearScene::Update(GameManager* gameManager)
{
	//背景の更新
	backGround_->Update();

	float minusSpeed = 4.0f;

	if (tMax <= t) {

		t = 0;
		if(isPush == true) {
			isPush = false;
		}
	else {
			isPush = true;
		}
	}

	t++;

	if (isPush == true) {
		if (plus < plusMax) {
			plus += minusSpeed;
		}

		if (plus >= plusMax) {
			plus = plusMax;
		}
	}else if (isPush == false) {
		if (plus > 0.0f) {
			plus -= minusSpeed;
		}

		if (plus <= 0.0f) {
			plus = 0.0f;
		}
	}

	weaponWorldTransform_.translation_ = { 0.0f,-0.8f + plus,2.0f };
	float ratio = plus / plusMax;

	weaponWorldTransform_.scale_.x = 1.0f + 0.5f * (1.0f  - ratio);
	weaponWorldTransform_.scale_.y = 1.0f + 0.5f * ratio;

	playerWorldTransform_.scale_.x = 1.0f + 0.5f * (1.0f - ratio);
	playerWorldTransform_.scale_.y = 1.0f + 0.5f * ratio;

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

	ImGui::Begin("Game Clear");
	ImGui::Text("push SPACE : Title");
	ImGui::End();

	playerWorldTransform_.UpdateMatrix();
	weaponWorldTransform_.UpdateMatrix();
	bossWorldTransform_.UpdateMatrix();

	viewProjection_.UpdateMatrix();
};

void GameClearScene::Draw(GameManager* gameManager)
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