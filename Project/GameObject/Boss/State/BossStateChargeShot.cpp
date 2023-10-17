#include "BossStateChargeShot.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateNormal.h"
#include "BossStateStun.h"
#include "Utility/GlobalVariables.h"

int BossStateChargeShot::chargeTime = 400;
int BossStateChargeShot::LaserAttackEndTime = 800;


BossStateChargeShot::~BossStateChargeShot() {

}

void BossStateChargeShot::Initialize(Boss* pBoss) {

	input_ = input_->GetInstance();

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

	bossWorldTransform_.translation_.y = 3.3f;
	pBoss->SetWorldTransform(bossWorldTransform_);

	//タイマーの初期化
	chargeTimer_ = chargeTime;
	endTimer_ = LaserAttackEndTime;
}

void BossStateChargeShot::Update(Boss* pBoss) {

	//グローバル変数の適応
	BossStateChargeShot::ApplyGlobalVariables();

	if (chargeTimer_ > 0)
	{
		chargeTimer_--;
		chargeWorldTransform_.scale_.x += 0.0008f;
		chargeWorldTransform_.scale_.y += 0.0008f;
		chargeWorldTransform_.scale_.z += 0.0008f;

		if (input_->IsPushKey(DIK_T))
		{
			pBoss->ChangeState(new BossStateStun());
			return;
		}
	}

	if (chargeTimer_ <= 0 && IsAttack_ == false)
	{
		IsMove_ = true;
	}

	if (IsMove_ == true)
	{
		bossWorldTransform_.translation_.x -= 0.05f;
		pBoss->SetWorldTransform(bossWorldTransform_);

		if (bossWorldTransform_.translation_.x <= -6.9f)
		{
			bossWorldTransform_.translation_.x = -6.9f;
			pBoss->SetWorldTransform(bossWorldTransform_);

			ChargeShot* chargeShot;

			chargeShot = new ChargeShot();
			chargeShot->Initialize();
			pBoss->AddChargeShot(chargeShot);

			IsMove_ = false;
			IsAttack_ = true;
		}
	}

	if (IsAttack_ == true) 
	{
		chargeTimer_ = -1;

		//ボスの移動
		bossWorldTransform_.translation_.x += 0.05f;
		pBoss->SetWorldTransform(bossWorldTransform_);
	}


	//ワールドトランスフォームの更新
	chargeWorldTransform_.UpdateMatrix();

	//攻撃終了
	if (bossWorldTransform_.translation_.x >= 5.8f)
	{
		pBoss->ChangeState(new BossStateNormal());
	}

	ImGui::Begin("ChargeShot");
	ImGui::Text("Push T Key : BossStateStun");
	ImGui::Text("bossTransform %f", bossWorldTransform_.translation_.x);
	ImGui::Text("chargeTimer %d", chargeTimer_);
	ImGui::End();
}

void BossStateChargeShot::Draw(Boss* pBoss, const ViewProjection& viewProjection) {

	if (chargeTimer_ > 0)
	{
		//モデルの描画
		chargemodel_->Draw(chargeWorldTransform_, viewProjection);
	}
}

void BossStateChargeShot::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	/*const char* groupName = "BossStateLaserAttack";
	WarningTime = globalVariables->GetIntValue(groupName, "WarningTime");
	LaserAttackEndTime = globalVariables->GetIntValue(groupName, "LaserAttackEndTime");
	laserScale_ = globalVariables->GetVector3Value(groupName, "LaserScale");*/
}