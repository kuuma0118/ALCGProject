#include "Particle.h"

void Particle::Initialize(const Vector3& translation, const Vector3& rotation, const Vector3& scale, const Vector3& velocity, const Vector4& color, float lifeTime) {
	//座標を設定
	translation_ = translation;
	//角度
	rotation_ = rotation;
	//スケール
	scale_ = { scale.x,scale.x,scale.x };
	//速度
	velocity_ = velocity;
	//色
	color_ = color;
	//寿命
	lifeTime_ = lifeTime;
}

void Particle::Update() {
	//今回はxyz全部大きさ同じなので、xから大きさを取るよ
	float scale = scale_.x;
	scale -= scaleMinus_;
	if (scale < 0.0f) {
		scale = 0.0f;
		isDead_ = true;
	}
	scale_ = { scale,scale,scale };
	translation_ = Add(translation_, velocity_);
}