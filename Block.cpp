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
	//model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}
