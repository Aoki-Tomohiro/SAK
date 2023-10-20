#include "ParticleEmitter.h"

void ParticleEmitter::Initialize() {
	//ランダムエンジンの初期化
	std::random_device seedGenerator;
	randomEngine_ = std::mt19937(seedGenerator());
	//インスタンシング用のリソースの作成
	instancingResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * maxInstance_);
	//SRVの作成
	instancingSrvHandleGPU_ = TextureManager::GetInstance()->CreateInstancingShaderResourceView(instancingResource_, maxInstance_, sizeof(ParticleForGPU));
	//モデルの作成
	if (particleModel_ == nullptr) {
		particleModel_ = ParticleModel::CreateFromOBJ("Resources/Particle", "Particle.obj");
	}
}


void ParticleEmitter::Update() {
	//パーティクルを生成
	const float kDeltaTime = 1.0f / 60.0f;
	frequencyTime_ += kDeltaTime;
	if (popFrequency_ <= frequencyTime_) {
		for (uint32_t index = 0; index < popCount_; ++index) {
			ParticleEmitter::Pop();
		}
		frequencyTime_ -= popFrequency_;
	}

	//パーティクルの更新
	for (std::list<std::unique_ptr<BaseParticle>>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end(); ++particleIterator) {
		particleIterator->get()->Update();
	}

	//エミッターの死亡フラグを立てる
	deleteTimer_ += kDeltaTime;
	if (deleteTimer_ > deleteTime_) {
		isDead_ = true;
	}

	ParticleEmitter::UpdateNumInstance();
}


void ParticleEmitter::Draw(const ViewProjection& viewProjection) {
	//エミッターの描画
	particleModel_->Draw(instancingSrvHandleGPU_, numInstance_, viewProjection);
}


void ParticleEmitter::UpdateNumInstance() {
	ParticleForGPU* instancingData = nullptr;
	numInstance_ = 0;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	for (std::list<std::unique_ptr<BaseParticle>>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		if (particleIterator->get()->IsDead()) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
		Matrix4x4 worldMatrix = MakeAffineMatrix(particleIterator->get()->GetScale(), particleIterator->get()->GetRotation(), particleIterator->get()->GetTranslation());
		if (numInstance_ < maxInstance_) {
			instancingData[numInstance_].world = worldMatrix;
			instancingData[numInstance_].color = particleIterator->get()->GetColor();
			numInstance_++;
		}
		++particleIterator;
	}
	instancingResource_->Unmap(0, nullptr);
}


void ParticleEmitter::Pop() {
	//座標
	Vector3 translation = {
		Random(popTranslation_.min.x,popTranslation_.max.x),
		Random(popTranslation_.min.y,popTranslation_.max.y),
		Random(popTranslation_.min.z,popTranslation_.max.z)
	};

	//回転
	Vector3 rotation = {
		Random(popRotation_.min.x,popRotation_.max.x),
		Random(popRotation_.min.y,popRotation_.max.y),
		Random(popRotation_.min.z,popRotation_.max.z)
	};

	//スケール
	Vector3 scale = {
		Random(popScale_.min.x,popScale_.max.x),
		Random(popScale_.min.y,popScale_.max.y),
		Random(popScale_.min.z,popScale_.max.z)
	};

	//方位角
	float azimuth = { Random(popAzimuth.min,popAzimuth.max) };
	float azimuthRadian = azimuth * float(std::numbers::pi / 180.0f);

	//仰角
	float elevation = { Random(popElevation.min,popElevation.max) };
	float elevationRadian = elevation * float(std::numbers::pi / 180.0f);

	//速度
	Vector3 velocity = {
		Random(popVelocity_.min.x,popVelocity_.max.x) * std::sin(elevationRadian) * std::cos(azimuthRadian),
		Random(popVelocity_.min.y,popVelocity_.max.y) * std::sin(elevationRadian) * std::sin(azimuthRadian),
		Random(popVelocity_.min.z,popVelocity_.max.z) * std::cos(elevationRadian)
	};

	//色
	Vector4 color = {
		Random(popColor_.min.x,popColor_.max.x),
		Random(popColor_.min.y,popColor_.max.y),
		Random(popColor_.min.z,popColor_.max.z),
		Random(popColor_.min.w,popColor_.max.w)
	};

	//寿命
	float lifeTime = Random(popLifeTime_.min, popLifeTime_.max);

	//パーティクルの生成
	BaseParticle* particle = nullptr;
	switch (particleType_) {
	case ParticleType::kNormal:
		particle = new BaseParticle();
		particle->Initialize(translation, rotation, scale, velocity, color, lifeTime);
		break;
	case ParticleType::kScale:
		particle = new Particle();
		particle->Initialize(translation, rotation, scale, velocity, color, lifeTime);
		break;
	}
	//パーティクルをリストに追加
	particles_.push_back(std::unique_ptr<BaseParticle>(particle));
}


float ParticleEmitter::Random(float min, float max) {
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(randomEngine_);
}


void ParticleEmitter::CreateWorldTransform() {
	//Instancing用のWorldTransformリソースを作る
	instancingResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * maxInstance_);
	//書き込むためのアドレスを取得
	ParticleForGPU* instancingData = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	//単位行列を書き込んでおく
	for (uint32_t index = 0; index < maxInstance_; ++index) {
		instancingData[index].world = MakeIdentity4x4();
		instancingData[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	instancingResource_->Unmap(0, nullptr);
}