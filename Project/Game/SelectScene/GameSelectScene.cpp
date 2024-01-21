#include "GameSelectScene.h"
#include "GameManager.h"
#include "GameScene.h"
#include <algorithm>

GameSelectScene::GameSelectScene() {};

GameSelectScene::~GameSelectScene() {};

void GameSelectScene::Initialize(GameManager* gameManager)
{
	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();

	soundHandle_ = audio_->SoundLoadWave("Resources/Sounds/Selection.wav");

	//スプライトの生成
	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });

	viewProjection_.UpdateMatrix();

	//武器の生成
	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();

	//プレイヤーの生成
	player_ = std::make_unique<Player>();
	player_->Initialize(weapon_.get());

	//天球の作成
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize();

	//ボスの生成
	boss_ = std::make_unique<Boss>();
	boss_->StartAnimationInit();
	boss_->SetWeapon(weapon_.get());

	//衝突マネージャーの生成
	collisionManager_ = std::make_unique<CollisionManager>();
};

void GameSelectScene::Update(GameManager* gameManager)
{
	if (!Input::GetInstance()->GetJoystickState(joyState_))
	{
		return;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if (isTransitionEnd_) {
			isTransition_ = true;
		}
	}

	//プレイヤーのアニメーションの更新
	player_->StartAnimation();

	//武器のアニメーションの更新
	weapon_->StartAnimaion();

	//ボスのアニメーション更新
	boss_->StartAnimation();

	//天球の更新
	backGround_->Update();


	//if (input_->IsPushKeyEnter(DIK_SPACE))
	//{
	//	if (isTransitionEnd_) {
	//		isTransition_ = true;
	//		if (soundCount_ == 0)
	//		{
	//			soundCount_ = 1;
	//			audio_->SoundPlayWave(soundHandle_, false);
	//		}
	//	}
	//}

	if (isTransitionEnd_ == false) {
		transitionTimer_ += 1.0f / kTransitionTime;
		transitionColor_.w = Lerp(transitionColor_.w, 0.0f, transitionTimer_);
		transitionSprite_->SetColor(transitionColor_);

		if (transitionColor_.w <= 0.0f) {
			isTransitionEnd_ = true;
			transitionTimer_ = 0.0f;
		}
	}

	//if (isTransition_) {
	//	transitionTimer_ += 1.0f / kTransitionTime;
	//	transitionColor_.w = Lerp(transitionColor_.w, 1.0f, transitionTimer_);
	//	transitionSprite_->SetColor(transitionColor_);

	//	if (transitionColor_.w >= 1.0f) {
	//		gameManager->ChangeScene(new GameScene);
	//	}
	//}

	viewProjection_.UpdateMatrix();

	//衝突判定
	collisionManager_->ClearColliderList();
	collisionManager_->SetColliderList(boss_.get());
	/*const std::list<std::unique_ptr<Missile>>& missiles = boss_->GetMissiles();
	for (const std::unique_ptr<Missile>& missile : missiles) {
		collisionManager_->SetColliderList(missile.get());
	}*/
	collisionManager_->SetColliderList(weapon_.get());
	collisionManager_->CheckAllCollisions();

	if (boss_->GetAnimationEnd()) {
		gameManager->ChangeScene(new GameScene());
	}
};

void GameSelectScene::Draw(GameManager* gameManager)
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

	player_->Draw(viewProjection_);

	weapon_->Draw(viewProjection_);

	//ボスの描画
	boss_->Draw(viewProjection_);

	//背景の描画
	backGround_->Draw(viewProjection_);

	Model::PostDraw();

#pragma endregion

#pragma region パーティクルの描画

	//パーティクルモデルの描画
	ParticleModel::PreDraw();

	weapon_->DrawParticle(viewProjection_);

	boss_->DrawParticle(viewProjection_);

	ParticleModel::PostDraw();

#pragma endregion

	PostProcess::GetInstance()->PostDraw();

#pragma region 前景スプライトの描画

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);

	weapon_->DrawSprite();

	boss_->DrawSprite();

	player_->DrawSprite();

	//transitionSprite_->Draw();

	Sprite::PostDraw();

#pragma endregion

};