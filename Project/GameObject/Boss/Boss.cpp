#include "Boss.h"
#include "State/BossStateNormal.h"
#include "State/BossStateDown.h"
#include "2D/ImGuiManager.h"
#include "../GameObject/Weapon/Weapon.h"
#include "Utility/GlobalVariables.h"

//実体定義
int Boss::MissileSpornTime = 90;

Boss::~Boss()
{
	delete hpBar_.sprite_;
}

void Boss::Initialize() {

	audio_ = Audio::GetInstance();

	soundHandle_[0] = audio_->SoundLoadWave("Resources/Sounds/Boss_Damage.wav");
	soundHandle_[1] = audio_->SoundLoadWave("Resources/Sounds/Missile_Bakuhatu.wav");

	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/Sphere", "sphere.obj"));

	bossModel_.reset(Model::CreateFromOBJ("Resources/Boss", "Boss.obj"));
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.y = 3.3f;
	worldTransform_.UpdateMatrix();

	//ボスの行動パターンの初期化
	state_ = std::make_unique<BossStateNormal>();
	state_->Initialize(this);

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);

	//Motionの値

	//そもそものおおきさ
	normalScale_ = { 0.4f,0.4f,0.4f };

	bossMotion_ = {
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
	};

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Missile";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "missileMoveSpeed", missileMoveSpeed_);
	globalVariables->AddItem(groupName, "missileSpornTime", MissileSpornTime);

	hpBar_ = {
		true,
		TextureManager::Load("Resources/Images/bossHp.png"),
		{barSpace  + 160, barSpace},
		0.0f,
		{barSize  ,1.0f},
		nullptr,
	};

	hpBar_.sprite_ = Sprite::Create(hpBar_.textureHandle_, hpBar_.position_);

	//パーティクル
	particleModel_.reset(ParticleModel::CreateFromOBJ("Resources/Particle", "Particle.obj"));
	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->Initialize();

	//ミサイルの爆発音
	missileSoundHandle_ = audio_->SoundLoadWave("Resources/Sounds/Missile_Bakuhatu.wav");

	//ミサイルのスポーンタイムの初期化
	currentMissileSpornTime_ = MissileSpornTime * 2;

	LoadMissileData();
}

void Boss::Update() {

	//グローバル変数の適応
	Boss::ApplyGlobalVariables();

	//状態の更新
	state_->Update(this);

	if (Hp_ < 0 && isDeadAnimation_ == false) {
		isDeadAnimation_ = true;
		ChangeState(new BossStateDown());
		if (particleSystem_->GetParticleEmitter("Charge")) {
			particleSystem_->GetParticleEmitter("Charge")->SetPopCount(0);
		}
	}

	////ミサイルの発生頻度を変更
	//currentMissileSpornTime_ = int(float(MissileSpornTime + MissileSpornTime * Hp_ / maxHp_));

	////ミサイルを生成
	//if (--missileSpornTimer_ < 0 && isDeadAnimation_ == false) {
	//	missileSpornTimer_ = currentMissileSpornTime_;
	//	missileDirection_ *= -1;
	//	Missile* missile = new Missile();

	//	missile->Initialize(Vector3{ 6.5f * missileDirection_,Random(/*-1.3f,*/-2.2f, 1.0f) ,0.0f }, Vector3{ missileMoveSpeed_ * (missileDirection_ * -1),0.0f,0.0f }, missileSoundHandle_);

	//	Boss::AddMissile(missile);
	//}

	//残り体力に応じてテンプレートを変える
	if (Hp_ >= missileTemplateThreshold1_) {
		templateIndex_ = 0;
	}
	else if (Hp_ < missileTemplateThreshold1_ && Hp_ >= missileTemplateThreshold2_) {
		templateIndex_ = 1;
	}
	else if (Hp_ < missileTemplateThreshold2_ && Hp_ >= missileTemplateThreshold3_) {
		templateIndex_ = 2;
	}


	////ミサイルの発射処理
	//if (++missileTemplates_[templateIndex_].spornTimer >= missileTemplates_[templateIndex_].spornTime) {
	//	//タイマーのリセット
	//	missileTemplates_[templateIndex_].spornTimer = 0;
	//	//ミサイルのパターンをランダムで決める
	//	int index = Random(0, static_cast<int>(missileTemplates_[templateIndex_].missilePatterns.size() - 1));
	//	//ミサイルの生成
	//	for (int i = 0; i < missileTemplates_[templateIndex_].missilePatterns[index].position.size(); i++) {
	//		Missile* missile = new Missile;
	//		missile->Initialize(missileTemplates_[templateIndex_].missilePatterns[index].position[i], missileTemplates_[templateIndex_].missilePatterns[index].velocity[i], missileSoundHandle_);
	//		AddMissile(missile);
	//	}
	//}


	//死亡フラグの立ったレーザーをリストから削除
	lasers_.remove_if([](std::unique_ptr<Laser>& laser) {
		if (laser->IsDead()) {
			laser.reset();
			return true;
		}
		return false;
	});

	//レーザーの更新
	for (std::unique_ptr<Laser>& laser : lasers_) {
		laser->Update();
	}

	//死亡フラグの立ったチャージショットをリストから削除
	chargeShot_.remove_if([](std::unique_ptr<ChargeShot>& chargeShot) {
		if (chargeShot->IsDead()) {
			chargeShot.reset();
			return true;
		}
		return false;
	});

	//チャージショットの更新
	for (std::unique_ptr<ChargeShot>& chargeShot : chargeShot_) {
		chargeShot->Update();
	}

	////死亡フラグの立ったミサイルをリストから削除
	//missiles_.remove_if([](std::unique_ptr<Missile>& missile) {
	//	if (missile->IsAlive() == false) {
	//		missile.reset();
	//		return true;
	//	}
	//	return false;
	//});

	////ミサイルの更新
	//for (std::unique_ptr<Missile>& missile : missiles_) {
	//	missile->Update();
	//}

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//モーション更新
	ModelMotion();
	//バー
	HPBarUpdate();

	//パーティクルの更新
	particleSystem_->Update();
}

