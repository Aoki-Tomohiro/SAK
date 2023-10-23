#pragma once
#include "BaseParticle.h"
#include "Utility/MathFunction.h"

class Particle : public BaseParticle {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="translation"></param>
	/// <param name="rotation"></param>
	/// <param name="scale"></param>
	/// <param name="velocity"></param>
	/// <param name="color"></param>
	/// <param name="lifeTime"></param>
	void Initialize(const Vector3& translation, const Vector3& rotation, const Vector3& scale, const Vector3& velocity, const Vector4& color, float lifeTime) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
	float scaleMinus_ = 0.01f;

};