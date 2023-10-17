#include "Boss.h"
#include "State/BossStateNormal.h"
#include "State/BossStateLaserAttack.h"

void Boss::Initialize() {

	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));

	bossModel_.reset(Model::CreateFromOBJ("Resources/Boss", "Boss.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.y = 3.3f;
	worldTransform_.UpdateMatrix();

	//ボスの行動パターンの初期化
	state_ = std::make_unique<BossStateNormal>();
	state_->Initialize(this);

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);

	//Motionの値

	//そもそものおおきさ
	normalScale_ = { 0.4f,0.4f,0.4f };

	bossMotion_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
	};

}

void Boss::Update() {

	//状態の更新
	state_->Update(this);

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

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//モーション更新
	ModelMotion();
}

void Boss::Draw(const ViewProjection& viewProjection) {

	//ボスのモデルの描画
	model_->Draw(worldTransform_, viewProjection);

	bossModel_->Draw(bossMotionWorldTransform_, viewProjection);

	//状態の描画
	state_->Draw(this, viewProjection);

	//レーザーの描画
	for (std::unique_ptr<Laser>& laser : lasers_) {
		laser->Draw(viewProjection);
	}
}

void Boss::ChangeState(IBossState* state) {

	state_.reset(state);
	state_->Initialize(this);
}

void Boss::AddLaser(Laser* laser) {

	//レーザーをリストに追加
	lasers_.push_back(std::unique_ptr<Laser>(laser));
}

void Boss::OnCollision() {
	ImGui::Begin("Collision");
	ImGui::Text("BossHit");
	ImGui::End();
}

Vector3 Boss::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}

void Boss::ModelMotion()
{

	bossMotionWorldTransform_.translation_ = Add(Add(bossMotion_.translation_, worldTransform_.translation_), normalTransration_);
	bossMotionWorldTransform_.scale_ = Multiply(Multiply(bossMotion_.scale_, worldTransform_.scale_), normalScale_);
	bossMotionWorldTransform_.rotation_ = Add(bossMotion_.rotation_, worldTransform_.rotation_);
	bossModel_->GetMaterial()->SetColor(bossMotion_.color_);

	bossMotionWorldTransform_.UpdateMatrix();

}
