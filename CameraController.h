#pragma once
#include "ViewProjection.h"
#include "Player.h"

class Player;

struct Rect {
	float left = 0.0f;   //左端
	float right = 1.0f;  //右端
	float bottom = 0.0f; //下端
	float top = 1.0f;    //上端
};

class CameraController {
public:
	void Initialize();
	void Updata();
	void SetTarget(Player* target) { target_ = target; }
	void Reset();
	void SetMovableArea(Rect area) { movableArea_ = area; }//セッター完成
	// 追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0, 0, -15.0f};
	const ViewProjection& GetViewProjection() const { return viewProjection_; }
	//カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};
	//カメラの目標座標
	Vector3 targetPos_;

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;
	//座標補間割合
	static inline const float kInterpolationRate = 0.03f;
	//速度掛け率
	const float kVelocityBias = 10.0f;
	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect margen = {-320, 320, -200, 200};
};

