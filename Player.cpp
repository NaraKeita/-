#define NOMINMAX
#include "Player.h"
#include <cassert>
#include <numbers>
#include "Input.h"
#include <algorithm>
#include "MapChipField.h"
#include "DebugText.h"

//初期化
void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;
	//textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
}

void Player::Update() {

	// ①移動入力
	InputMove();

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	// ②マップ衝突チェック
	// 衝突情報を初期化
	//CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	// ②マップ衝突チェック
	CheckMapCollision(collisionMapInfo);
	// ③移動
	CheckMapMove(collisionMapInfo);
	// ④天井に接触している場合の処理
	CheckMapCeiling(collisionMapInfo);
	//// ⑤壁に接触している場合の処理
	//CheckMapWall(collisionMapInfo);
	//// ⑥接地状態の切り替え処理
	//CheckMapLanding(collisionMapInfo);

	
	bool landing = false;

	// 地面との当たり判定
	// 下降中?
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	// 接地状態
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (collisionMapInfo.landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAcceleration);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}
	// ⑦旋回行列
	AnimateTurn();
	// 行列を更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_);
}

// ①移動入力
void Player::InputMove() {
	//左右移動操作
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {
			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_D)) {
				acceleration.x += kAcceleration;
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				// 左右加速
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_A)) {
				acceleration.x -= kAcceleration;
				// 右移動中の左移動
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				// 左右加速
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}

			// 　加速/減速
			velocity_ += acceleration;
			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_W)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
	}
	// 空中
	else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度軽減
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}
//②マップ衝突判定
void Player::CheckMapCollision(CollisionMapInfo& info) { // マップ衝突判定
	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);  // マップ衝突判定下方向
	// CheckMapCollisionRight(info); // マップ衝突判定右方向
	// CheckMapCollisionLeft(info);  // マップ衝突判定左方向
}
//マップ衝突判定　上
void Player::CheckMapCollisionUp(CollisionMapInfo& info) { // マップ衝突判定上方向
	// 上昇あり？
	if (info.move.y <= 0) {
		return;
	}
	// 移動後の４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, info.move.y);
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}

// マップ衝突判定　下
void Player::CheckMapCollisionDown(CollisionMapInfo& info) { // マップ衝突判定下方向
	// 上昇あり？
	if (info.move.y >= 0) {
		return;
	}
	// 移動後の４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	// 着地フラグ
	//if (info.landing) {
	//	// 着地状態に切り替える（落下を止める）
	//	onGround_ = true;
	//	// 着地時にX速度を減衰
	//	velocity_.x *= (1.0f - kAttenuationLanding);
	//	// Y速度を０にする
	//	velocity_.y = 0.0f;
	//}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool hit = false;

	////ジャンプ開始
	//if (velocity_.y > 0.0f) {
	//	onGround_ = false;
	//} else {
	//	// 左下点の判定
	//	kLeftBottom + Vector3(0, -0.1f, 0);
	//	// 右下点の判定
	//	kRightBottom + Vector3(0, -0.1f, 0);
	//	// 落下開始
	//	if (!hit) {
	//		// 空中状態に切り替える
	//		onGround_ = false;
	//	}
	//}

	
	
	
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}


	
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		/*info.move.y = std::max(0.0f, info.move.y);*/
		info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.landing = true;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

//③判定結果を反映して移動させる
void Player::CheckMapMove(CollisionMapInfo& info) {
	//移動
	worldTransform_.translation_ += info.move;
}

//④天井に接触している場合の処理
void Player::CheckMapCeiling(CollisionMapInfo& info) {
	  //天井に当たった？
	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

//// ⑤壁に接触している場合の処理
//void Player::CheckMapWall(CollisionMapInfo& info) {
//
//}

//// ⑥接地状態の切り替え処理
//void Player::CheckMapLanding(CollisionMapInfo& info) { CollisionMapInfo dm = info; }

//⑦旋回制御
void Player::AnimateTurn() {

}
