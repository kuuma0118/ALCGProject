#pragma once
#include "IBossState.h"
#include "3D/Matrix/WorldTransform.h"
#include "Utility/MathFunction.h"

class BossStateWait : public IBossState {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossStateWait() override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pBoss"></param>
	void Initialize(Boss* pBoss) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pBoss"></param>
	void Update(Boss* pBoss) override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pBoss"></param>
	/// <param name="viewProjection"></param>
	void Draw(Boss* pBoss, const ViewProjection& viewProjection) override;

	/// <summary>
	/// グローバル変数の適応
	/// </summary>
	void ApplyGlobalVariables() override;

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//回転速度
	float rotSpeed_ = 0.02f;

};

