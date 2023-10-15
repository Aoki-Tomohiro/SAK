#include "Boss.h"
#include "State/BossStateNormal.h"
#include "State/BossStateLaserAttack.h"
#include "2D/ImGuiManager.h"
#include "../GameObject/Weapon/Weapon.h"
#include "Utility/GlobalVariables.h"

//実体定義
int Boss::MissileSpornTime = 90;

void Boss::Initialize() {

	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.y = 3.3f;
	//ボスの行動パターンの初期化
	state_ = std::make_unique<BossStateNormal>();
	state_->Initialize(this);

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Missile";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "missileMoveSpeed", missileMoveSpeed_);
	globalVariables->AddItem(groupName, "missileSpornTime", MissileSpornTime);
}

void Boss::Update() {

	//グローバル変数の適応
	Boss::ApplyGlobalVariables();

	//状態の更新
	state_->Update(this);

	//ミサイルを生成
	if (--missileSpornTimer_ < 0) {
		missileSpornTimer_ = MissileSpornTime;
		missileDirection_ *= -1;
		Missile* missile = new Missile();
		missile->Initialize(Vector3{ 13.0f * missileDirection_,RandomTY(-1.3f, 1.0f) ,0.0f }, Vector3{ missileMoveSpeed_ * (missileDirection_ * -1),0.0f,0.0f });
		Boss::AddMissile(missile);
	}

	//死亡フラグの立ったレーザーをリストから削除
	lasers_.remove_if([](std::unique_ptr<Laser>& laser) {
		if (laser->IsDead()) {
			laser.reset();
			return true;
		}
		return false;
	});

	//レーザーの更新
	for (std::unique_ptr<Laser>& laser : lasers_) {
		laser->Update();
	}

	//死亡フラグの立ったミサイルをリストから削除
	missiles_.remove_if([](std::unique_ptr<Missile>& missile) {
		if (missile->IsAlive() == false) {
			missile.reset();
			return true;
		}
		return false;
	});

	//ミサイルの更新
	for (std::unique_ptr<Missile>& missile : missiles_) {
		missile->Update();
	}

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("Boss");
	ImGui::Text("HP : %f", Hp_);
	ImGui::End();
}

void Boss::Draw(const ViewProjection& viewProjection) {

	//ボスのモデルの描画
	model_->Draw(worldTransform_, viewProjection);

	//状態の描画
	state_->Draw(this, viewProjection);

	//レーザーの描画
	for (std::unique_ptr<Laser>& laser : lasers_) {
		laser->Draw(viewProjection);
	}

	//ミサイルの描画
	for (std::unique_ptr<Missile>& missile : missiles_) {
		missile->Draw(viewProjection);
	}
}

void Boss::ChangeState(IBossState* state) {
	state_.reset(state);
	state_->Initialize(this);
}

void Boss::AddLaser(Laser* laser) {
	lasers_.push_back(std::unique_ptr<Laser>(laser));
}

void Boss::AddMissile(Missile* missile){
	missiles_.push_back(std::unique_ptr<Missile>(missile));
}

float Boss::RandomTY(float min_value, float max_value)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min_value, max_value);

	return dis(gen); // ランダムな浮動小数点数を生成して返す
}

void Boss::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Missile";
	missileMoveSpeed_ = globalVariables->GetFloatValue(groupName, "missileMoveSpeed");
	MissileSpornTime = globalVariables->GetIntValue(groupName, "missileSpornTime");
}

void Boss::OnCollision(uint32_t collisionAttribute, float damage) {
	if (IsHit_ == false) {
		Hp_ -= damage;
	}
}

Vector3 Boss::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}