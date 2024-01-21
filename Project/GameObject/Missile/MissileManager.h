#pragma once
#include "Missile.h"
#include <list>
#include <memory>
#include <random>

class MissileManager{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Weapon* weapon);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ランダム生成
	/// </summary>
	/// <param name="min_value"></param>
	/// <param name="max_value"></param>
	/// <returns></returns>
	float RandomTY(float min_value, float max_value);

	/// <summary>
	/// グローバル変数の適応
	/// </summary>
	void ApplyGlobalVariables();

	const std::unique_ptr<Missile>& GetLeftMissile() { return leftMissile_; };

	const std::unique_ptr<Missile>& GetRightMissile() { return rightMissile_; };

	void SetWeapon(Weapon* weapon) { weapon_ = weapon; }

private:
	std::unique_ptr<Missile> leftMissile_ = nullptr;
	std::unique_ptr<Missile> rightMissile_ = nullptr;
	float missileMoveSpeed_ = 0.05f;

	Weapon* weapon_;

};

