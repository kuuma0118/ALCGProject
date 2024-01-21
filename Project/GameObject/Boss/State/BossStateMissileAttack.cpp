#include "BossStateMissileAttack.h"
#include "BossStateNormal.h"
#include "../GameObject/Boss/Boss.h"
#include "Utility/GlobalVariables.h"

BossStateMissileAttack::~BossStateMissileAttack() {

}

void BossStateMissileAttack::Initialize(Boss* pBoss) {
	//ワールドトランスフォームの初期化
	worldTransform_ = pBoss->GetWorldTransform();
	//速度の初期化
	if (pBoss->GetMoveDirection() == 1) {
		moveSpeed_ = moveSpeed_;
	}
	else {
		moveSpeed_ = -moveSpeed_;
	}
}

void BossStateMissileAttack::Update(Boss* pBoss) {
	//グローバル変数の適応
	BossStateMissileAttack::ApplyGlobalVariables();
	//移動処理
	if (pBoss->GetMoveDirection() == 1) {
		moveSpeed_ = moveSpeed_;
	}
	else {
		moveSpeed_ = -moveSpeed_;
	}

	////演出用のミサイルを生成
	//if (--missileSpornTimer_ < 0 && missileCount_ < kMissileMax) {
	//	missileSpornTimer_ = kMissileSpornTime;
	//	missileCount_++;
	//	Vector3 missilePos = pBoss->GetWorldPosition();
	//	Missile* missile[2];
	//	missile[0] = new Missile();
	//	missile[0]->Initialize(missilePos, Vector3{ 0.1f,0.0f,0.0f }, pBoss->GetMissileSoundHandle());
	//	missiles_.push_back(std::unique_ptr<Missile>(missile[0]));
	//	missile[1] = new Missile();
	//	missile[1]->Initialize(missilePos, Vector3{ -0.1f,0.0f,0.0f }, pBoss->GetMissileSoundHandle());
	//	missiles_.push_back(std::unique_ptr<Missile>(missile[1]));
	//}

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

	////演出用のミサイルがすべて画面外に出たらミサイルを追加する
	//if (missileCount_ > 0 && missiles_.size() == 0) {
	//	isMove_ = true;
	//	if (--addMissileTimer_ < 0 && addMissileCount_ < kMissileMax) {
	//		addMissileTimer_ = kMissileSpornTime;
	//		addMissileCount_++;
	//		Missile* missile[2];
	//		missile[0] = new Missile();
	//		missile[0]->Initialize(Vector3{ -6.5f,pBoss->Random(-2.2f, 1.0f) ,0.0f }, Vector3{ 0.05f,0.0f,0.0f }, pBoss->GetMissileSoundHandle());
	//		pBoss->AddMissile(missile[0]);
	//		missile[1] = new Missile();
	//		missile[1]->Initialize(Vector3{ 6.5f,pBoss->Random(-2.2f, 1.0f) ,0.0f }, Vector3{ -0.05f,0.0f,0.0f }, pBoss->GetMissileSoundHandle());
	//		pBoss->AddMissile(missile[1]);
	//	}
	//}

	if (isMove_) {
		//移動処理
		worldTransform_.translation_ = Add(worldTransform_.translation_, Vector3{ moveSpeed_,0.0f,0.0f });

		//画面端まで移動したら移動方向を変える
		if (worldTransform_.translation_.x <= Missile::widthMin || worldTransform_.translation_.x >= Missile::widthMax) {
			int direction = pBoss->GetMoveDirection();
			pBoss->SetMoveDirection(direction *= -1);
			moveSpeed_ *= pBoss->GetMoveDirection();
		}

		//本体にワールドトランスフォームを設定
		pBoss->SetWorldTransform(worldTransform_);
	}

	//ミサイルをすべて追加し終わったら通常状態に戻る
	if (addMissileCount_ == kMissileMax && missiles_.size() == 0) {
		pBoss->ChangeState(new BossStateNormal());
	}
}

void BossStateMissileAttack::Draw(Boss* pBoss, const ViewProjection& viewProjection) {
	//ミサイルの描画
	for (std::unique_ptr<Missile>& missile : missiles_) {
		missile->Draw(viewProjection);
	}
}

void BossStateMissileAttack::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "BossStateNormal";
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "MoveSpeed");
}