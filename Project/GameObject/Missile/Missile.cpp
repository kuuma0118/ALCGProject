#include "missile.h"
#include "Utility/GlobalVariables.h"

float Missile::widthMin = -5.6f;
float Missile::widthMax = 5.6f;

void Missile::Initialize(const Vector3& position, const Vector3& velocity, uint32_t soundHandle)
{
	input_ = Input::GetInstance();

	audio_ = Audio::GetInstance();

	model_.reset(Model::CreateFromOBJ("Resources", "sphere.obj"));

	missileModel_.reset(Model::CreateFromOBJ("Resources/Missile", "Missile.obj"));

	textureHandle_ = TextureManager::Load("Resources/white.png");

	/*soundHandle_ = audio_->SoundLoadWave("Resources/Sounds/Missile_Bakuhatu.wav");*/
	soundHandle_ = soundHandle;

	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 0.3f,0.3f,0.3f };

	worldTransform_.UpdateMatrix();

	missileMoveSpeed_ = velocity;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeMissile);
	SetCollisionMask(kCollisionMaskMissile);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabb = { { -worldTransform_.scale_.x,-worldTransform_.scale_.y,-worldTransform_.scale_.z}, { worldTransform_.scale_.x,worldTransform_.scale_.y,worldTransform_.scale_.z} };
	SetAABB(aabb);

	//worldTransform_[0].translation_.x = 8.0f;
	//worldTransform_[0].translation_.y = RandomTY(-1.3f, 1.8f);
	//worldTransform_[0].translation_.z = 10.0f;
	//worldTransform_[0].scale_ = { 0.3f,0.3f,0.3f };

	//worldTransform_[1].translation_.x = -8.0f;
	//worldTransform_[1].translation_.y = RandomTY(-1.3f, 1.8f);
	//worldTransform_[1].translation_.z = 10.0f;
	//worldTransform_[1].scale_ = { 0.3f,0.3f,0.3f };

	//そもそものおおきさ
	normalScale_ = { 1.0f,1.0f,1.0f };

	missileMotion_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
	};
}

void Missile::Update()
{
	if (isAlive_) 
	{
		worldTransform_.translation_ = Add(worldTransform_.translation_, missileMoveSpeed_);
		worldTransform_.UpdateMatrix();
	}

	//if (worldTransform_.translation_.x < -6.5f || worldTransform_.translation_.x > 6.5f || worldTransform_.translation_.y > 13.0f) {
	//	isAlive_ = false;
	//}

	if (worldTransform_.translation_.x < -30.0f || worldTransform_.translation_.x > 30.0f || worldTransform_.translation_.y > 13.0f) {
		isAlive_ = false;
	}

	ModelMotion();
}

void Missile::Draw(const ViewProjection viewProjection)
{
	if (isAlive_) {
		//model_->Draw(worldTransform_, viewProjection, textureHandle_);

		missileModel_->Draw(missileMotionWorldTransform_, viewProjection);
	}
}

void Missile::OnCollision(uint32_t collisionAttribute, float damage)
{
	audio_->SoundPlayWave(soundHandle_, false);
	isAlive_ = false;
}

Vector3 Missile::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

void Missile::ModelMotion()
{
	if (missileMoveSpeed_.x < 0) {
		missileMotion_.rotation_.z = 3.14f;
		missileMotion_.rotation_.x -= 0.1f;
	}
	else {
		missileMotion_.rotation_.z = 0.0f;
		missileMotion_.rotation_.x -= 0.1f;
	}

	

	if (missileMotion_.rotation_.x >= 360.0f) {
		missileMotion_.rotation_.x -= 360.0f;
	}else if (missileMotion_.rotation_.x <= -360.0f) {
		missileMotion_.rotation_.x += 360.0f;
	}

	missileMotionWorldTransform_.translation_ = Add(Add(missileMotion_.translation_, worldTransform_.translation_), normalTransration_);
	missileMotionWorldTransform_.scale_ = Multiply(Multiply(missileMotion_.scale_, worldTransform_.scale_), normalScale_);
	missileMotionWorldTransform_.rotation_ = Add(missileMotion_.rotation_, worldTransform_.rotation_);
	missileModel_->GetMaterial()->SetColor(missileMotion_.color_);

	missileMotionWorldTransform_.UpdateMatrix();

}

