#include "GameScene.h"
//#include "Model.h"
#include "TextureManager.h"
//#include "WorldTransform.h"
#include <cassert>
//#include "CameraController.h"

GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {
	delete sprite_;
	delete player_;
	delete model_;
	delete modelBlock_;
	delete debugCamera_;
	delete modelEnemy_;
	delete modelSkydome_;
	delete modelDeathParticle_;
	delete mapChipField_;

	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			delete worldTransformBlock;
		}
	}

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	
	for (DeathParticles* deathParticles : deathPartiy_) {
		delete deathParticles;
	}

	worldTransformBlocks_.clear();
}

//初期化
void GameScene::Initialize() {
	

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	//スカイドームの生成
	skydome_ = new Skydome();

	//プレイヤー
	model_ = Model::CreateFromOBJ("player",true);

	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// ビュープロジェクションの初期化
	viewProjection_.farZ;
	viewProjection_.Initialize();

	//スカイドームの初期化
	skydome_->Initialize(modelSkydome_,&viewProjection_);

	

	// 3Dモデルの生成(block)
	modelBlock_ = Model::CreateFromOBJ("block");

	//3Dモデルの生成(skydome)
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//3Dモデルの生成(Enemy)
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	//ブロックの表示される数字
	const uint32_t kNumBlockVirtical = 20;
	const uint32_t kNumBlockHorizontal = 100;

	/*const float kBlockWodth = 2.0f;
	const float kBlockHeight = 2.0f;*/

	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	debugCamera_ = new DebugCamera(kNumBlockHorizontal, kNumBlockVirtical);

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの生成と初期化
	player_->SetMapChipField(mapChipField_);
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	player_->Initialize(model_, &viewProjection_, playerPosition);

	player_->Update();
	Vector3 position = player_->GetWorldPosition();

	//雑魚敵の生成
	enemy_ = new Enemy();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(9, 18);
	enemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

	modelDeathParticle_ = Model::CreateFromOBJ("DeathParticle", true);
	// 仮の生成処理。後で消す
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, position);

	//カメラ
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();
		enemyPosition = mapChipField_->GetMapChipPositionByIndex(11 + i * 3, 18 - i);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);
		enemies_.push_back(newEnemy);
	}
}

//更新
void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);

	//自キャラの更新
	player_->Update();
	//Vector3 position = player_->GetWorldPosition();
	//雑魚敵の更新
	enemy_->Update();
	//スカイドームの更新
	skydome_->Update();
	//カメラコントローラーの更新
	cameraController_->Updata();
	//デバックカメラの更新
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

			worldTransformBlock->UpdateMatrix();
		}
	}

	//敵の更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	if (deathParticles_) {
		deathParticles_->Update();
	}

	//全ての当たり判定を行う
	CheckAllCollisions();

    #ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		cameraController_->Updata();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
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
	//雑魚敵の描画
	enemy_->Draw();
	//スカイドームの描画
	skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock,viewProjection_);
		}
	}

	//敵の更新
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (deathParticles_) {
		deathParticles_->Draw();
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

void GameScene::CheckAllCollisions() {
	#pragma region 
	{
		//判定対象１と２の座標
		AABB aabb1, aabb2;
		//自キャラの座標
		aabb1 = player_->GetAABB();
		//自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {
		    //敵弾の座標
			aabb2 = enemy->GetAABB();
			//AABB動詞の交差判定
			if (IsCollision(aabb1, aabb2)) {
			    //自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy_);
				//敵弾の衝突自コールバックを呼び出す
				enemy_->OnCollision(player_);
			}
		}
	}
	#pragma endregion
}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	//要素数を変更する
	//列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
	//1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	//ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
