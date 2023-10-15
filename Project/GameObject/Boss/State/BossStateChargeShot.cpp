#include "BossStateChargeShot.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateNormal.h"
#include "Utility/GlobalVariables.h"

int BossStateChargeShot::chargeTime = 400;
int BossStateChargeShot::LaserAttackEndTime = 800;


BossStateChargeShot::~BossStateChargeShot() {

}

void BossStateChargeShot::Initialize(Boss* pBoss) {

	//グローバル変数
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	//const char* groupName = "BossStateLaserAttack";
	////グループを追加
	//globalVariables->CreateGroup(groupName);
	//globalVariables->AddItem(groupName, "WarningTime", WarningTime);
	//globalVariables->AddItem(groupName, "LaserAttackEndTime", LaserAttackEndTime);
	//globalVariables->AddItem(groupName, "LaserScale", laserScale_);

	//グローバル変数の適応
	BossStateChargeShot::ApplyGlobalVariables();

	//モデルの作成
	chargemodel_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	//ワールドトランスフォームの初期化
	chargeWorldTransform_.translation_.y = 2.0f;
	chargeWorldTransform_.scale_ = { 0.1f,0.1f,0.1f };

	chargeShot_ = std::make_unique<ChargeShot>();
	chargeShot_->Initialize();

	//タイマーの初期化
	chargeTimer_ = chargeTime;
	endTimer_ = LaserAttackEndTime;
}

void BossStateChargeShot::Update(Boss* pBoss) {

	//グローバル変数の適応
	BossStateChargeShot::ApplyGlobalVariables();

	chargeTimer_--;

	if (chargeTimer_ > 0)
	{
		chargeWorldTransform_.scale_.x += 0.0008f;
		chargeWorldTransform_.scale_.y += 0.0008f;
		chargeWorldTransform_.scale_.z += 0.0008f;
	}

	if (chargeTimer_ < 0)
	{
		chargeShot_->Update();
	}

	//ワールドトランスフォームの更新
	chargeWorldTransform_.UpdateMatrix();

	//攻撃終了
	if (--endTimer_ < 0) {
		pBoss->ChangeState(new BossStateNormal());
	}
}

void BossStateChargeShot::Draw(Boss* pBoss, const ViewProjection& viewProjection) {

	if (chargeTimer_ > 0)
	{
		//モデルの描画
		chargemodel_->Draw(chargeWorldTransform_, viewProjection);
	}

	if (chargeTimer_ < 0 && endTimer_ > 0)
	{
		chargeShot_->Draw(viewProjection);
	}
}

void BossStateChargeShot::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	/*const char* groupName = "BossStateLaserAttack";
	WarningTime = globalVariables->GetIntValue(groupName, "WarningTime");
	LaserAttackEndTime = globalVariables->GetIntValue(groupName, "LaserAttackEndTime");
	laserScale_ = globalVariables->GetVector3Value(groupName, "LaserScale");*/
}