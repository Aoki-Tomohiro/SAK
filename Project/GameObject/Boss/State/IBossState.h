#pragma once

class Boss;

class IBossState {
public:
	virtual void Initialize(Boss* pBoss) = 0;
	virtual void Update(Boss* pBoss) = 0;
	virtual void ApplyGlobalVariables() = 0;
};