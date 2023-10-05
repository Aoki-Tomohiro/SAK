#include "Boss.h"
#include "2D/ImGuiManager.h"
#include "Utility/GlobalVariables.h"

void Boss::Initialize() {
	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.y = 3.0f;
	worldTransform_.translation_.z = 10.0f;

	//グローバル変数
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Boss";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "moveSpeed", moveSpeed_);
}

void Boss::Update() {
	//移動処理
	moveSpeed_ *= moveDirection_;
	worldTransform_.translation_.x += moveSpeed_;

	//画面端まで移動したら移動方向を変える
	if (worldTransform_.translation_.x <= -7.0f || worldTransform_.translation_.x >= 7.0f) {
		moveDirection_ *= -1.0f;
	}

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//グローバル変数の適応
	Boss::ApplyGlobalVariables();
}

void Boss::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void Boss::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Boss";
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "moveSpeed");
}