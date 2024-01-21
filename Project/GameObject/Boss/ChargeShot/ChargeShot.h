#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Utility/CollisionManager/Collider.h"
#include "Utility/CollisionManager/CollisionConfig.h"
#include "3D/Particle/ParticleSystem.h"

class ChargeShot : public Collider
{
public:
	static int chargeShotEndTime;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	void Initialize(Vector3 position, float chargeShotpeed);

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
	/// 死亡フラグを取得
	/// </summary>
	/// <returns>死亡フラグ</returns>
	bool IsDead() { return isDead_; };

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="damage">ダメージ</param>
	void OnCollision(uint32_t collisionAttribute, float damage) override;

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// パーティクルシステムを設定
	/// </summary>
	/// <param name="particleSystem"></param>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }

private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//死亡フラグ
	bool isDead_ = false;
	int deadTimer_ = 0;
	//チャージショットのスケール
	Vector3 chargeShotScale_ = { 1.0f,1.0f,1.0f };
	//チャージショットの速さ
	float chargeShotSpeed_ = 0.07f;
	//パーティクルシステム
	ParticleSystem* particleSystem_ = nullptr;
};

