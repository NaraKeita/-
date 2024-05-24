#include "GameScene.h"
#include "Model.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>

GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {
	delete sprite_;
	delete player_;
	delete model_;
	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;

	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("playar.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	//自キャラの生成
	player_ = new Player();

	//スカイドームの生成
	skydome_ = new Skydome();

//	model_ = Model::Create();

	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_, &viewProjection_);

	//スカイドームの初期化
	skydome_->Initialize(model_,&viewProjection_);

	// 3Dモデルの生成(player)
	modelBlock_ = Model::CreateFromOBJ("cube");

	//3Dモデルの生成(skydome)
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	const float kBlockWodth = 2.0f;
	const float kBlockHeight = 2.0f;

	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if ((i + j) % 2 == 0)

				continue;
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWodth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}

	debugCamera_ = new DebugCamera(kNumBlockHorizontal, kNumBlockVirtical);
}

void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);

	//自キャラの更新
	player_->Update();
	skydome_->Update();
	
	debugCamera_->Update();

	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			if (!worldTransformBlock)

				continue;

			 // 平行移動
			Matrix4x4 result{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 
				worldTransformBlock->translation_.x,
				worldTransformBlock->translation_.y,
				worldTransformBlock->translation_.z,
				1.0f
			};
			

			worldTransformBlock->matWorld_ = result;

			worldTransformBlock->TransferMatrix();
		}
	}

    #ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
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

	// sprite_->Draw();
	//自キャラの描画
	player_->Draw();
	//スカイドームの描画
	skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock,viewProjection_);
		}
	}
	
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
