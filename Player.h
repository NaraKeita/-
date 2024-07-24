#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "ViewProjection.h"

#pragma once

class MapChipField;

class Player {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	// アップデート
	void Update();
	// 描画
	void Draw();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	
	/*void HitCeiling(const CollisionMapInfo& collisionMapInfo);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);*/

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	
	Vector3 velocity_ = {};

	//加速
	static inline const float kAcceleration = 0.01f;
	//減速
	static inline const float kAttenuation = 0.02f;
	//最大速度
	static inline const float kLimitRunSpeed = 1.0f;

	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	//旋回時間(秒)
	static inline const float kTimeTurn = 0.3f;

	//重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.1f;//ジャンプの高さが変わる
	//最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 1.0f;//ジャンプし終わって落ちるときの速度が変わる
	//ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 1.0f;//ジャンプの高さが変わる
	// 接地状態フラグ
	bool onGround_ = true;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//キャラの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// ①移動入力
	void InputMove();

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool ceiling = false; // 天井衝突フラグ
		bool landing = false; // 着地フラグ
		bool hitWall = false; // 壁接触フラグ
		Vector3 move;         // 移動量
	};

	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数
	};

	// マップ衝突判定
	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);

	// ③判定結果を反映して移動させる
	void CheckMapMove(CollisionMapInfo& info);
	// ④天井に接触している場合の処理
	void CheckMapCeiling(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	static inline const float kBlank = 0.04f;
	// ⑦旋回制御
	void AnimateTurn();
};


