#pragma once
#include "IBossState.h"
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include <array>
#include <ctime>
#include <cstdlib>
#include <memory>

class BossStateLaserAttack : public IBossState{
public:
	//警告時間
	static int WarningTime;
	static int LaserAttackEndTime;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossStateLaserAttack() override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pBoss">ボスのポインタ</param>
	void Initialize(Boss* pBoss) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pBoss">ボスのポインタ</param>
	void Update(Boss* pBoss) override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pBoss">ボスのポインタ</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(Boss* pBoss, const ViewProjection& viewProjection) override;

	/// <summary>
	/// グローバル変数の適応
	/// </summary>
	void ApplyGlobalVariables() override;

private:
	//レーザーの警告モデル
	std::array<std::unique_ptr<Model>, 2> warningModels_{};
	std::array<WorldTransform, 2>warningWorldTransforms_{};
	//レーザー攻撃までの待機時間
	int warningTimer_ = 0;
	//攻撃終了までのタイマー
	int endTimer_ = 0;
	//レーザーのスケール
	Vector3 laserScale_{ 1.0f,10.0f,0.1f };
};