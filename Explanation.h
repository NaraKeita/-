#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DirectXCommon.h"
#include"Skydome.h"
#include "Input.h"

class Explanation {
public:
	Explanation();
	~Explanation();
	void Initialize();
	void Update();
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:
	// 終了フラグ
	bool finished_ = false;
	Model* modelFont_ = nullptr;

	// 3Dモデル
	Model* modelSkydome_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	WorldTransform worldTransformFont_;
	// WorldTransform worldTransformFontSpace_;
	WorldTransform worldTransformPlayer_;

	DirectXCommon* dxCommon_ = nullptr;

	float timer_ = 0.0f;
	static inline const float kMotionTime = 1.0f;
};
