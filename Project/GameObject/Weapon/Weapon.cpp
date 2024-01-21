#include "Weapon.h"
#include <Math.h>
#include "../GameObject/Player/Player.h"
#include "Utility/GlobalVariables.h"

//実体定義
int Weapon::InvincibleTime = 60;

Weapon::~Weapon()
{
	for (int i = 0; i < MaxHp_; i++) {
		delete heartUI_[i].sprite_;
	}
}

void Weapon::Initialize()
{
  
	weaponModelDummy_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));

	weaponModel_.reset(Model::CreateFromOBJ("Resources/Head", "Head.obj"));
	weaponRodModel_.reset(Model::CreateFromOBJ("Resources/Rod", "rod.obj"));

	involvedMissile_.reset(Model::CreateFromOBJ("Resources/Missile", "Missile.obj"));
  

	input_ = Input::GetInstance();

	audio_ = Audio::GetInstance();

	//Weapon
	weaponWorldTransform_.translation_.x = 0.0f;
	weaponWorldTransform_.translation_.y = 1.8f;
	weaponWorldTransform_.scale_ = { 0.4f,0.4f,0.4f };
	
	//Missile
	involvedMissileWorldTransform_.translation_ = { 0.0f,-1.5f,0.0f };
	involvedMissileWorldTransform_.scale_ = { 0.3f,0.3f,0.3f };
	involvedMissileWorldTransform_.rotation_.z = 1.57f;

	weaponWorldTransform_.UpdateMatrix();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Weapon";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "attackSpeedNormal", attackSpeed_[0]);
	globalVariables->AddItem(groupName, "attackSpeedLevel1", attackSpeed_[1]);
	globalVariables->AddItem(groupName, "attackSpeedLevel2", attackSpeed_[2]);
	globalVariables->AddItem(groupName, "attackSpeedLevel3", attackSpeed_[3]);
	globalVariables->AddItem(groupName, "attackDamageNormal", attackDamage_[0]);
	globalVariables->AddItem(groupName, "attackDamageLevel1", attackDamage_[1]);
	globalVariables->AddItem(groupName, "attackDamageLevel2", attackDamage_[2]);
	globalVariables->AddItem(groupName, "attackDamageLevel3", attackDamage_[3]);
	globalVariables->AddItem(groupName, "chargeSpeed", chargeSpeed_);
	globalVariables->AddItem(groupName, "InvincibleTime", InvincibleTime);

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetRadius(weaponWorldTransform_.scale_.x);

	//モーション

	weaponMotion_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
		{ 0.0f,0.1f,0.0f },
		{ 1.7f,1.7f,1.7f },

	};

	weaponRodMotion_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
		{ 0.0f,0.0f,0.0f },
		{ 1.7f,(3.3f + 2.2f)  * 0.5f - 0.2f,1.7f },
	};


	chargeRotateSpeed_ = 0.2f;
	for (int i = 0; i < 4; i++) {
		attackRotateSpeed_[i] = i * 0.2f;;
	}

	ModelMotion();

	Hp_ = MaxHp_;


	//HP表示の隙間；
	float hpSpace = 16.0f;
	float hpSpriteSize = 64.0f;

	for (int i = 0; i < MaxHp_; i++) {
		heartUI_[i] = {
			true,
			TextureManager::Load("Resources/Images/heart.png"),
			{ hpSpace + (hpSpace + hpSpriteSize)* i + 160 ,float(WinApp::GetInstance()->kClientHeight) - hpSpace - hpSpriteSize},
			0.0f,
			{1.0f,1.0f},
			nullptr,
		};

		heartUI_[i].sprite_ = Sprite::Create(heartUI_[i].textureHandle_, heartUI_[i].position_);
	}

	redTextureHandle_ = TextureManager::Load("Resources/Images/Red.png");
	redSprite_.reset(Sprite::Create(redTextureHandle_, { 0.0f,0.0f }));
	redSprite_->SetColor({1.0f,1.0f ,1.0f ,0.3f});

	particleModel_.reset(ParticleModel::CreateFromOBJ("Resources/Particle", "Particle.obj"));
	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->Initialize();

	soundHandle_[0] = audio_->SoundLoadWave("Resources/Sounds/Misslie_Sasaru.wav");
	soundHandle_[1] = audio_->SoundLoadWave("Resources/Sounds/Player_Damage.wav");
	soundHandle_[2] = audio_->SoundLoadWave("Resources/Sounds/Head_Charge.wav");
	soundHandle_[3] = audio_->SoundLoadWave("Resources/Sounds/Head_Attack.wav");
}

