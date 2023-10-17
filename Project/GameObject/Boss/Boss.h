#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "Utility/CollisionManager/Collider.h"
#include "State/IBossState.h"
#include "Laser/Laser.h"

/// <summary>
/// ボス
/// </summary>
class Boss : public Collider {
public:
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
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 行動パターンの変更
	/// </summary>
	/// <param name="state">行動パターン</param>
	void ChangeState(IBossState* state);

	/// <summary>
	/// レーザーを追加
	/// </summary>
	/// <param name="laser">レーザー</param>
	void AddLaser(Laser* laser);

	/// <summary>
	/// ワールドトランスフォームを設定
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	void SetWorldTransform(const WorldTransform& worldTransform) { worldTransform_ = worldTransform; };

	/// <summary>
	/// ワールドトランスフォームの取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	const WorldTransform& GetWorldTransform() const { return worldTransform_; };

	/// <summary>
	/// レーザーのリストを取得
	/// </summary>
	/// <returns>レーザーのリスト</returns>
	const std::list<std::unique_ptr<Laser>>& GetLaser() { return lasers_; };

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// モーション用関数
	/// </summary>
	void ModelMotion();

private:
	//ボスのモデル(ダミー)
	std::unique_ptr<Model> model_ = nullptr;

	//ボスのモデル
	std::unique_ptr<Model> bossModel_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_{};

	//ボスの行動パターン
	std::unique_ptr<IBossState> state_ = nullptr;

	//レーザーのリスト
	std::list<std::unique_ptr<Laser>> lasers_{};


	//モデルとモーション

	WorldTransform bossMotionWorldTransform_{};;

	struct bossMotionStruct {
		Vector3 translation_; /*worldTransform_.translation_を基準としたLocal座標*/
		Vector3 rotation_;/*worldTransform_.rotation_を基準としたLocal回転*/
		Vector3 scale_;/*worldTransform_.scale_を基準としたLocalスケール*/
		Vector4 color_;/*色やんね*/
	};

	//そもそものサイズ
	Vector3 normalScale_;
	//そもそもの位置
	Vector3 normalTransration_;

	bossMotionStruct bossMotion_;

};