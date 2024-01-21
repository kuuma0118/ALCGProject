#include "ChageParticle.h"
#include "Utility/Random.h"

void ChageParticle::Initialize(const Vector3& translation, const Vector3& rotation, const Vector3& scale, const Vector3& velocity, const Vector4& color, float lifeTime) {
	//中心点の初期化
	center_ = translation;
	//角度の初期化
	rotation_ = rotation;
	//スケール
	scale_ = scale;
	//速度
	velocity_ = velocity;
	//色
	color_ = color;
	//寿命
	lifeTime_ = lifeTime;
	//アルファ値の初期化
	alpha_ = color_.w;
	//座標
	//float radius = 0.0f;
	//float angle = Random::GetRandomFloat(0.0f, 150.0f);
	//float x = radius * std::cos(angle);
	//float y = radius * std::sin(angle);
	//translation_ = { x,y,0.0f };
	translation_ = { center_.x + Random::GetRandomFloat(-2.0f,2.0f),center_.y + Random::GetRandomFloat(-2.0f,2.0f),0.0f };
}

void ChageParticle::Update() {
	const float speed = 0.1f;
	float deltaX = center_.x - translation_.x;
	float deltaY = center_.y - translation_.y;
	float length = sqrt(deltaX * deltaX + deltaY * deltaY);
	if (length > 0) {
		deltaX /= length;
		deltaY /= length;
		translation_.x += deltaX * speed;
		translation_.y += deltaY * speed;
	}

	const float kDeltaTime = 1.0f / 60.0f;
	currentTime_ += kDeltaTime;
	color_.w = alpha_ - (currentTime_ / lifeTime_);
	if (currentTime_ > lifeTime_) {
		isDead_ = true;
	}
}