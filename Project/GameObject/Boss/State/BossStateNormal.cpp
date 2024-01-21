#include "BossStateNormal.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateLaserAttack.h"
#include "BossStateMissileAttack.h"
#include "BossStateChargeShot.h"
#include "BossStateStun.h"
#include "Utility/GlobalVariables.h"

//実態定義
int BossStateNormal::AttackInterval = 360;

BossStateNormal::~BossStateNormal() {

}

void BossStateNormal::Initialize(Boss* pBoss) {
	//ワールドトランスフォームの初期化
	worldTransform_ = pBoss->GetWorldTransform();
	//攻撃間隔の初期化
	nextAttackTimer_ = AttackInterval;
	//速度の初期化
	if (pBoss->GetMoveDirection() == 1) {
		moveSpeed_ = moveSpeed_;
	}
	else {
		moveSpeed_ = -moveSpeed_;
	}
	lazerAttackTimer_ = AttackInterval;
	chargeShotTimer_ = AttackInterval;

	//グローバル変数
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "BossStateNormal";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "MoveSpeed", moveSpeed_);
	globalVariables->AddItem(groupName, "AttackInterval", AttackInterval);
}

void BossStateNormal::Update(Boss* pBoss) {

	//グローバル変数の適応
	BossStateNormal::ApplyGlobalVariables();

	//移動処理
	if (pBoss->GetMoveDirection() == 1) {
		moveSpeed_ = moveSpeed_;
	}
	else {
		moveSpeed_ = -moveSpeed_;
	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, Vector3{ moveSpeed_,0.0f,0.0f });

	//画面端まで移動したら移動方向を変える
	if (worldTransform_.translation_.x < Missile::widthMin || worldTransform_.translation_.x > Missile::widthMax) {
		int direction = pBoss->GetMoveDirection();
		pBoss->SetMoveDirection(direction *= -1);
	}

	//ワールドトランスフォームを設定
	pBoss->SetWorldTransform(worldTransform_);
    
	//チャージショット状態に変更
	if (--nextAttackTimer_ < 0)
	{
		//次の行動をランダムに決める
		if (isAttack_ == false) {
			isAttack_ = true;
			//ボスの体力が半分以下だったらチャージショットを攻撃パターンに加える
			if (pBoss->GetHP() > pBoss->kHpMax / 2) {
				nextAttack_ = pBoss->Random(0, 1);
			}
			else {
				nextAttack_ = pBoss->Random(0, 2);
			}
		}

		switch (nextAttack_) {
		case LaserAttack:
			pBoss->ChangeState(new BossStateLaserAttack());
			break;
	/*	case MissileAttack:
			pBoss->ChangeState(new BossStateMissileAttack());
			break;*/
		case ChargeShot:
			if (worldTransform_.translation_.x > 10000.1f && worldTransform_.translation_.x > -10000.1f)
			{
				worldTransform_.translation_.x = 0.0f;
				pBoss->SetWorldTransform(worldTransform_);
				pBoss->ChangeState(new BossStateChargeShot());
			}
			break;
		}
	}
}

void BossStateNormal::Draw(Boss* pBoss, const ViewProjection& viewProjection) {

}

void BossStateNormal::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "BossStateNormal";
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "MoveSpeed");
	AttackInterval = globalVariables->GetIntValue(groupName, "AttackInterval");
}