void Weapon::Update()
{
	XINPUT_STATE preJoyState_ = joyState_;
	Vector3 move = { 0, 0, 0 };

	if (!Input::GetInstance()->GetJoystickState(joyState_))
	{
		return;
	}

	//HP描画
	for (int i = 0; i < MaxHp_; i++) {
		heartUI_[i].isDraw_ = false;
	}
	for (int i = 0; i < Hp_; i++) {
		heartUI_[i].isDraw_ = true;
	}

	//プレイヤーの左右移動
	if (Input::GetInstance()->GetJoystickState(joyState_) && IsAttack_ == false)
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
			move = Multiply(weaponMoveSpeed_, Normalize(move));

			weaponWorldTransform_.translation_ = Add(weaponWorldTransform_.translation_, move);

			if (weaponWorldTransform_.translation_.x <= Missile::widthMin)
			{
				weaponWorldTransform_.translation_.x = Missile::widthMin;
			}

			if (weaponWorldTransform_.translation_.x >= Missile::widthMax)
			{
				weaponWorldTransform_.translation_.x = Missile::widthMax;
			}
		}
	}

	weaponWorldTransform_.translation_ = Add(weaponWorldTransform_.translation_, move);
	weaponWorldTransform_.matWorld_ = MakeAffineMatrix(
		weaponWorldTransform_.scale_, weaponWorldTransform_.rotation_, weaponWorldTransform_.translation_);

	//攻撃処理
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A && IsAttack_ == false && IsCoolDown_ == false)
	{
		
		pushCount_++;
		IsCharge_ = true;
		

	}else if(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A && IsCoolDown_ == false){
		if (pushCount_ < 12)
		{
			audio_->SoundPlayWave(soundHandle_[3], false);
			IsCharge_ = false;
			IsAttack_ = true;
			pushCount_ = 0;
			//発射パーティクル
			ParticleEmitter* newParticleEmitter = EmitterBuilder()
				.SetEmitterName("Attack")
				.SetParticleType(ParticleEmitter::ParticleType::kNormal)
				.SetTranslation(weaponWorldTransform_.translation_)
				.SetArea({ -0.4f,-0.6f,0.0f }, { 0.4f,-0.6f,0.0f })
				.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
				.SetScale({ 0.05f, 0.6f,0.1f }, { 0.05f ,0.8f ,0.1f })
				.SetAzimuth(0.0f, 0.0f)
				.SetElevation(0.0f, 0.0f)
				.SetVelocity({ 0.0f ,0.0f ,0.0f }, { 0.0f ,0.0f ,0.0f })
				.SetColor({ 1.0f ,1.0f ,1.0f,0.4f }, { 1.0f ,1.0f ,1.0f ,0.6f })
				.SetLifeTime(0.15f, 0.15f)
				.SetCount(20)
				.SetFrequency(0.01f)
				.SetDeleteTime(2.0f)
				.Build();
			particleSystem_->AddParticleEmitter(newParticleEmitter);
		}

		if (pushCount_ >= 12)
		{
			IsCharge_ = false;
			pushCount_ = 0;
		}
	}


	if (IsCharge_ == true)
	{
		chargeCount_++;
		weaponWorldTransform_.translation_.y -= chargeSpeed_;

		if (weaponWorldTransform_.translation_.y > -2.3f)
		{
			audio_->SoundPlayWave(soundHandle_[2], false);
		}

		if (weaponWorldTransform_.translation_.y <= -2.3f)
		{
			weaponWorldTransform_.translation_.y = -2.3f;
		}
	}

	if (IsAttack_ == true && chargeCount_ < 20)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[0] + involvedCount_ * missileDamage);
		//エミッターの位置を変更
		for (ParticleEmitter* particleEmitter : particleSystem_->GetParticleEmitters("Attack")) {
			particleEmitter->SetTranslation(weaponWorldTransform_.translation_);
		}

		if (weaponWorldTransform_.translation_.y >= 2.2f)
		{
			weaponWorldTransform_.translation_.y = 2.2f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
			//パーティクルを発生させないようにする
			for (ParticleEmitter* particleEmitter : particleSystem_->GetParticleEmitters("Attack")) {
				particleEmitter->SetPopCount(0);
			}
		}
	}

	//チャージ1
	if (IsAttack_ == true && chargeCount_ >= 20 && chargeCount_ < 50)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[1] + involvedCount_ * missileDamage);
		//エミッターの位置を変更
		for (ParticleEmitter* particleEmitter : particleSystem_->GetParticleEmitters("Attack")) {
			particleEmitter->SetTranslation(weaponWorldTransform_.translation_);
		}

		if (weaponWorldTransform_.translation_.y >= 2.9f)
		{
			weaponWorldTransform_.translation_.y = 2.9f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
			//パーティクルを発生させないようにする
			for (ParticleEmitter* particleEmitter : particleSystem_->GetParticleEmitters("Attack")) {
				particleEmitter->SetPopCount(0);
			}
		}
	}

	//チャージ2
	if (IsAttack_ == true && chargeCount_ >= 50 && chargeCount_ < 90)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[2] + involvedCount_ * missileDamage);
		//エミッターの位置を変更
		for (ParticleEmitter* particleEmitter : particleSystem_->GetParticleEmitters("Attack")) {
			particleEmitter->SetTranslation(weaponWorldTransform_.translation_);
		}

		if (weaponWorldTransform_.translation_.y >= 3.5f)
		{
			weaponWorldTransform_.translation_.y = 3.5f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
			//パーティクルを発生させないようにする
			for (ParticleEmitter* particleEmitter : particleSystem_->GetParticleEmitters("Attack")) {
				particleEmitter->SetPopCount(0);
			}
		}
	}

	//チャージ3
	if (IsAttack_ == true && chargeCount_ >= 90)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[3] + involvedCount_ * missileDamage);
		//エミッターの位置を変更
		for (ParticleEmitter* particleEmitter : particleSystem_->GetParticleEmitters("Attack")) {
			particleEmitter->SetTranslation(weaponWorldTransform_.translation_);
		}

		if (weaponWorldTransform_.translation_.y >= 4.5f)
		{
			weaponWorldTransform_.translation_.y = 4.5f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
			//パーティクルを発生させないようにする
			for (ParticleEmitter* particleEmitter : particleSystem_->GetParticleEmitters("Attack")) {
				particleEmitter->SetPopCount(0);
			}
		}
	}

	//攻撃後のクールダウン
	if (IsCoolDown_ == true)
	{
		coolDownTimer_--;
		weaponWorldTransform_.translation_.y -= coolDownSpeed_;
		if (weaponWorldTransform_.translation_.y <= 1.8f)
		{
			weaponWorldTransform_.translation_.y = 1.8f;
			coolDownTimer_ = 60;
			IsCoolDown_ = false;
			IsHit_ = false;
			/*isInvolvedMissile_ = false;*/
			involvedCount_ = 0;
			involvedMissile_->GetMaterial()->SetColor(missileColor_[involvedCount_]);
		}
	}

	//巻き込んだミサイルの処理
	if (isInvolvedMissile_)
	{
		involvedMissileWorldTransform_.translation_ = {
				weaponWorldTransform_.translation_.x,
				weaponWorldTransform_.translation_.y + 0.25f,
				weaponWorldTransform_.translation_.z,
		};
		involvedMissile_->GetMaterial()->SetColor(missileColor_[involvedCount_ - 1]);
	}

	weaponWorldTransform_.UpdateMatrix();
  
	ModelMotion();
  
	involvedMissileWorldTransform_.UpdateMatrix();


	//パーティクルの更新
	particleSystem_->Update();


	//無敵時間の処理
	if (invincibleFlag_) {
		invincibleTimer_--;
		if (invincibleTimer_ < 0) {
			invincibleFlag_ = false;
		}

		if (invincibleTimer_ > 40) {
			damageRed = true;
		}
		else {
			damageRed = false;
		}
	}

  
	Weapon::ApplyGlobalVariables();
}

