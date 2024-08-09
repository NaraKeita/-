#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Player.h"
#include "Enemy.h"
#include"Skydome.h"
#include<vector>
#include"DebugCamera.h"
#include "GameScene.h"
#include "MapChipField.h"
#include "CameraController.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	Sprite* sprite_ = nullptr;
	bool isDebugCameraActive_ = false;

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void GenerateBlocks();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0;
	//自キャラ
	Player* player_ = nullptr;
	//雑魚敵
	Enemy* enemy_ = nullptr;
	//スカイドームキャラ
	Skydome* skydome_ = nullptr;
	//ブロック
	Model* modelBlock_ = nullptr;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	//プレイヤーのモデル
	Model* model_ = nullptr;
	//雑魚敵のモデル
	Model* modelEnemy_ = nullptr;

	ViewProjection viewProjection_;

	DebugCamera* debugCamera_ = nullptr;

	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_;

	CameraController* cameraController_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};