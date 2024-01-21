#pragma once
#include "IBossState.h"
#include "3D/Matrix/WorldTransform.h"

class BossStateDown : public IBossState {
public:

	//アニメーションの終わりまでの時間
	static const int kAnimationTime = 180;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossStateDown() override;

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
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//初期化時のy座標
	Vector3 initTranslation_{};
	//初期化時の角度
	Vector3 initRotation_{};
	//目標座標
	Vector3 targetTranslation_{ 0.0f,-3.3f,0.0f };;
	//目標角度
	Vector3 targetRotation_{ 1.57f,0.0f,0.0f };
	//タイマー
	float timer_ = 0.0f;

};

