#include "ViewProjection.h"

void ViewProjection::UpdateViewMatrix() {

	//カメラのワールド行列を作成
	Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotation_, translation_);

	//ビュー行列の計算
	matView_ = Inverse(worldMatrix);
}

void ViewProjection::UpdateProjectionMatrix() {

	//プロジェクション行列の計算
	matProjection_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
}

void ViewProjection::UpdateMatrix() {

	//ビュー行列の計算
	ViewProjection::UpdateViewMatrix();

	//プロジェクション行列の計算
	ViewProjection::UpdateProjectionMatrix();
}