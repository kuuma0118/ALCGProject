#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "2D/ImGuiManager.h"
#include "Components/Input.h"
#include "Components/Audio.h"
#include <random>
#include "Utility/CollisionManager/Collider.h"
#include "Utility/CollisionManager/CollisionConfig.h"
#include "../GameObject/Weapon/Weapon.h"

class Missile : public Collider
{
public:
	static float widthMin;
	static float widthMax;

	void Initialize(const Vector3& position, const Vector3& velocity, uint32_t audioHandle);

	void Update();

	void Draw(const ViewProjection viewProjection);

	bool IsAlive() { return isAlive_; };

	void Resporn() { isAlive_ = true; };

	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; };

	void OnCollision(uint32_t collisionAttribute, float damage) override;

	Vector3 GetWorldPosition() override;

	void ModelMotion();

	void SetWeapon(Weapon* weapon) { weapon_ = weapon; }

private:
	Input* input_ = nullptr;

	Audio* audio_ = nullptr;

	std::unique_ptr<Model> model_;

	std::unique_ptr<Model> missileModel_;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	uint32_t soundHandle_ = 0u;

	Weapon* weapon_;

	Matrix4x4 localMatrix_;

	Vector3 missileMoveSpeed_ = { 0.0f,0.05f,0.0f };

	float missileFollowingSpeed_ = 0.2f;

	bool isAlive_ = true;

	bool IsFollowingWeapon_ = false;

	WorldTransform missileMotionWorldTransform_;

	struct missileMotionStruct {
		Vector3 translation_; /*worldTransform_.translation_を基準としたLocal座標*/
		Vector3 rotation_;/*worldTransform_.rotation_を基準としたLocal回転*/
		Vector3 scale_;/*worldTransform_.scale_を基準としたLocalスケール*/
		Vector4 color_;/*色やんね*/
	};

	//そもそものサイズ
	Vector3 normalScale_;
	//そもそもの位置
	Vector3 normalTransration_;

	missileMotionStruct missileMotion_;
  
	bool IsMove_ = false;
};

