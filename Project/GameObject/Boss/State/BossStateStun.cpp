#include "BossStateStun.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateNormal.h"
#include "Utility/GlobalVariables.h"

BossStateStun::~BossStateStun() 
{

}

void BossStateStun::Initialize(Boss* pBoss) 
{
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
	BossStateStun::ApplyGlobalVariables();
}

void BossStateStun::Update(Boss* pBoss) 
{
	if (input_->IsPushKey(DIK_T))
	{
		pBoss->ChangeState(new BossStateNormal());
	}

	ImGui::Begin("Stun");
	ImGui::Text("Push T Key : BossStateNormal");
	ImGui::End();
}

void BossStateStun::Draw(Boss* pBoss, const ViewProjection& viewProjection)
{

}

void BossStateStun::ApplyGlobalVariables() 
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	/*const char* groupName = "BossStateLaserAttack";
	WarningTime = globalVariables->GetIntValue(groupName, "WarningTime");
	LaserAttackEndTime = globalVariables->GetIntValue(groupName, "LaserAttackEndTime");
	laserScale_ = globalVariables->GetVector3Value(groupName, "LaserScale");*/
}
