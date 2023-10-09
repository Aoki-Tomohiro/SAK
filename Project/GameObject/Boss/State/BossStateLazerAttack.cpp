#include "BossStateLaserAttack.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateNormal.h"
#include "Utility/GlobalVariables.h"

//実体定義
int BossStateLaserAttack::WarningTime = 160;
int BossStateLaserAttack::LaserAttackEndTime = 240;

BossStateLaserAttack::~BossStateLaserAttack() {

}

void BossStateLaserAttack::Initialize(Boss* pBoss) {

	//グローバル変数
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "BossStateLaserAttack";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "WarningTime", WarningTime);
	globalVariables->AddItem(groupName, "LaserAttackEndTime", LaserAttackEndTime);
	globalVariables->AddItem(groupName, "LaserScale", laserScale_);

	//グローバル変数の適応
	BossStateLaserAttack::ApplyGlobalVariables();

	//モデルの生成
	Vector4 warningColor = { 1.0f,0.0f,0.0f,0.6f };
	bool isLighting = false;
	for (int i = 0; i < warningModels_.size(); i++) {
		warningModels_[i].reset(Model::CreateFromOBJ("Resources/Plane", "Plane.obj"));
		warningModels_[i]->GetMaterial()->SetColor(warningColor);
		warningModels_[i]->GetDirectionalLight()->SetEnableLighting(isLighting);
	}
	
	//ワールドトランスフォームの初期化
	warningWorldTransforms_[0].translation_.x = -4.0f;
	warningWorldTransforms_[0].translation_.z = 10.0f;
	warningWorldTransforms_[0].scale_ = laserScale_;
	warningWorldTransforms_[1].translation_.x = 4.0f;
	warningWorldTransforms_[1].translation_.z = 10.0f;
	warningWorldTransforms_[1].scale_ = laserScale_;

	//タイマーの初期化
	warningTimer_ = WarningTime;
	endTimer_ = LaserAttackEndTime;
}

void BossStateLaserAttack::Update(Boss* pBoss) {

	//グローバル変数の適応
	BossStateLaserAttack::ApplyGlobalVariables();

	//ワールドトランスフォームの更新
	for (int i = 0; i < warningWorldTransforms_.size(); i++) {
		warningWorldTransforms_[i].UpdateMatrix();
	}

	//警告が終わったらレーザーを出す
	if (warningTimer_ >= 0) {
		warningTimer_--;
	}

	if (warningTimer_ == 0) {
		warningTimer_ = -1;
		Laser* lasers[2];
		for (int i = 0; i < warningModels_.size(); i++) {
			lasers[i] = new Laser();
			Vector3 pos = {
				warningWorldTransforms_[i].translation_.x,
				warningWorldTransforms_[i].translation_.y + 10.0f,
				warningWorldTransforms_[i].translation_.z,
			};
			lasers[i]->Initialize(pos, laserScale_);
			pBoss->AddLaser(lasers[i]);
		}
	}

	//攻撃終了
	if (--endTimer_ < 0) {
		pBoss->ChangeState(new BossStateNormal());
	}
}

void BossStateLaserAttack::Draw(Boss* pBoss, const ViewProjection& viewProjection) {

	//警告モデルの描画
	for (int i = 0; i < warningModels_.size(); i++) {
		if (warningTimer_ > 0) {
			warningModels_[i]->Draw(warningWorldTransforms_[i], viewProjection);
		}
	}
}

void BossStateLaserAttack::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "BossStateLaserAttack";
	WarningTime = globalVariables->GetIntValue(groupName, "WarningTime");
	LaserAttackEndTime = globalVariables->GetIntValue(groupName, "LaserAttackEndTime");
	laserScale_ = globalVariables->GetVector3Value(groupName, "LaserScale");
}