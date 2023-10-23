#pragma once
#include "Particle.h"
#include "ChageParticle.h"
#include "Utility/Random.h"
#include <list>
#include <memory>
#include <numbers>

/// <summary>
/// パーティクルエミッター
/// </summary>
class ParticleEmitter {
public:
	enum class ParticleType {
		kNormal,
		kScale,
		kCharge,
	};

	struct minmaxStructVector4 {
		Vector4 min;
		Vector4 max;
	};

	struct minmaxStructVector3 {
		Vector3 min;
		Vector3 max;
	};

	struct minmaxStructFloat {
		float min;
		float max;
	};

	///// <summary>
	///// 初期化
	///// </summary>
	//void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクルのリストを取得
	/// </summary>
	/// <returns></returns>
	std::list<std::unique_ptr<BaseParticle>>& GetParticles() { return particles_; };

	/// <summary>
	/// 死亡フラグを取得
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead_; };

private:
	/// <summary>
	/// パーティクルの生成
	/// </summary>
	void Pop();

private:
	//パーティクルのリスト
	std::list<std::unique_ptr<BaseParticle>> particles_{};
	//パーティクルのタイプ
	ParticleType particleType_ = ParticleType::kNormal;
	//パーティクルの発生位置
	Vector3 popTranslation_ = { 0.0f,0.0f,0.0f };
	//パーティクルの発生範囲
	minmaxStructVector3 popArea_ = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//パーティクルの角度
	minmaxStructVector3 popRotation_ = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//パーティクルのスケール
	minmaxStructVector3 popScale_ = { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} };
	//パーティクルの方位角
	minmaxStructFloat popAzimuth = { 0.0f,360.0f };
	//パーティクルの仰角
	minmaxStructFloat popElevation = { 0.0f,180.0f };
	//パーティクルの初速度
	minmaxStructVector3 popVelocity_ = { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} };
	//パーティクルの色
	minmaxStructVector4 popColor_ = { {1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f} };
	//パーティクルの寿命
	minmaxStructFloat popLifeTime_ = { 0.5f,1.0f };
	//発生数
	uint32_t popCount_ = 1;
	//発生頻度
	float popFrequency_ = 0.1f;
	//頻度用時刻
	float frequencyTime_ = popFrequency_;
	//エミッターの寿命
	float deleteTime_ = 10.0f;
	//寿命用のタイマー
	float deleteTimer_ = 0.0f;
	//死亡フラグ
	bool isDead_ = false;
	//Builderをフレンドクラスに設定
	friend class EmitterBuilder;

};