#pragma once
#include "IBossState.h"
#include "../GameObject/Missile/Missile.h"
#include "3D/Matrix/WorldTransform.h"
#include <list>

class BossStateMissileAttack : public IBossState{
public:
	//演出用のミサイルの発生時間
	static const int kMissileSpornTime = 20;
	static const int kMissileMax = 3;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossStateMissileAttack() override;

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
	void Draw(Boss* pBoss, const ViewProjection& viewProjection)override;

	/// <summary>
	/// グローバル変数の適応
	/// </summary>
	void ApplyGlobalVariables() override;

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//演出用のミサイル
	std::list<std::unique_ptr<Missile>> missiles_{};
	//演出用のミサイルのタイマー
	int missileSpornTimer_ = kMissileSpornTime;
	int addMissileTimer_ = kMissileSpornTime;
	//ミサイルを出した数
	int missileCount_ = 0;
	int addMissileCount_ = 0;
	//移動フラグ
	bool isMove_ = false;
	//移動速度
	float moveSpeed_ = 0.06f;

};

