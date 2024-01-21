#pragma once
#include "Components/Audio.h"
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "Utility/CollisionManager/Collider.h"
#include "State/IBossState.h"
#include "Laser/Laser.h"
#include "../GameObject/Missile/Missile.h"
#include "ChargeShot/ChargeShot.h"
#include <random>
#include "../UI.h"
#include "3D/Model/ParticleModel.h"
#include "3D/Particle/ParticleSystem.h"
#include <sstream>

class Weapon;

/// <summary>
/// ボス
/// </summary>
class Boss : public Collider {
public:
	//ミサイルの発生時間
	static int MissileSpornTime;

	static const int kHpMax = 100;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Boss();

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
	/// ミサイルを追加
	/// </summary>
	/// <param name="missile">ミサイル</param>
	/*void AddMissile(Missile* missile);*/
  
	/// チャージショットを追加
	/// </summary>
	/// <param name="chargeShot">チャージショット</param>
	/*void AddChargeShot(ChargeShot* chargeShot);*/

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
	const std::list<std::unique_ptr<Laser>>& GetLaser() const { return lasers_; };

	/// <summary>
	/// ミサイルのリストを取得
	/// </summary>
	/// <returns>ミサイルのリスト</returns>
	/*const std::list<std::unique_ptr<Missile>>& GetMissiles() const { return missiles_; };*/

	///// <summary>
	///// 攻撃が当たったかのフラグを取得
	///// </summary>
	///// <param name="IsHit"></param>
	//void IsHit(bool IsHit) { IsHit_ = IsHit; };

	/// <summary>
	/// ランダム生成
	/// </summary>
	/// <param name="min_value"></param>
	/// <param name="max_value"></param>
	/// <returns></returns>
	float Random(float min_value, float max_value);

	/// <summary>
	/// ランダム生成
	/// </summary>
	/// <param name="min_value"></param>
	/// <param name="max_value"></param>
	/// <returns></returns>
	int Random(int min_value, int max_value);

	/// <summary>
	/// 進行方向を取得
	/// </summary>
	/// <returns></returns>
	int GetMoveDirection() { return moveDirection_; };

	/// <summary>
	/// 進行方向を設定
	/// </summary>
	/// <param name="moveDirection"></param>
	void SetMoveDirection(int moveDirection) { moveDirection_ = moveDirection; };

	/// <summary>
	/// グローバル変数の適応
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// プレイヤーのクラスを借りる
	/// </summary>
	/// <param name="weapon"></param>
	void SetWeapon(Weapon* weapon) { weapon_ = weapon; };

	/// <summary>
	/// ミサイルの当たった数を設定
	/// </summary>
	/// <param name="missileCount"></param>
	void SetHitMissileCount(int missileCount) { hitMissileCount_ = missileCount; };

	/// <summary>
	/// チャージショットのリストを取得
	/// </summary>
	/// <returns>チャージショットのリスト</returns>
	//const std::list<std::unique_ptr<ChargeShot>>& GetChargeShot() { return chargeShot_; };

	/// <summary>
	/// ミサイルの当たった数を取得
	/// </summary>
	/// <returns></returns>
	int GetHitMissileCount() { return hitMissileCount_; };

	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns></returns>
	float GetHP() { return Hp_; };

	bool GetIsActive() { return isActive_; };

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision(uint32_t collisionAttribute, float damage) override;

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// モーション用関数
	/// </summary>
	void ModelMotion();

	/// <summary>
	/// スプライトのDraw
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// HpBar関連の関数
	/// </summary>
	/// <returns></returns>
	void HPBarUpdate();

	/// <summary>
	/// エミッターを追加
	/// </summary>
	/// <param name="particleEmitter"></param>
	void AddParticleEmitter(ParticleEmitter* particleEmitter) { particleSystem_->AddParticleEmitter(particleEmitter); };

	/// <summary>
	/// エミッターを取得
	/// </summary>
	ParticleEmitter* GetParticleEmitter(const std::string& name) { return particleSystem_->GetParticleEmitter(name); };

	/// <summary>
	/// パーティクルの描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void DrawParticle(const ViewProjection& viewProjection);

	/// <summary>
	/// ゲームスタート時のアニメーション初期化
	/// </summary>
	void StartAnimationInit();

	/// <summary>
	/// ゲームスタート時のアニメーション
	/// </summary>
	void StartAnimation();

	/// <summary>
	/// アニメーションのフラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetAnimationEnd() { return animationEnd_; };
  
	/// パーティクルシステムを取得
	/// </summary>
	/// <returns></returns>
	ParticleSystem* GetParticleSystem() { return particleSystem_.get(); };

	/// <summary>
	/// ミサイルのサウンドハンドルを取得
	/// </summary>
	uint32_t GetMissileSoundHandle() { return missileSoundHandle_; };

	void SetIsDead() { isDead_ = true; };

	bool GetIsDead() { return isDead_; };

	bool GetIsDeadAnimation() { return isDeadAnimation_; };

	/// <summary>
	/// ミサイルデータの読み込み
	/// </summary>
	void LoadMissileData();

private:
	Audio* audio_ = nullptr;

	uint32_t soundHandle_[2];

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
	////ミサイルのリスト
	//std::list<std::unique_ptr<Missile>> missiles_{};
	//チャージショットのリスト
	std::list<std::unique_ptr<ChargeShot>> chargeShot_{};
	//最大体力
	const float maxHp_ = 430.0f;
	//体力
	float Hp_ = maxHp_;
	//当たったミサイルの数
	int hitMissileCount_ = 0;
	//ミサイルのスポーンタイマー
	int currentMissileSpornTime_ = 0;
	int missileSpornTimer_ = 0;
	//ミサイルの進行方向
	int missileDirection_ = 1;
	//ミサイルの速度
	float missileMoveSpeed_ = 0.05f;
	//進行方向
	int moveDirection_ = 1;
	bool isActive_ = false;

	//プレイヤー
	Weapon* weapon_ = nullptr;


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

	//HPバー系
	UIStruct hpBar_;
	const float barSpace = 16.0f;
	float barSize = WinApp::GetInstance()->kClientWidth - 320 - barSpace * 2;

	//パーティクル
	std::unique_ptr<ParticleModel> particleModel_ = nullptr;
	std::unique_ptr<ParticleSystem> particleSystem_ = nullptr;

	//アニメーション用のフラグ
	bool animationFlag_ = false;
	bool animationEnd_ = false;

	//ミサイルの爆発音
	uint32_t missileSoundHandle_ = 0;

	bool isDead_ = false;
	bool isDeadAnimation_ = false;




	//ミサイルのパターン
	struct MissilePattern {
		std::vector<Vector3> position;
		std::vector<Vector3> velocity;
	};

	//ミサイルのテンプレート
	struct MissileTemplate {
		std::vector<MissilePattern> missilePatterns;
		int spornTime = 0;
		int spornTimer = 0;
	};

	//ミサイルのテンプレが変わる閾値
	int missileTemplateThreshold1_ = 290;
	int missileTemplateThreshold2_ = 140;
	int missileTemplateThreshold3_ = 0;

	//テンプレートの配列
	std::vector<MissileTemplate> missileTemplates_;

	//ミサイルCSVの読み込み
	std::stringstream missileData_;

	//テンプレートのインデックス
	int templateIndex_ = 0;
};