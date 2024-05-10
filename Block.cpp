#include "Block.h"
#include <cassert>

void Block::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);
	viewProjection_ = viewProjection;
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
}

void Block::Update() {
	worldTransform_.TransferMatrix();
}

void Block::Draw() { 
<<<<<<< HEAD
	//model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
=======
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02
}
