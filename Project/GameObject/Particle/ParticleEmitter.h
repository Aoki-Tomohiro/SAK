#pragma once
#include "PopParticle.h"
#include <list>
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"

class ParticleEmitter
{
public:
	~ParticleEmitter();

	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

	///<summary>
	///1回だけパーティクルを指定角度に呼び出す
	/// </summary>
	/// <param name ="popCount">一度に出す個数</param>
	/// <param name ="minAngle">最小角度</param>
	/// <param name ="maxAngle">最大角度</param>
	void Pop(int popCount, float minAngle, float maxAngle);

	void ApplyGlobalVariables();

	bool GetIsDead() { return isDead; };

	float RandomF(float min, float max);

private:
	Input* input_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	std::list<PopParticle*> particles_;

	const int deleteFrame = 180;

	int nowFrame = 0;

	bool isDead = false;

	struct minmaxStruct {
		float min;
		float max;
	};

	float popScaleMinus_ = 0.01f;

	//それぞれの最小最大ランダム
	minmaxStruct popScale_ = { 0.1f,0.15f };
	minmaxStruct popSpeed_ = { 0.02f,0.04f };
};

