#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Utility/GlobalVariables.h"

Player::~Player()
{
	delete tutorialUI_.sprite_;
}

void Player::Initialize(Weapon* weapon)
{
  
	playerModelDummy_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));
	platformModel_.reset(Model::CreateFromOBJ("Resources/Platform", "Platform.obj"));
  
	weapon_ = weapon;

	textureHandle_ = TextureManager::Load("Resources/uvChecker.png");

	input_ = Input::GetInstance();

	//Player
	playerWorldTransform_.translation_.x = 0.0f;
	playerWorldTransform_.translation_.y = -3.3f;
	playerWorldTransform_.scale_ = { 0.8f,0.8f,0.8f };

	playerWorldTransform_.UpdateMatrix();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "playerMoveSpeed", playerMoveSpeed_);


	//Motion
	platformMotion_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
	};

	NormalScale_ = { 1.0f,1.0f,1.0f };

	platformMotionMove_ = {
		0.1f,
		20,
		0,
		1,
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
	};

	ModelMotion();

	tutorialUI_ = {
			true,
			TextureManager::Load("Resources/Images/rule.png"),
			{ float(WinApp::GetInstance()->kClientWidth) - tutorialSpace - tutorialSpriteSize.x - 160,float(WinApp::GetInstance()->kClientHeight) - tutorialSpace - tutorialSpriteSize.y},
			0.0f,
			{1.0f,1.0f},
			nullptr,
	};

	tutorialUI_.sprite_ = Sprite::Create(tutorialUI_.textureHandle_, tutorialUI_.position_);

	//パーティクルの初期化
	particleModel_.reset(ParticleModel::CreateFromOBJ("Resources/Particle", "Particle.obj"));
	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->Initialize();

	//エミッターの作成
	ParticleEmitter* particleEmitter = EmitterBuilder()
		.SetArea({ -0.5f,-0.5f,0.0f }, { 0.5f,-0.5f,0.0f })
		.SetAzimuth(0.0f, 0.0f)
		.SetColor({ 1.0f,1.0f,1.0f,0.5f }, { 1.0f,1.0f,1.0f,0.5f })
		.SetCount(10)
		.SetDeleteTime(60)
		.SetElevation(0.0f, 0.0f)
		.SetEmitterName("PlayerMove")
		.SetFrequency(0.1f)
		.SetLifeTime(0.4f, 0.4f)
		.SetParticleType(ParticleEmitter::ParticleType::kScale)
		.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
		.SetScale({ 0.2f,0.2f,0.2f }, { 0.2f,0.2f,0.2f })
		.SetTranslation(playerWorldTransform_.translation_)
		.SetVelocity({ 0.1f,0.1f,0.1f }, { 0.1f,0.1f,0.1f })
		.Build();
	particleSystem_->AddParticleEmitter(particleEmitter);
}

