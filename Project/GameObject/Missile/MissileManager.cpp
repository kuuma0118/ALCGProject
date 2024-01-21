#include "MissileManager.h"
#include "Utility/GlobalVariables.h"

void MissileManager::Initialize(Weapon* weapon) {

	weapon_ = weapon;

	//ミサイルの生成
	leftMissile_ = std::make_unique<Missile>();
	rightMissile_ = std::make_unique<Missile>();

	//ミサイルの初期化
	leftMissile_->Initialize(Vector3{ -13.0f,RandomTY(-1.3f, 1.8f) ,0.0f }, Vector3{ missileMoveSpeed_,0.0f,0.0f });
	rightMissile_->Initialize(Vector3{ 13.0f,RandomTY(-1.3f, 1.8f) ,0.0f }, Vector3{ -missileMoveSpeed_,0.0f,0.0f });

	leftMissile_->SetWeapon(weapon_);
	rightMissile_->SetWeapon(weapon_);

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Missile";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "missileMoveSpeed", missileMoveSpeed_);

	MissileManager::ApplyGlobalVariables();
}

void MissileManager::Update() {

	if (leftMissile_->IsAlive() == false) {
		leftMissile_->SetPosition(Vector3{ -13.0f,RandomTY(-1.3f, 1.8f) ,0.0f });
		leftMissile_->Resporn();
	}

	if (rightMissile_->IsAlive() == false) {
		rightMissile_->SetPosition(Vector3{ 13.0f,RandomTY(-1.3f, 1.8f) ,0.0f });
		rightMissile_->Resporn();
	}

	//ミサイルの更新
	leftMissile_->Update();
	rightMissile_->Update();
}

void MissileManager::Draw(const ViewProjection& viewProjection) {
	//ミサイルの描画
	leftMissile_->Draw(viewProjection);
	rightMissile_->Draw(viewProjection);
}

float MissileManager::RandomTY(float min_value, float max_value)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min_value, max_value);

	return dis(gen); // ランダムな浮動小数点数を生成して返す
}

void MissileManager::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Missile";
	missileMoveSpeed_ = globalVariables->GetFloatValue(groupName, "missileMoveSpeed");
}