#include "ModelTester.h"

void ModelTester::Initialize()
{
	model_.reset(Model::CreateFromOBJ("Resources/Boss", "Boss.obj"));
	worldTransform_.translation_ = { 0.0f,0.0f ,0.0f };
	worldTransform_.UpdateMatrix();
}

void ModelTester::Update()
{
	worldTransform_.UpdateMatrix();
}

void ModelTester::Draw(const ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
