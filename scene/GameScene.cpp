#include "GameScene.h"
#include "Model.h"
#include "Block.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>

// デストラクタ
GameScene::GameScene() { delete model_;
<<<<<<< HEAD
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			delete worldTransformBlock;
		}
=======
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02
	}
	worldTransformBlocks_.clear();
}

<<<<<<< HEAD
GameScene::~GameScene() {
	delete block_;
	delete debugCamera_;
	worldTransformBlocks_.clear();
}
=======
GameScene::~GameScene() { delete block_; }
>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	//
	model_ = Model::Create();
	//
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 自キャラの生成
	block_ = new Block();
	// 自キャラの初期化
	block_->Initialize(model_, textureHandle_, &viewProjection_);
<<<<<<< HEAD

	modelBlock_ = Model::CreateFromOBJ("cube");

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//要素数
	const uint32_t kNumBlockVirtical = 10; 
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック一個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHight = 2.0f;
	//要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	//キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if ((i + j) % 2 == 0)
				continue;
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHight * i;
		}
	}
=======
	//要素数
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック一個分の横幅
	const float kBlockWidth = 2.0f;
	//要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	//キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; i++) {
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}

>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02
}

void GameScene::Update() {
	// 自キャラの更新
	block_->Update();
<<<<<<< HEAD
	//デバッグカメラの更新
	debugCamera_->Update();
	#ifdef _DEBUG
	if (input_->TriggerKey(Keys[Akey])) {
		isDebugCameraActive_ = true;
	}
	#endif
	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			/* worldTransformBlocks_[0] = 1;
=======
	//ブロックの更新
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		/*worldTransformBlocks_[0] = 1;
>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02
		worldTransformBlocks_[1] = 2;
		worldTransformBlocks_[2] = 3;
		worldTransformBlocks_[3] = 4;*/

<<<<<<< HEAD
		// worldTransformBlock->UpdateMatrix();

		// 平行移動
=======
		//平行移動
>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02
		Matrix4x4 result{
		    1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			worldTransformBlock->translation_.x,
			worldTransformBlock->translation_.y,
			worldTransformBlock->translation_.z,
<<<<<<< HEAD
		    1.0f};
		// 平行移動だけ代入
		worldTransformBlock->matWorld_ = result;
		// 定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	    }
    }
=======
		    1.0f
		};
		//平行移動だけ代入
		worldTransformBlock->matWorld_ = result;
		//定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	}
>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

<<<<<<< HEAD
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
		if (!worldTransformBlock)
			continue;
		model_->Draw(*worldTransformBlock, debugCamera_->GetViewProjection());
	    }
    }

=======
>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	block_->Draw();

	//ブロックの描画
<<<<<<< HEAD
	/*for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}*/
	
	
=======
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		model_->Draw(*worldTransformBlock, viewProjection_);
	}
	worldTransformBlocks_.clear();
>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
