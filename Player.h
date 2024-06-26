#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"

#pragma once

enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();
	Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;

	static inline const float kAcceleration = 0.04f;
	static inline const float kAttenuation = 0.04f;
	static inline const float kLimitRunSpeed = 0.2f;
	//旋回時間(秒)
	static inline const float kTimeTurn = 0.3f;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;

};


