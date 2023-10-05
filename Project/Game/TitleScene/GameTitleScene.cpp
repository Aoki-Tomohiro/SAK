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

	ImGui::Begin("Game Title");
	ImGui::Text("push Space : Game Select");
	ImGui::End();
};

void GameTitleScene::Draw(GameManager* gameManager)
{
	//モデルの描画
	Model::PreDraw();


	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);


	Sprite::PostDraw();
};