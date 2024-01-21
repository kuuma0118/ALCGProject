#pragma once
#include "IBossState.h"
#include "2D/ImGuiManager.h"
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "Components/Input.h"

class BossStateStun : public IBossState
{
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossStateStun() override;

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
	Input* input_ = nullptr;

	int stunTimer_;
};