void Player::Update() 
{
	//エミッターが消えていたら再生成
	if (particleSystem_->GetParticleEmitter("PlayerMove") == nullptr) {
		//エミッターの作成
		ParticleEmitter* particleEmitter = EmitterBuilder()
			.SetArea({ -0.5f,-0.5f,0.0f }, { 0.5f,-0.5f,0.0f })
			.SetAzimuth(180.0f, 180.0f)
			.SetColor({ 1.0f,1.0f,1.0f,0.5f }, { 1.0f,1.0f,1.0f,0.5f })
			.SetCount(10)
			.SetDeleteTime(60)
			.SetElevation(0.0f, 0.0f)
			.SetEmitterName("PlayerMove")
			.SetFrequency(0.1f)
			.SetLifeTime(0.4f, 0.4f)
			.SetParticleType(ParticleEmitter::ParticleType::kScale)
			.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
			.SetScale({ 0.2f,0.2f,0.2f }, { 0.2f,0.2f,0.2f })
			.SetTranslation(playerWorldTransform_.translation_)
			.SetVelocity({ 0.1f,0.1f,0.1f }, { 0.1f,0.1f,0.1f })
			.Build();
		particleSystem_->AddParticleEmitter(particleEmitter);
	}

	//パーティクルを出さないようにする
	particleSystem_->GetParticleEmitter("PlayerMove")->SetPopCount(0);


	//プレイヤーの左右移動
	if (Input::GetInstance()->GetJoystickState(joyState_) && weapon_->GetIsAttack() == false)
	{
		const float deadZone = 0.7f;

		bool isMoving = false;

		Vector3 move = { (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f,0.0f };

		if (Length(move) > deadZone)
		{
			isMoving = true;
		}

		if (isMoving)
		{
			move = Multiply(playerMoveSpeed_, Normalize(move));

			playerWorldTransform_.translation_ = Add(playerWorldTransform_.translation_, move);

			//移動中はパーティクルを出す
			if (move.x > 0.0f) {
				particleSystem_->GetParticleEmitter("PlayerMove")->SetPopArea({ -1.0f,-0.5f,0.0f }, { -1.0f,-0.5f,0.0f });
				particleSystem_->GetParticleEmitter("PlayerMove")->SetPopAzimuth(180.0f, 180.0f);
				particleSystem_->GetParticleEmitter("PlayerMove")->SetPopCount(10);
				particleSystem_->GetParticleEmitter("PlayerMove")->SetTranslation(playerWorldTransform_.translation_);
			}
			else if (move.x <= 0.0f) {
				particleSystem_->GetParticleEmitter("PlayerMove")->SetPopArea({ 1.0f,-0.5f,0.0f }, { 1.0f,-0.5f,0.0f });
				particleSystem_->GetParticleEmitter("PlayerMove")->SetPopAzimuth(0.0f, 0.0f);
				particleSystem_->GetParticleEmitter("PlayerMove")->SetPopCount(10);
				particleSystem_->GetParticleEmitter("PlayerMove")->SetTranslation(playerWorldTransform_.translation_);
			}

			
			if (playerWorldTransform_.translation_.x <= Missile::widthMin)
			{
				playerWorldTransform_.translation_.x = Missile::widthMin;
			}

			if (playerWorldTransform_.translation_.x >= Missile::widthMax)
			{
				playerWorldTransform_.translation_.x = Missile::widthMax;
			}

			
		}
		ModelMotion();
	}

	
	playerWorldTransform_.UpdateMatrix();
	prePlayerTranslation_ = playerWorldTransform_.translation_;
	Player::ApplyGlobalVariables();

	//パーティクルの更新
	particleSystem_->Update();
}

void Player::Draw(const ViewProjection viewProjection)
{
	platformModel_->Draw(platformMotionWorldTransform_, viewProjection);

	//playerModelDummy_->Draw(playerWorldTransform_, viewProjection, textureHandle_);
}

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	playerMoveSpeed_ = globalVariables->GetFloatValue(groupName, "playerMoveSpeed");
}

void Player::ModelMotion()
{
	//MoveMotion
	{

		motionMode_ = Stay;

		if (prePlayerTranslation_.x != playerWorldTransform_.translation_.x) {
			motionMode_ = Move;
		}

		switch (motionMode_)
		{
		case Stay:
		default:

			platformMotion_.translation_ = {0.0f,0.0f ,0.0f };
			platformMotion_.rotation_ = { 0.0f,0.0f ,0.0f };
			platformMotion_.scale_ = { 1.0f,1.0f ,1.0f };

			break;
		case Move:
			//
			platformMotionMove_.time += platformMotionMove_.timePlus;

			if (platformMotionMove_.time >= platformMotionMove_.duration || platformMotionMove_.time <= 0) {
				platformMotionMove_.time = std::clamp(platformMotionMove_.time, 0.0f, platformMotionMove_.duration);
				platformMotionMove_.timePlus *= -1;
			}

			float t = platformMotionMove_.time / platformMotionMove_.duration;
			float ta = t < 0.5 ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2;

			platformMotionMove_.flex_.x = NormalScale_.x + platformMotionMove_.maxFlex_ * ta - platformMotionMove_.maxFlex_ * 0.5f;
			platformMotionMove_.flex_.y = NormalScale_.y + platformMotionMove_.maxFlex_ * (1.0f - ta) - platformMotionMove_.maxFlex_ * 0.5f;
			platformMotionMove_.flexPos_.y = (platformMotionMove_.maxFlex_ * ta) * -0.5f;

			platformMotion_.scale_ = platformMotionMove_.flex_;
			platformMotion_.translation_ = platformMotionMove_.flexPos_;

			break;
		}
	}

	platformMotionWorldTransform_.translation_ = Add(platformMotion_.translation_, playerWorldTransform_.translation_);
	platformMotionWorldTransform_.scale_ = Multiply(platformMotion_.scale_, playerWorldTransform_.scale_);
	platformMotionWorldTransform_.rotation_ = Add(platformMotion_.rotation_, playerWorldTransform_.rotation_);
	platformModel_->GetMaterial()->SetColor(platformMotion_.color_);

	platformMotionWorldTransform_.UpdateMatrix();
}

void Player::DrawSprite()
{
	tutorialUI_.sprite_->Draw();
}

void Player::StartAnimation() {
	//更新
	playerWorldTransform_.UpdateMatrix();
	ModelMotion();
	prePlayerTranslation_ = playerWorldTransform_.translation_;
	Player::ApplyGlobalVariables();
}

void Player::DrawParticle(const ViewProjection& viewProjection) {
	//パーティクルモデルの描画
	particleModel_->Draw(particleSystem_.get(), viewProjection);
}
