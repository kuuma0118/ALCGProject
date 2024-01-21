#include "BossStateDown.h"
#include "../GameObject/Boss/Boss.h"

BossStateDown::~BossStateDown() {

}

void BossStateDown::Initialize(Boss* pBoss) {
	//ワールドトランスフォームの初期化
	worldTransform_ = pBoss->GetWorldTransform();
	initTranslation_ = worldTransform_.translation_;
	initRotation_ = worldTransform_.rotation_;

	targetTranslation_.x = initTranslation_.x;
	targetTranslation_.z = initTranslation_.z;
}

void BossStateDown::Update(Boss* pBoss) {

	ParticleEmitter* particleEmitter = EmitterBuilder()
		.SetParticleType(ParticleEmitter::ParticleType::kScale)
		.SetTranslation(worldTransform_.translation_)
		.SetArea({ -2.0f,-2.0f,-2.0f }, { 2.0f,2.0f,2.0f })
		.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
		.SetScale({ 0.1f, 0.1f,0.1f }, { 0.15f ,0.15f ,0.15f })
		.SetAzimuth(0.0f, 360.0f)
		.SetElevation(0.0f, 0.0f)
		.SetVelocity({ 0.2f ,0.2f ,0.2f }, { 0.2f ,0.2f ,0.2f })
		.SetColor({ 1.0f,0.5f,0.0f,1.0f }, { 1.0f,0.5f,0.0f,1.0f })
		.SetLifeTime(0.1f, 1.0f)
		.SetCount(100)
		.SetFrequency(4.0f)
		.SetDeleteTime(1.0f)
		.Build();
	pBoss->AddParticleEmitter(particleEmitter);

	if (timer_ < 1.0f) {
		timer_ += 1.0f / kAnimationTime;
	}

	//移動処理
	worldTransform_.translation_ = Lerp(initTranslation_, targetTranslation_, timer_);

	//回転処理
	worldTransform_.rotation_ = Lerp(initRotation_, targetRotation_, timer_);

	//ワールドトランスフォームの更新
	pBoss->SetWorldTransform(worldTransform_);

	if (timer_ > 1.0f) {
		pBoss->SetIsDead();
	}
}

void BossStateDown::Draw(Boss* pBoss, const ViewProjection& viewProjection) {

}

void BossStateDown::ApplyGlobalVariables() {

}