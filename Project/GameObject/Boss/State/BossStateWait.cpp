#include "BossStateWait.h"
#include "../GameObject/Boss/Boss.h"
#include "BossStateNormal.h"

BossStateWait::~BossStateWait() {

}

void BossStateWait::Initialize(Boss* pBoss) {
	//ワールドトランスフォームを取得
	worldTransform_ = pBoss->GetWorldTransform();
	//ボスを後ろに向かせる
	worldTransform_.rotation_.y = 3.14f;
}

void BossStateWait::Update(Boss* pBoss) {
	if (pBoss->GetIsActive()) {
		worldTransform_.rotation_.y -= rotSpeed_;
	}

	//ワールドトランスフォームを設定
	pBoss->SetWorldTransform(worldTransform_);

	if (worldTransform_.rotation_.y <= 0.0f) {
		pBoss->ChangeState(new BossStateNormal());
	}
}

void BossStateWait::Draw(Boss* pBoss, const ViewProjection& viewProjection) {

}

void BossStateWait::ApplyGlobalVariables() {

}