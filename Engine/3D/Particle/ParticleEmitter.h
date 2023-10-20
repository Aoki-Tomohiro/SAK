#pragma once
#include "3D/Model/ParticleModel.h"
#include "Particle.h"
#include <memory>
#include <numbers>
#include <random>

/// <summary>
/// パーティクルエミッター
/// </summary>
class ParticleEmitter {
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	enum class ParticleType {
		kNormal,
		kScale,
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

	/// <summary>
	/// GPUに送るパーティクルの構造体
	/// </summary>
	struct ParticleForGPU {
		Matrix4x4 world;
		Vector4 color;
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// インスタンス数の更新
	/// </summary>
	void UpdateNumInstance();

	/// <summary>
	/// GPUハンドルを取得
	/// </summary>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetDescriptorHandleGPU() { return instancingSrvHandleGPU_; };

	/// <summary>
	/// インスタンス数を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetNumInstance() { return numInstance_; };

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

	/// <summary>
	/// ランダム生成
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	float Random(float min, float max);

	/// <summary>
	/// ワールドトランスフォームのCBVを作成
	/// </summary>
	void CreateWorldTransform();

private:
	//Instancing用のWorldTransform
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	//Instancing用のSRVハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};
	//ランダムエンジン
	std::mt19937 randomEngine_;
	//パーティクルのリスト
	std::list<std::unique_ptr<BaseParticle>> particles_{};
	//パーティクルモデル
	ParticleModel* particleModel_ = nullptr;
	//パーティクルのタイプ
	ParticleType particleType_ = ParticleType::kNormal;
	//パーティクルの発生範囲
	minmaxStructVector3 popTranslation_ = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
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
	//インスタンス数の上限
	uint32_t maxInstance_ = 100;
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
	//描画するインスタンス数
	uint32_t numInstance_ = 0;
	//Builderをフレンドクラスに設定
	friend class EmitterBuilder;

};