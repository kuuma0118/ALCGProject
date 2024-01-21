#pragma once
#include "IScene.h"
#include "Base/TextureManager.h"
#include "Components/Audio.h"
#include "Components/Input.h"
#include "2D/ImGuiManager.h"
#include "3D/DebugCamera/DebugCamera.h"

#include "3D/Model/Model.h"
#include "2D/Sprite.h"
#include "3D/Matrix/WorldTransform.h"
#include "3D/Matrix/ViewProjection.h"
#include "Utility/CollisionManager/CollisionManager.h"

#include "../GameObject/Player/Player.h"
#include "../GameObject/Weapon/Weapon.h"
#include "../GameObject/Boss/Boss.h"
#include "../GameObject/Missile/Missile.h"
#include "../BackGround.h"

#include <memory>

class GameSelectScene : public IScene
{
public:
	//トランジションの時間
	static const int kTransitionTime = 60;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameSelectScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameSelectScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameManager* gameManager)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameManager* gameManager)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(GameManager* gameManager)override;

private:
	//TextureManager
	TextureManager* textureManager_ = nullptr;
	//Audio
	Audio* audio_ = nullptr;
	//Input
	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	//サウンド
	uint32_t soundHandle_ = 0u;
	int soundCount_ = 0;
  
	//トランジション用のスプライト
	std::unique_ptr<Sprite> transitionSprite_;
	//トランジションのテクスチャ
	uint32_t transitionTextureHandle_ = 0;
	//トランジションの色
	Vector4 transitionColor_ = { 0.0f,0.0f,0.0f,1.0f };
	//トランジションのフラグ
	bool isTransition_ = false;
	bool isTransitionEnd_ = false;
	//トランジションのタイマー
	float transitionTimer_ = 0;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Weapon> weapon_ = nullptr;

	//ボス
	std::unique_ptr<Boss> boss_ = nullptr;

	//天球
	std::unique_ptr<BackGround> backGround_ = nullptr;

	ViewProjection viewProjection_{};

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
};