void Weapon::Draw(const ViewProjection viewProjection)
{
  
	weaponModel_->Draw(weaponMotionWorldTransform_, viewProjection);
	weaponRodModel_->Draw(weaponRodMotionWorldTransform_, viewProjection);
	//weaponModelDummy_->Draw(weaponWorldTransform_, viewProjection);

	//ミサイルを巻き込んでいるときに描画する
	if (isInvolvedMissile_) {
		involvedMissile_->Draw(involvedMissileWorldTransform_, viewProjection);
	}
  
}

void Weapon::DrawParticle(const ViewProjection& viewProjection) {
	//パーティクルの描画
	particleModel_->Draw(particleSystem_.get(), viewProjection);
}

void Weapon::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Weapon";
	attackSpeed_[0] = globalVariables->GetFloatValue(groupName, "attackSpeedNormal");
	attackSpeed_[1] = globalVariables->GetFloatValue(groupName, "attackSpeedLevel1");
	attackSpeed_[2] = globalVariables->GetFloatValue(groupName, "attackSpeedLevel2");
	attackSpeed_[3] = globalVariables->GetFloatValue(groupName, "attackSpeedLevel3");
	attackDamage_[0] = globalVariables->GetFloatValue(groupName, "attackDamageNormal");
	attackDamage_[1] = globalVariables->GetFloatValue(groupName, "attackDamageLevel1");
	attackDamage_[2] = globalVariables->GetFloatValue(groupName, "attackDamageLevel2");
	attackDamage_[3] = globalVariables->GetFloatValue(groupName, "attackDamageLevel3");
	chargeSpeed_ = globalVariables->GetFloatValue(groupName, "chargeSpeed");
	InvincibleTime = globalVariables->GetIntValue(groupName, "InvincibleTime");
}

