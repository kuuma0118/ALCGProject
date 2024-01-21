#include "GameTitleScene.h"
#include "GameSelectScene.h"
#include "GameManager.h"
#include "GameScene.h"
#include "Components/PostProcess.h"
#include <cassert>
#include <algorithm>

GameTitleScene::GameTitleScene() {};

GameTitleScene::~GameTitleScene() {};

void GameTitleScene::Initialize(GameManager* gameManager)
{
	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();

	soundHandle_ = audio_->SoundLoadWave("Resources/Sounds/Selection.wav");
	titleSoundHandle_ = audio_->SoundLoadWave("Resources/Sounds/Title.wav");

	playerModel_.reset(Model::CreateFromOBJ("Resources/Platform", "Platform.obj"));
	weaponModel_.reset(Model::CreateFromOBJ("Resources/Head", "Head.obj"));

	playerWorldTransform_.translation_.x = 0.0f;
	playerWorldTransform_.translation_.y = -3.3f;
	playerWorldTransform_.scale_ = { 0.8f,0.8f,0.8f };

	weaponWorldTransform_.translation_.x = 0.0f;
	weaponWorldTransform_.translation_.y = -2.5f;
	weaponWorldTransform_.scale_ = { 0.8f,0.8f,0.8f };

	//背景の生成
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	viewProjection_.UpdateMatrix();

	//スプライトの生成
	//4x3用スプライトの生成
	x4x3TextureHandle_ = TextureManager::Load("Resources/Images/4x3Black.png");
	x4x3Sprite_.reset(Sprite::Create(x4x3TextureHandle_, { 0.0f,0.0f }));

	//コントローラースプライトの生成
	controllerTextureHandle_ = TextureManager::Load("Resources/Images/Cont.png");
	controllerSprite_.reset(Sprite::Create(controllerTextureHandle_, { 0.0f,0.0f }));

	titleTextureHandle_ = TextureManager::Load("Resources/Images/title.png");
	titleSprite_.reset(Sprite::Create(titleTextureHandle_, { 0.0f,0.0f }));

	pressATextureHandle_ = TextureManager::Load("Resources/Images/PressAbutton.png");
	pressASprite_.reset(Sprite::Create(pressATextureHandle_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f - 508.0f * 0.5f , 550.0f}));

	//スプライトの生成
	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });

	audio_->SoundPlayWave(titleSoundHandle_, true);

	//ポストプロセスの有効化
	PostProcess::GetInstance()->SetIsPostProcessActive(true);
	PostProcess::GetInstance()->SetIsBloomActive(true);

	//パーティクルモデルの作成
	particleModel_.reset(ParticleModel::CreateFromOBJ("Resources/Particle", "Particle.obj"));
	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->Initialize();
	textureHandle_ = TextureManager::Load("Resources/Particle.png");
};

