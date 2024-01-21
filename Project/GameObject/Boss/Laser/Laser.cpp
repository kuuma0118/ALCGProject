#include "Laser.h"

int Laser::deadTime = 60;

void Laser::Initialize(const Vector3& position, const Vector3& scale) {

	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Laser", "Laser.obj"));
	model_->GetDirectionalLight()->SetEnableLighting(false);
	//ワールドトランスフォームの初期化
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = scale;

	//タイマーの初期化
	deadTimer_ = deadTime;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeLaser);
	SetCollisionMask(kCollisionMaskLaser);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabbSize{ {-scale.x,-scale.y,-scale.z},{scale.x,scale.y,scale.z} };
	SetAABB(aabbSize);
}

void Laser::Update() {
	worldTransform_.translation_.y -= laserSpeed_;

	if (worldTransform_.translation_.y < 0.0f) {
		worldTransform_.translation_.y = 0.0f;

		if (isDead_ == false)
		{
			ParticleEmitter* newParticleEmitter = EmitterBuilder()
				.SetParticleType(ParticleEmitter::ParticleType::kScale)
				.SetTranslation({ worldTransform_.translation_.x,-4.0f,worldTransform_.translation_.z })
				.SetArea({ -1.0f,0.0f,-2.0f }, { 1.0f,0.3f,2.0f })
				.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
				.SetScale({ 0.2f, 0.2f,0.2f }, { 0.25f ,0.25f ,0.25f })
				.SetAzimuth(0.0f, 180.0f)
				.SetElevation(0.0f, 0.0f)
				.SetVelocity({ 0.02f ,0.02f ,0.02f }, { 0.04f ,0.04f ,0.04f })
				.SetColor({ 1.0f ,1.0f ,1.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f })
				.SetLifeTime(0.1f, 1.0f)
				.SetCount(80)
				.SetFrequency(4.0f)
				.SetDeleteTime(3.0f)
				.Build();
			particleSystem_->AddParticleEmitter(newParticleEmitter);
		}
	}

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//死亡フラグを立てる
	if (--deadTimer_ < 0) {
		isDead_ = true;
	}
}

void Laser::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void Laser::OnCollision(uint32_t collisionAttribute, float damage) {

}

Vector3 Laser::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}