void Boss::Draw(const ViewProjection& viewProjection) {

	//ボスのモデルの描画
	//model_->Draw(worldTransform_, viewProjection);

	bossModel_->Draw(bossMotionWorldTransform_, viewProjection);

	//状態の描画
	state_->Draw(this, viewProjection);

	//レーザーの描画
	for (std::unique_ptr<Laser>& laser : lasers_) {
		laser->Draw(viewProjection);
	}

	//レーザーの描画
	for (std::unique_ptr<ChargeShot>& chargeShot : chargeShot_) {
		chargeShot->Draw(viewProjection);
	}

	////ミサイルの描画
	//for (std::unique_ptr<Missile>& missile : missiles_) {
	//	missile->Draw(viewProjection);
	//}
}

void Boss::ChangeState(IBossState* state) {
	state_.reset(state);
	state_->Initialize(this);
}

void Boss::AddLaser(Laser* laser) {
	lasers_.push_back(std::unique_ptr<Laser>(laser));
}

//void Boss::AddMissile(Missile* missile){
//	missiles_.push_back(std::unique_ptr<Missile>(missile));
//}
//
//void Boss::AddChargeShot(ChargeShot* chargeShot) {
//
//	//レーザーをリストに追加
//	chargeShot_.push_back(std::unique_ptr<ChargeShot>(chargeShot));
//}

float Boss::Random(float min_value, float max_value)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min_value, max_value);

	return dis(gen); // ランダムな浮動小数点数を生成して返す
}

int Boss::Random(int min_value, int max_value) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min_value, max_value);

	return dis(gen); // ランダムな浮動小数点数を生成して返す
}

void Boss::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Missile";
	missileMoveSpeed_ = globalVariables->GetFloatValue(groupName, "missileMoveSpeed");
	MissileSpornTime = globalVariables->GetIntValue(groupName, "missileSpornTime");
}

void Boss::OnCollision(uint32_t collisionAttribute, float damage) {
  
	if (weapon_->GetIsHit() == false && weapon_->GetIsCoolDown() == false && isActive_) {
		audio_->SoundPlayWave(soundHandle_[0], false);
		audio_->SoundPlayWave(soundHandle_[1], false);
		Hp_ -= damage;
		if (collisionAttribute & kCollisionAttributePlayer) {
			hitMissileCount_ += weapon_->GetInvolvedMissileCount();
		}
	}

	if (weapon_->GetInvolvedMissileCount() > 0 && isActive_ == false) {
		audio_->SoundPlayWave(soundHandle_[0], false);
		isActive_ = true;
		if (weapon_->GetIsHit() == false) {
			Hp_ -= damage;
		}
	}

	animationFlag_ = true;
}

