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
#include "DeathParticles.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数（型）
	Sprite* sprite_ = nullptr;
	bool isDebugCameraActive_ = false;

	//ゲームのフェーズ
	enum class Phase {
		kPlay,    //ゲームプレイ
		kDeath,   //デス演出
	};

	//ゲームの現在フェーズ（変数）
	Phase phase_;

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

	//全ての当たり判定を行う
	void CheckAllCollisions();

	void ChangePhase();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private: // メンバ変数
	// 終了フラグ
	bool finished_ = false;
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
	//パーティクル
	DeathParticles* deathParticles_ = nullptr;
	Model* modelDeathParticle_ = nullptr;

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

	std::list<Enemy*> enemies_;
	std::list<DeathParticles*> deathPartiy_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};