#include "BackGround.h"

void BackGround::Initialize()
{
	skyDomeModel_.reset(Model::CreateFromOBJ("Resources/Skydome", "skydome.obj"));
	skyDomeWorldTransform_.translation_ = { 0.0f,0.0f ,1000.0f };
	skyDomeWorldTransform_.scale_ = { 1000.0f,1000.0f ,1000.0f };
	skyDomeWorldTransform_.rotation_ = { 0.0f,0.0f ,0.0f };
	skyDomeWorldTransform_.UpdateMatrix();
}

void BackGround::Update()
{
	skyDomeWorldTransform_.rotation_.y += 0.001f;
	skyDomeWorldTransform_.rotation_.z += 0.001f;
	skyDomeWorldTransform_.UpdateMatrix();
}

void BackGround::Draw(const ViewProjection viewProjection)
{
	skyDomeModel_->Draw(skyDomeWorldTransform_, viewProjection);
}
