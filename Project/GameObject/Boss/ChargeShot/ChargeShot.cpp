#include "ChargeShot.h"
#include "../GameObject/Missile/Missile.h"

int ChargeShot::chargeShotEndTime = 120;

void ChargeShot::Initialize(Vector3 position, float chargeShotpeed) {
	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/ChargeBeam", "ChargeBeam.obj"));
	model_->GetDirectionalLight()->SetEnableLighting(false);
	//ワールドトランスフォームの初期化
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y = -1.0f;

	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 3.7f;
	worldTransform_.scale_.z = 0.5f;

	chargeShotSpeed_ = chargeShotpeed;
	deadTimer_ = chargeShotEndTime;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeLaser);
	SetCollisionMask(kCollisionMaskLaser);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabbSize{ {-worldTransform_.scale_.x,-worldTransform_.scale_.y,-worldTransform_.scale_.z},{worldTransform_.scale_.x,worldTransform_.scale_.y,worldTransform_.scale_.z} };
	SetAABB(aabbSize);
}

void ChargeShot::Update() {
	worldTransform_.translation_.x -= chargeShotSpeed_;

	ParticleEmitter* newParticleEmitter = EmitterBuilder()
			.SetParticleType(ParticleEmitter::ParticleType::kScale)
			.SetTranslation({ worldTransform_.translation_.x,-4.0f,worldTransform_.translation_.z })
			.SetArea({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
			.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
			.SetScale({ 0.2f, 0.2f,0.2f }, { 0.25f ,0.25f ,0.25f })
			.SetAzimuth(0.0f, 360.0f)
			.SetElevation(0.0f, 0.0f)
			.SetVelocity({ 0.02f ,0.02f ,0.02f }, { 0.04f ,0.04f ,0.04f })
			.SetColor({ 1.0f ,0.0f ,0.0f ,1.0f }, { 1.0f ,0.0f ,0.0f ,1.0f })
			.SetLifeTime(0.1f, 1.0f)
			.SetCount(100)
			.SetFrequency(4.0f)
			.SetDeleteTime(3.0f)
			.Build();
		particleSystem_->AddParticleEmitter(newParticleEmitter);

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();


	//死亡フラグを立てる
	if (--deadTimer_ < 0)
	{
		isDead_ = true;
	}
}

void ChargeShot::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void ChargeShot::OnCollision(uint32_t collisionAttribute, float damage) {

}

Vector3 ChargeShot::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}