void GameTitleScene::Update(GameManager* gameManager) 
{
	Input::GetInstance()->GetJoystickState(joyState_);

	//背景の更新
	backGround_->Update();

	playerWorldTransform_.translation_.x -= playerMoveSpeed_;
	weaponWorldTransform_.translation_.x -= weaponMoveSpeed_;

	if (playerWorldTransform_.translation_.x <= -0.0f)
	{
		playerMoveSpeed_ = -0.0f;
		weaponMoveSpeed_ = -0.0f;
	}

	if (playerWorldTransform_.translation_.x >= 1.6f)
	{
		playerMoveSpeed_ = 0.0f;
		weaponMoveSpeed_ = 0.0f;
	}


	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if (isTransitionEnd_) {
			isTransition_ = true;
			if (soundCount_ == 0)
			{
				soundCount_ = 1;
				audio_->SoundPlayWave(soundHandle_, false);
			}
		}
	}

	if (isTransitionEnd_ == false) {
		transitionTimer_ += 1.0f / kTransitionTime;
		transitionColor_.w = Lerp(transitionColor_.w, 0.0f, transitionTimer_);
		transitionSprite_->SetColor(transitionColor_);

		if (transitionColor_.w <= 0.0f) {
			isTransitionEnd_ = true;
			transitionTimer_ = 0.0f;
		}
	}

	if (isTransition_) {
		transitionTimer_ += 1.0f / kTransitionTime;
		transitionColor_.w = Lerp(transitionColor_.w, 1.0f, transitionTimer_);
		transitionSprite_->SetColor(transitionColor_);

		if (transitionColor_.w >= 1.0f) {
			audio_->StopAudio(titleSoundHandle_);
			gameManager->ChangeScene(new GameScene);

		}
	}

	if (input_->IsPushKeyEnter(DIK_P)) {
		////設定したい項目だけ.Setを呼び出す
		//ParticleEmitter* newParticleEmitter = EmitterBuilder()
		//	.SetParticleType(ParticleEmitter::ParticleType::kScale)
		//	.SetTranslation({ 0.0f,0.0f,0.0f })
		//	.SetArea({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
		//	.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
		//	.SetScale({ 0.1f, 0.1f,0.1f }, { 0.15f ,0.15f ,0.15f })
		//	.SetAzimuth(0.0f, 360.0f)
		//	.SetElevation(0.0f, 0.0f)
		//	.SetVelocity({ 0.02f ,0.02f ,0.02f }, { 0.04f ,0.04f ,0.04f })
		//	.SetColor({ 1.0f ,1.0f ,1.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f })
		//	.SetLifeTime(0.1f, 1.0f)
		//	.SetCount(100)
		//	.SetFrequency(4.0f)
		//	.SetDeleteTime(3.0f)
		//	.Build();
		//particleSystem_->AddParticleEmitter(newParticleEmitter);

		ParticleEmitter* newParticleEmitter = EmitterBuilder()
			.SetParticleType(ParticleEmitter::ParticleType::kScale)
			.SetTranslation({ 0.0f,0.0f,0.0f })
			.SetArea({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
			.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
			.SetScale({ 0.2f, 0.2f,0.2f }, { 0.25f ,0.25f ,0.25f })
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

		////火花
		//ParticleEmitter* particleEmitter = EmitterBuilder()
		//	.SetTranslation({0.0f,0.0f,0.0f})
		//	.SetArea({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
		//	.SetRotation({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f })
		//	.SetScale({ 0.1f,0.1f,0.1f }, { 0.1f,0.1f,0.1f })
		//	.SetColor({ 1.0f,0.5f,0.0f,1.0f }, { 1.0f,0.5f,0.0f,1.0f })
		//	.SetAzimuth(250.0f/* - (involvedCount_ * 5)*/, 290.0f/* + (involvedCount_ * 5)*/)
		//	.SetElevation(0.0f, 0.0f)
		//	.SetVelocity({ 0.2f,0.2f,0.2f }, { 0.4f,0.4f,0.4f })
		//	.SetLifeTime(0.4f, 0.6f)
		//	.SetCount(50/* + (involvedCount_ * 50)*/)
		//	.SetFrequency(2.0f)
		//	.SetDeleteTime(1.0f)
		//	.Build();
		//particleSystem_->AddParticleEmitter(particleEmitter);
	}

	particleSystem_->Update();

	playerWorldTransform_.UpdateMatrix();
	weaponWorldTransform_.UpdateMatrix();

	viewProjection_.UpdateMatrix();
};

void GameTitleScene::Draw(GameManager* gameManager)
{
	//背景スプライトの描画
	Sprite::PreDraw(Sprite::kBlendModeNormal);

	Sprite::PostDraw();

	DirectXCommon::GetInstance()->ClearDepthBuffer();

	PostProcess::GetInstance()->PreDraw();

	//モデルの描画
	Model::PreDraw();

	//背景の描画
	backGround_->Draw(viewProjection_);

	playerModel_->Draw(playerWorldTransform_, viewProjection_);

	weaponModel_->Draw(weaponWorldTransform_, viewProjection_);

	Model::PostDraw();

	//パーティクルの描画
	ParticleModel::PreDraw();

	particleModel_->Draw(particleSystem_.get(), viewProjection_, textureHandle_);

	ParticleModel::PostDraw();

	PostProcess::GetInstance()->PostDraw();

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);


	titleSprite_->Draw();
	
	pressASprite_->Draw();

	controllerSprite_->Draw();

	x4x3Sprite_->Draw();

	transitionSprite_->Draw();

	Sprite::PostDraw();
};