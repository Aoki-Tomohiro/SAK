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

	model1_.reset(Model::CreateFromOBJ("Resources/Platform", "Platform.obj"));
	model2_.reset(Model::CreateFromOBJ("Resources/Head", "Head.obj"));
	model3_.reset(Model::CreateFromOBJ("Resources/Boss", "Boss.obj"));

	worldTransform1_.translation_ = { -4.5f,-2.0f,9.0f };
	worldTransform1_.rotation_.z = 1.6f;
	worldTransform1_.scale_ = { 1.5f,1.5f,1.5f };

	worldTransform2_.translation_ = { -5.5f,-2.0f,9.0f };
	worldTransform2_.rotation_.z = 1.6f;
	worldTransform2_.scale_ = { 1.5f,1.5f,1.5f };

	worldTransform3_.translation_ = { 4.5f,-2.2f,0.0f };
	worldTransform3_.rotation_.z = 0.6f;
	worldTransform3_.scale_ = { 1.5f,1.5f,1.5f };

	//背景の生成
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	viewProjection_.UpdateMatrix();
};

void GameOverScene::Update(GameManager* gameManager)
{
	//背景の更新
	backGround_->Update();

	if (input_->IsPushKeyEnter(DIK_SPACE))
	{
		gameManager->ChangeScene(new GameTitleScene);
	}

	ImGui::Begin("Game Over");
	ImGui::Text("push SPACE : Title");
	ImGui::SliderFloat3("translation", &worldTransform1_.translation_.x, -10.0f, 10.0f, "%.3f");
	ImGui::SliderFloat3("rotation", &worldTransform1_.rotation_.x, 0.0f, 10.0f, "%.3f");
	ImGui::SliderFloat3("scale", &worldTransform1_.scale_.x, 0.0f, 10.0f, "%.3f");
	ImGui::End();

	worldTransform1_.UpdateMatrix();
	worldTransform2_.UpdateMatrix();
	worldTransform3_.UpdateMatrix();

	viewProjection_.UpdateMatrix();
};

void GameOverScene::Draw(GameManager* gameManager)
{
	//モデルの描画
	Model::PreDraw();

	//背景の描画
	backGround_->Draw(viewProjection_);

	model1_->Draw(worldTransform1_, viewProjection_);

	model2_->Draw(worldTransform2_, viewProjection_);

	model3_->Draw(worldTransform3_, viewProjection_);

	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);


	Sprite::PostDraw();
};