Vector3 Boss::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}

void Boss::ModelMotion()
{

	bossMotionWorldTransform_.translation_ = Add(Add(bossMotion_.translation_, worldTransform_.translation_), normalTransration_);
	bossMotionWorldTransform_.scale_ = Multiply(Multiply(bossMotion_.scale_, worldTransform_.scale_), normalScale_);
	bossMotionWorldTransform_.rotation_ = Add(bossMotion_.rotation_, worldTransform_.rotation_);
	bossModel_->GetMaterial()->SetColor(bossMotion_.color_);

	bossMotionWorldTransform_.UpdateMatrix();

}

void Boss::DrawSprite()
{

	hpBar_.sprite_->Draw();

}

void Boss::HPBarUpdate()
{

	hpBar_.size_ = {(Hp_ / maxHp_) * barSize,1.0f };

	hpBar_.sprite_->SetSize(hpBar_.size_);
}

void Boss::DrawParticle(const ViewProjection& viewProjection) {
	particleModel_->Draw(particleSystem_.get(), viewProjection);
}

void Boss::StartAnimationInit() {
	//初期化
	Boss::Initialize();
	worldTransform_.rotation_.y = 3.14f;
	/*Missile* missile[2];
	missile[0] = new Missile();
	missile[0]->Initialize({ 5.0f ,0.4f,0.0f }, { -missileMoveSpeed_ ,0.0f,0.0f }, missileSoundHandle_);
	missiles_.push_back(std::unique_ptr<Missile>(missile[0]));
	missile[1] = new Missile();
	missile[1]->Initialize({ -5.0f ,-0.2f,0.0f }, { missileMoveSpeed_ ,0.0f,0.0f }, missileSoundHandle_);
	missiles_.push_back(std::unique_ptr<Missile>(missile[1]));*/
}

void Boss::StartAnimation() {

	//グローバル変数の適応
	Boss::ApplyGlobalVariables();

	if (animationEnd_ == false) {
		if (animationFlag_) {
			const float rotSpeed = 0.02f;
			worldTransform_.rotation_.y -= rotSpeed;
		}

		if (worldTransform_.rotation_.y <= 0.0f) {
			animationEnd_ = true;
		}
	}

	////死亡フラグの立ったミサイルをリストから削除
	//missiles_.remove_if([](std::unique_ptr<Missile>& missile) {
	//	if (missile->IsAlive() == false) {
	//		missile.reset();
	//		return true;
	//	}
	//	return false;
	//	});

	////ミサイルの更新
	//for (std::unique_ptr<Missile>& missile : missiles_) {
	//	missile->Update();
	//}

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//モーション更新
	ModelMotion();
	//バー
	HPBarUpdate();

	//パーティクルの更新
	particleSystem_->Update();
}

void Boss::LoadMissileData() {
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/Missile.csv");
	assert(file.is_open());

	//ファイル内容をコピー
	missileData_ << file.rdbuf();
	file.close();

	//読み込み
	uint32_t templateIndex = 0;
	uint32_t patternIndex = 0;
	std::string line;
	while (getline(missileData_, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("Template") == 0) {
			MissileTemplate missileTemplate;
			getline(line_stream, word, ',');
			missileTemplate.spornTime = (int)std::atof(word.c_str());
			missileTemplates_.push_back(missileTemplate);
			templateIndex++;
		}
		else if (word.find("Pattern") == 0) {
			MissilePattern missilePattern;
			missileTemplates_[templateIndex - 1].missilePatterns.push_back(missilePattern);
			patternIndex++;
		}
		else if (word.find("Missile") == 0) {
			getline(line_stream, word, ',');
			float posX = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float posY = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float posZ = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float velX = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float velY = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float velZ = (float)std::atof(word.c_str());

			missileTemplates_[templateIndex - 1].missilePatterns[patternIndex - 1].position.push_back(Vector3{ posX,posY,posZ });
			missileTemplates_[templateIndex - 1].missilePatterns[patternIndex - 1].velocity.push_back(Vector3{ velX,velY,velZ });
		}
		else if (word.find("End") == 0) {
			patternIndex = 0;
		}
	}
}
