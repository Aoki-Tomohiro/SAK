#include "BossStateChargeShot.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateNormal.h"
#include "BossStateStun.h"
#include "Utility/GlobalVariables.h"

int BossStateChargeShot::chargeTime = 200;
int BossStateChargeShot::chargeShotEndTime = 240;


BossStateChargeShot::~BossStateChargeShot() {

}

void BossStateChargeShot::Initialize(Boss* pBoss) {

	input_ = input_->GetInstance();

	pBoss->SetHitMissileCount(0);

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

	chargeShotSpeed_ = 0.05f;
	respownCount_ = Random(1,2);

	//タイマーの初期化
	chargeTimer_ = chargeTime;
	endTimer_ = chargeShotEndTime;
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

		if (pBoss->GetHitMissileCount() >= 5)
		{
			pBoss->ChangeState(new BossStateStun());
			return;
		}
	}

	if (chargeTimer_ <= 0 && IsAttack_ == false)
	{
		IsMove_ = true;
	}

	if (IsMove_ == true && respownCount_ == 1)
	{
		bossWorldTransform_.translation_.x += chargeShotSpeed_;
		chargeWorldTransform_.translation_.x += chargeShotSpeed_;
		pBoss->SetWorldTransform(bossWorldTransform_);

		if (bossWorldTransform_.translation_.x >= 6.9f)
		{
			bossWorldTransform_.translation_.x = 6.9f;
			pBoss->SetWorldTransform(bossWorldTransform_);

			ChargeShot* chargeShot;

			chargeShot = new ChargeShot();
			chargeShot->Initialize(bossWorldTransform_.translation_,chargeShotSpeed_);
			pBoss->AddChargeShot(chargeShot);

			IsMove_ = false;
			IsAttack_ = true;
		}
	}

	if (IsAttack_ == true && respownCount_ == 1)
	{
		chargeTimer_ = -1;
		chargeShotSpeed_ = -0.05f;
		endTimer_--;

		//ボスの移動
		bossWorldTransform_.translation_.x += chargeShotSpeed_;
		pBoss->SetWorldTransform(bossWorldTransform_);
	}

	if (IsMove_ == true && respownCount_ == 2)
	{
		chargeShotSpeed_ = -0.05f;
		bossWorldTransform_.translation_.x += chargeShotSpeed_;
		chargeWorldTransform_.translation_.x += chargeShotSpeed_;
		pBoss->SetWorldTransform(bossWorldTransform_);

		if (bossWorldTransform_.translation_.x <= -6.9f)
		{
			bossWorldTransform_.translation_.x = -6.9f;
			pBoss->SetWorldTransform(bossWorldTransform_);

			ChargeShot* chargeShot;

			chargeShot = new ChargeShot();
			chargeShot->Initialize(bossWorldTransform_.translation_, chargeShotSpeed_);
			pBoss->AddChargeShot(chargeShot);

			IsMove_ = false;
			IsAttack_ = true;
		}
	}

	if (IsAttack_ == true && respownCount_ == 2)
	{
		chargeTimer_ = -1;
		chargeShotSpeed_ = 0.05f;
		endTimer_--;

		//ボスの移動
		bossWorldTransform_.translation_.x += chargeShotSpeed_;
		pBoss->SetWorldTransform(bossWorldTransform_);
	}


	//ワールドトランスフォームの更新
	chargeWorldTransform_.UpdateMatrix();

	//攻撃終了
	if (endTimer_ <= 0)
	{
		pBoss->ChangeState(new BossStateNormal());
	}

	ImGui::Begin("ChargeShot");
	ImGui::Text("Push T Key : BossStateStun");
	ImGui::Text("bossTransform %f", bossWorldTransform_.translation_.x);
	ImGui::Text("respownCount %d", respownCount_);
	ImGui::Text("chargeTimer %d", chargeTimer_);
	ImGui::Text("endTimer %d", endTimer_);
	ImGui::End();
}

void BossStateChargeShot::Draw(Boss* pBoss, const ViewProjection& viewProjection) {

	if (IsAttack_ == false)
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

int BossStateChargeShot::Random(int min_value, int max_value)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min_value, max_value);

	return dis(gen);
}