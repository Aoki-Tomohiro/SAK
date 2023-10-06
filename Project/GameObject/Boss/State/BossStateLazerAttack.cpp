#include "BossStateLaserAttack.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateNormal.h"

void BossStateLaserAttack::Initialize(Boss* pBoss) {

}

void BossStateLaserAttack::Update(Boss* pBoss) {
	if (--endTimer_ < 0) {
		pBoss->ChangeState(new BossStateNormal());
	}
}

void BossStateLaserAttack::ApplyGlobalVariables() {

}