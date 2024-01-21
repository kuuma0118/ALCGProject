#include "GameManager.h"
#include "GameTitleScene.h"
#include "GameSelectScene.h"
#include "GameScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "Utility/GlobalVariables.h"

GameManager::GameManager() {

	//ゲームウィンドウ作成
	winApp_ = WinApp::GetInstance();
	winApp_->CreateGameWindow(L"LE2B_30_ワタナベ_クウマ_チョコレートアタック", winApp_->kClientWidth, winApp_->kClientHeight);

	//DirectXの初期化
	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize();

	//TextureManagerの初期化
	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize();

	//ImGuiの初期化
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize();

	//Audioの初期化
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	//Inputの初期化
	input_ = Input::GetInstance();
	input_->Initialize();

	//PostProcessの初期化
	postProcess_ = PostProcess::GetInstance();
	postProcess_->Initialize();

	//モデルの静的初期化
	Model::StaticInitialize();

	//パーティクルの静的初期化
	ParticleModel::StaticInitialize();

	//スプライトの静的初期化
	Sprite::StaticInitialize();

	//グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();

	//ランダムエンジンの初期化
	Random::Initialize();

	//シーンの初期化
	currentScene_ = new GameTitleScene();
	currentScene_->Initialize(this);
}

GameManager::~GameManager() {

	//シーンの削除
	delete currentScene_;
	currentScene_ = nullptr;

	Model::Release();
	ParticleModel::Release();
	Sprite::Release();
}

void GameManager::ChangeScene(IScene* newScene) {

	currentScene_ = nullptr;
	currentScene_ = newScene;
	currentScene_->Initialize(this);
}

void GameManager::run() {

	while (true) {
		//メッセージ処理
		if (winApp_->ProcessMessage()) {
			break;
		}

		//ImGui受付開始
		imguiManager_->Begin();
		//Inputの更新
		input_->Update();
		//グローバル変数の更新
	/*	GlobalVariables::GetInstance()->Update();*/
		//ゲームシーンの更新
		currentScene_->Update(this);
		//ポストプロセスの更新
		postProcess_->Update();
		//ImGui受付終了
		imguiManager_->End();

		//描画開始
		dxCommon_->PreDraw();
		//ゲームシーンの描画
		currentScene_->Draw(this);
		//ImGuiの描画
		imguiManager_->Draw();
		//描画終了
		dxCommon_->PostDraw();
	}
	//ImGuiの解放処理
	imguiManager_->ShutDown();
	//Audioの解放処理
	audio_->Finalize();
	//ゲームウィンドウ削除
	winApp_->CloseGameWindow();
}