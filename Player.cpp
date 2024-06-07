#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, /* uint32_t textureHandle,*/ ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	worldTransform_.Initialize();
	model_ = model;
	worldTransform_.translation_ = position;
	//textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
}

void Player::Update() { 
	//worldTransform_.TransferMatrix(); 
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_/*, textureHandle_*/); 
}