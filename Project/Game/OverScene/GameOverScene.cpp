#include "GameOverScene.h"
#include "GameManager.h"
#include "GameTitleScene.h"
#include "GameScene.h"
#include <cassert>

GameOverScene::GameOverScene() {};

GameOverScene::~GameOverScene() {};

void GameOverScene::Initialize(GameManager* gameManager)
{
	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();

	soundHandle_ = audio_->SoundLoadWave("Resources/Sounds/Selection.wav");
	overSoundHandle_ = audio_->SoundLoadWave("Resources/Sounds/GameOver.wav");
  
	playerModel_.reset(Model::CreateFromOBJ("Resources/Platform", "Platform.obj"));
	weaponModel_.reset(Model::CreateFromOBJ("Resources/Head", "Head.obj"));
	bossModel_.reset(Model::CreateFromOBJ("Resources/Boss", "Boss.obj"));

	playerWorldTransform_.translation_ = { -4.5f,-3.2f,0.0f };
	playerWorldTransform_.rotation_.z = 1.6f;
	playerWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };

	weaponWorldTransform_.translation_ = { -5.5f,-3.2f,0.0f };
	weaponWorldTransform_.rotation_.z = 1.6f;
	playerWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };

	bossWorldTransform_.translation_ = { 2.5f,-1.2f,-3.0f };
	bossWorldTransform_.rotation_.z = 0.6f;
	bossWorldTransform_.scale_ = { 1.5f,1.5f,1.5f };

	//背景の生成
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	viewProjection_.UpdateMatrix();

	audio_->SoundPlayWave(overSoundHandle_, true);
  
	//スプライトの生成
	loseTextureHandle_ = TextureManager::Load("Resources/Images/lose.png");
	loseSprite_.reset(Sprite::Create(loseTextureHandle_, { 0.0f,0.0f }));

	//4x3用スプライトの生成
	x4x3TextureHandle_ = TextureManager::Load("Resources/Images/4x3Black.png");
	x4x3Sprite_.reset(Sprite::Create(x4x3TextureHandle_, { 0.0f,0.0f }));

	pressATextureHandle_ = TextureManager::Load("Resources/Images/PressAbutton.png");
	pressASprite_.reset(Sprite::Create(pressATextureHandle_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f - 508.0f * 0.5f , 550.0f }));

	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });
};

void GameOverScene::Update(GameManager* gameManager)
{
	//背景の更新
	backGround_->Update();

	if (!Input::GetInstance()->GetJoystickState(joyState_))
	{
		return;
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

	if (input_->IsPushKeyEnter(DIK_T))
	{
		audio_->StopAudio(overSoundHandle_);
		gameManager->ChangeScene(new GameTitleScene());
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
			audio_->StopAudio(overSoundHandle_);
			gameManager->ChangeScene(new GameScene);
		}
	}

	playerWorldTransform_.UpdateMatrix();
	weaponWorldTransform_.UpdateMatrix();
	bossWorldTransform_.UpdateMatrix();

	viewProjection_.UpdateMatrix();
};

void GameOverScene::Draw(GameManager* gameManager)
{
	PostProcess::GetInstance()->PreDraw();

#pragma region 背景スプライトの描画

	//背景スプライトの描画
	Sprite::PreDraw(Sprite::kBlendModeNormal);

	Sprite::PostDraw();

#pragma endregion

	//深度バッファをクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();

#pragma region モデルの描画

	//モデルの描画
	Model::PreDraw();

	//背景の描画
	backGround_->Draw(viewProjection_);

	playerModel_->Draw(playerWorldTransform_, viewProjection_);

	weaponModel_->Draw(weaponWorldTransform_, viewProjection_);

	bossModel_->Draw(bossWorldTransform_, viewProjection_);

	Model::PostDraw();

#pragma endregion

#pragma region パーティクルの描画

	//パーティクルモデルの描画
	ParticleModel::PreDraw();

	ParticleModel::PostDraw();

#pragma endregion

	PostProcess::GetInstance()->PostDraw();

#pragma region 前景スプライトの描画

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);

	loseSprite_->Draw();

	x4x3Sprite_->Draw();

	pressASprite_->Draw();

	transitionSprite_->Draw();

	Sprite::PostDraw();

#pragma endregion

};