#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include "Utility/CollisionManager/Collider.h"
#include "../GameObject/Weapon/Weapon.h"
#include "../GameObject/Missile/Missile.h"
#include "../UI.h"
#include "3D/Model/ParticleModel.h"
#include "3D/Particle/ParticleSystem.h"

class Player
{
public:

	~Player();

	void Initialize(Weapon* weapon);

	void Update();

	void Draw(const ViewProjection viewProjection);

	void ApplyGlobalVariables();

	void ModelMotion();

	void DrawSprite();

	void StartAnimation();
  
	void DrawParticle(const ViewProjection& viewProjection);

private:
	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	std::unique_ptr<Model> playerModelDummy_ = nullptr;

	std::unique_ptr<Model> platformModel_ = nullptr;

	WorldTransform playerWorldTransform_;

	uint32_t textureHandle_ = 0u;

	Weapon* weapon_;

	//自機の横移動スピード
	float playerMoveSpeed_ = 0.05f;

	//モデルとモーション
	enum {
		Stay,
		Move,
	};

	int  motionMode_;
	Vector3 prePlayerTranslation_;

	WorldTransform platformMotionWorldTransform_;

	struct platformMotionStruct {
		Vector3 translation_; /*playerWorldTransform_.translation_を基準としたLocal座標*/
		Vector3 rotation_;/*playerWorldTransform_.rotation_を基準としたLocal回転*/
		Vector3 scale_;/*playerWorldTransform_.scale_を基準としたLocalスケール*/
		Vector4 color_;/*色やんね*/
	};

    platformMotionStruct platformMotion_;

	//横移動
	struct platformMotionMoveStruct {
		float maxFlex_;/*伸び具合*/
		float duration;/*伸びるのにかかる時間*/
		float time;
		float timePlus;
		Vector3 flex_;
		Vector3 flexPos_;
	};

	//そもそものサイズ
	Vector3 NormalScale_;

	platformMotionMoveStruct platformMotionMove_;

	UIStruct tutorialUI_;

	float tutorialSpace = 16.0f;
	Vector2 tutorialSpriteSize = {131.0f * 2.0f,36.0f * 2.0f };

	//パーティクル
	std::unique_ptr<ParticleModel> particleModel_ = nullptr;
	std::unique_ptr<ParticleSystem> particleSystem_ = nullptr;
};



