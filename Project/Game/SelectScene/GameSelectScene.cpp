#include "GameSelectScene.h"
#include "GameManager.h"
#include "GameScene.h"

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
};

void GameSelectScene::Update(GameManager* gameManager)
{
	if (input_->IsPushKeyEnter(DIK_SPACE))
	{
		gameManager->ChangeScene(new GameScene);
	}

	ImGui::Begin("Game Select");
	ImGui::Text("push Space : Game Play");
	ImGui::End();
};

void GameSelectScene::Draw(GameManager* gameManager)
{
	//モデルの描画
	Model::PreDraw();


	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);


	Sprite::PostDraw();
};