void Weapon::OnCollision(uint32_t collisionAttribute, float damage)
{
	//衝突相手がボスの場合
	if (collisionAttribute & kCollisionAttributeEnemy)
	{		
		if (IsHit_ == false && IsCoolDown_ == false) 
		{
			//衝撃
			ParticleEmitter* newParticleEmitter = EmitterBuilder()
				.SetParticleType(ParticleEmitter::ParticleType::kScale)
				.SetTranslation(weaponWorldTransform_.translation_)
				.SetArea({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
				.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
				.SetScale({ 0.1f, 0.1f,0.1f }, { 0.15f ,0.15f ,0.15f })
				.SetAzimuth(0.0f, 360.0f)
				.SetElevation(0.0f, 0.0f)
				.SetVelocity({ 0.02f ,0.02f ,0.02f }, { 0.04f ,0.04f ,0.04f })
				.SetColor({ 1.0f ,1.0f ,1.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f })
				.SetLifeTime(0.1f, 1.0f)
				.SetCount(100)
				.SetFrequency(4.0f)
				.SetDeleteTime(3.0f)
				.Build();
			particleSystem_->AddParticleEmitter(newParticleEmitter);

			//火花
			ParticleEmitter* particleEmitter = EmitterBuilder()
				.SetTranslation(weaponWorldTransform_.translation_)
				.SetArea({0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f})
				.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
				.SetScale({ 0.1f,0.1f,0.1f }, { 0.1f,0.1f,0.1f })
				.SetColor({ 1.0f,0.5f,0.0f,1.0f }, { 1.0f,0.5f,0.0f,1.0f })
				.SetAzimuth(250.0f - (involvedCount_ * 5), 290.0f + (involvedCount_ * 5))
				.SetElevation(0.0f, 0.0f)
				.SetVelocity({ 0.2f,0.2f,0.2f }, { 0.4f,0.4f,0.4f })
				.SetLifeTime(0.4f, 0.6f)
				.SetCount(50 + (involvedCount_ * 50))
				.SetFrequency(2.0f)
				.SetDeleteTime(1.0f)
				.Build();
			particleSystem_->AddParticleEmitter(particleEmitter);

			IsHit_ = true;
			isInvolvedMissile_ = false;
			involvedCount_ = 0;
		}

		if (IsCoolDown_ == false) {
			//衝撃波
			for (uint32_t i = 0; i < 90; ++i) {
				ParticleEmitter* particleEmitter = EmitterBuilder()
					.SetParticleType(ParticleEmitter::ParticleType::kNormal)
					.SetTranslation(weaponWorldTransform_.translation_)
					.SetArea({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
					.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
					.SetScale({ 0.1f,0.1f,0.1f }, { 0.1f,0.1f,0.1f })
					.SetColor({ 1.0f,0.5f,0.0f,1.0f }, { 1.0f,0.5f,0.0f,1.0f })
					.SetAzimuth(float(i) * 4.0f, float(i) * 4.0f)
					.SetElevation(0.0f, 0.0f)
					.SetVelocity({ 0.04f,0.04f,0.04f }, { 0.04f,0.04f,0.04f })
					.SetLifeTime(0.4f, 0.4f)
					.SetCount(1)
					.SetFrequency(2.0f)
					.SetDeleteTime(1.0f)
					.Build();
				particleSystem_->AddParticleEmitter(particleEmitter);
			}
		}
	}
	//ボス以外の場合
	else
	{
		//攻撃中の場合
		if (IsAttack_)
		{
			//衝突相手がミサイルの場合カウントを増やす
			if (collisionAttribute & kCollisionAttributeMissile) 
			{
				audio_->SoundPlayWave(soundHandle_[0], false);
				isInvolvedMissile_ = true;
				if (involvedCount_ < 5) {
					involvedCount_++;
				}
				involvedMissileWorldTransform_.translation_ = {
				weaponWorldTransform_.translation_.x,
				weaponWorldTransform_.translation_.y + 0.25f,
				weaponWorldTransform_.translation_.z,
				};
				involvedMissileWorldTransform_.UpdateMatrix();
			}
			//ミサイル以外の場合はダメージを食らう
			else {
				//無敵状態でなければダメージを食らう
				if (invincibleFlag_ == false && IsDeadAnimation_ == false)
				{
					audio_->SoundPlayWave(soundHandle_[1], false);
					invincibleFlag_ = true;
					invincibleTimer_ = InvincibleTime;
					Hp_ -= int(damage);
				}
			}
		}
		//攻撃中ではない場合
		else
		{
			//無敵状態でなければダメージを食らう
			if (invincibleFlag_ == false && IsDeadAnimation_ == false)
			{
				audio_->SoundPlayWave(soundHandle_[1], false);
				invincibleFlag_ = true;
				invincibleTimer_ = InvincibleTime;
				Hp_ -= int(damage);
			}
		}
	}
}

Vector3 Weapon::GetWorldPosition()
{
	Vector3 pos;
	pos.x = weaponWorldTransform_.matWorld_.m[3][0];
	pos.y = weaponWorldTransform_.matWorld_.m[3][1];
	pos.z = weaponWorldTransform_.matWorld_.m[3][2];
	return pos;
}

void Weapon::ModelMotion()
{
	motionMode_ = Stay;

	if (IsCharge_ == true && chargeCount_< 90) {
		motionMode_ = Charge;
	}else if (IsAttack_ == true) {
		motionMode_ = Attack;
	}

	switch (motionMode_)
	{
	default:
		case  Stay:

		break;
		case  Charge:
			weaponMotion_.rotation_.y += chargeRotateSpeed_;
			break;
		case  Attack:

			if (chargeCount_ < 20)
			{
				weaponMotion_.rotation_.y += attackRotateSpeed_[0];
			}
			else if (chargeCount_ >= 20 && chargeCount_ < 50)
			{
				weaponMotion_.rotation_.y += attackRotateSpeed_[1];
			}
			else if (chargeCount_ >= 50 && chargeCount_ < 90)
			{
				weaponMotion_.rotation_.y += attackRotateSpeed_[2];
			}
			else if (chargeCount_ >= 90 )
			{
				weaponMotion_.rotation_.y += attackRotateSpeed_[3];
			}
			break;
	}

	//ヘッドとプラットフォームまでの距離
	float distance = weaponWorldTransform_.translation_.y -playerPosY;
	float distanceHalf = distance * 0.5f;

	weaponRodMotion_.translation_.y = -distanceHalf;

	weaponRodMotion_.scale_.y = distance;

	weaponMotionWorldTransform_.translation_ = Add(Add(weaponMotion_.translation_, weaponWorldTransform_.translation_), weaponMotion_.normalTransration_);
	weaponMotionWorldTransform_.scale_ = Multiply(Multiply(weaponMotion_.scale_, weaponWorldTransform_.scale_), weaponMotion_.normalScale_);
	weaponMotionWorldTransform_.rotation_ = Add(weaponMotion_.rotation_, weaponWorldTransform_.rotation_);
	weaponModel_->GetMaterial()->SetColor(weaponMotion_.color_);

	weaponRodMotionWorldTransform_.translation_ = Add(Add(weaponRodMotion_.translation_, weaponWorldTransform_.translation_), weaponRodMotion_.normalTransration_);
	weaponRodMotionWorldTransform_.scale_ = Multiply(Multiply(weaponRodMotion_.scale_, weaponWorldTransform_.scale_), weaponRodMotion_.normalScale_);
	weaponRodMotionWorldTransform_.rotation_ = Add(weaponRodMotion_.rotation_, weaponWorldTransform_.rotation_);
	weaponRodModel_->GetMaterial()->SetColor(weaponRodMotion_.color_);

	weaponMotionWorldTransform_.UpdateMatrix();
	weaponRodMotionWorldTransform_.UpdateMatrix();
}

void Weapon::DrawSprite()
{
	for (int i = 0; i < MaxHp_; i++) {
		if (heartUI_[i].isDraw_ == true) {
			heartUI_[i].sprite_->Draw();
		}
	}

	if (damageRed == true) {
		redSprite_->Draw();
	}
}


void Weapon::StartAnimaion() {

	if (animationFlag_) {
		if (IsAttack_ == false && IsCoolDown_ == false) {
			pushCount_++;
			IsCharge_ = true;
		}

		if (chargeCount_ >= 90) {
			audio_->SoundPlayWave(soundHandle_[3], false);
			IsCharge_ = false;
			IsAttack_ = true;
			pushCount_ = 0;
			animationFlag_ = false;
		}
	}

	//更新
	if (IsCharge_ == true)
	{
		audio_->SoundPlayWave(soundHandle_[2], false);
		chargeCount_++;
		weaponWorldTransform_.translation_.y -= chargeSpeed_;

		if (weaponWorldTransform_.translation_.y <= -2.3f)
		{
			weaponWorldTransform_.translation_.y = -2.3f;
		}
	}

	if (IsAttack_ == true && chargeCount_ < 20)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[0] + involvedCount_ * missileDamage);

		if (weaponWorldTransform_.translation_.y >= 2.2f)
		{
			weaponWorldTransform_.translation_.y = 2.2f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
		}
	}

	//チャージ1
	if (IsAttack_ == true && chargeCount_ >= 20 && chargeCount_ < 50)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[1] + involvedCount_ * missileDamage);

		if (weaponWorldTransform_.translation_.y >= 2.9f)
		{
			weaponWorldTransform_.translation_.y = 2.9f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
		}
	}

	//チャージ2
	if (IsAttack_ == true && chargeCount_ >= 50 && chargeCount_ < 90)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[2] + involvedCount_ * missileDamage);

		if (weaponWorldTransform_.translation_.y >= 3.5f)
		{
			weaponWorldTransform_.translation_.y = 3.5f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
		}
	}

	//チャージ3
	if (IsAttack_ == true && chargeCount_ >= 90)
	{
		weaponWorldTransform_.translation_.y += attackSpeed_[3];
		SetDamage(attackDamage_[3] + involvedCount_ * missileDamage);

		if (weaponWorldTransform_.translation_.y >= 5.0f)
		{
			weaponWorldTransform_.translation_.y = 5.0f;
			chargeCount_ = 0;
			IsAttack_ = false;
			IsCoolDown_ = true;
		}
	}

	//攻撃後のクールダウン
	if (IsCoolDown_ == true)
	{
		coolDownTimer_--;
		weaponWorldTransform_.translation_.y -= coolDownSpeed_;
		if (weaponWorldTransform_.translation_.y <= 1.8f)
		{
			weaponWorldTransform_.translation_.y = 1.8f;
			coolDownTimer_ = 30;
			IsCoolDown_ = false;
			IsHit_ = false;
			/*isInvolvedMissile_ = false;*/
			involvedCount_ = 0;
			involvedMissile_->GetMaterial()->SetColor(missileColor_[involvedCount_]);
		}
	}

	//巻き込んだミサイルの処理
	if (isInvolvedMissile_)
	{
		involvedMissileWorldTransform_.translation_ = {
				weaponWorldTransform_.translation_.x,
				weaponWorldTransform_.translation_.y + 0.25f,
				weaponWorldTransform_.translation_.z,
		};
		involvedMissile_->GetMaterial()->SetColor(missileColor_[involvedCount_ - 1]);
	}

	weaponWorldTransform_.UpdateMatrix();

	ModelMotion();

	involvedMissileWorldTransform_.UpdateMatrix();

	//パーティクルの処理
	particleSystem_->Update();


	//無敵時間の処理
	if (invincibleFlag_) {
		invincibleTimer_--;
		if (invincibleTimer_ < 0) {
			invincibleFlag_ = false;
		}
	}


	Weapon::ApplyGlobalVariables();
}

