#include "GameTitleScene.h"
#include "GameManager.h"
#include "GameSelectScene.h"
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
	//スプライトの生成
	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });
};

void GameTitleScene::Update(GameManager* gameManager) 
{
	if (!Input::GetInstance()->GetJoystickState(joyState_))
	{
		return;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
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
			gameManager->ChangeScene(new GameSelectScene());
		}
	}

	ImGui::Begin("Game Title");
	ImGui::Text("push Space : Game Select");
	ImGui::Text("%f", transitionColor_.w);
	ImGui::End();
};

void GameTitleScene::Draw(GameManager* gameManager)
{
	//モデルの描画
	Model::PreDraw();


	Model::PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);

	transitionSprite_->Draw();

	Sprite::PostDraw();
};