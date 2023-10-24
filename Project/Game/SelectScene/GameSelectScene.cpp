#include "GameSelectScene.h"
#include "GameManager.h"
#include "GameScene.h"
#include <algorithm>

GameSelectScene::GameSelectScene() {};

GameSelectScene::~GameSelectScene() {};

void GameSelectScene::Initialize(GameManager* gameManager)
{
	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();

	soundHandle_ = audio_->SoundLoadWave("Resources/Sounds/Selection.wav");

	//スプライトの生成
	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });

	viewProjection_.UpdateMatrix();

	//武器の生成
	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();

	//プレイヤーの生成
	player_ = std::make_unique<Player>();
	player_->Initialize(weapon_.get());

	//天球の作成
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();
};

void GameSelectScene::Update(GameManager* gameManager)
{
	//プレイヤーのアニメーションの更新
	player_->StartAnimation();

	//武器のアニメーションの更新
	weapon_->StartAnimaion();

	//天球の更新
	backGround_->Update();


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
			gameManager->ChangeScene(new GameScene);
		}
	}

	viewProjection_.UpdateMatrix();

	ImGui::Begin("Game Select");
	ImGui::Text("push Space : Game Play");
	ImGui::Text("%f", transitionColor_.w);
	ImGui::End();
};

void GameSelectScene::Draw(GameManager* gameManager)
{
	//モデルの描画
	Model::PreDraw();

	player_->Draw(viewProjection_);

	weapon_->Draw(viewProjection_);

	backGround_->Draw(viewProjection_);

	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);

	transitionSprite_->Draw();

	Sprite::PostDraw();
};