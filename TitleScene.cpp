#include "TitleScene.h"
#include <numbers>

//コンストラクタ
TitleScene::TitleScene() {
}

//デストラクタ
TitleScene::~TitleScene() {
	delete modelFont_;
	delete modelPlayer_;
}

//初期化
void TitleScene::Initialize() { 
	modelFont_ = Model::CreateFromOBJ("titleFont"); 
	modelPlayer_ = Model::CreateFromOBJ("player");

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	worldTransformFont_.Initialize();
	worldTransformPlayer_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();

	worldTransformFont_.translation_.y = 10;//プレイヤーの位置.x
	worldTransformPlayer_.translation_.y = -8;//プレイヤーの位置.y
	worldTransformFont_.scale_ = {2, 2, 2};//文字の大きさ
	worldTransformPlayer_.scale_ = {10, 10, 10};//プレイヤーの大きさ
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;
}

//更新
void TitleScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	//タイマーを加算
	timer_ += 1.0f / 60.0f;

	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float> + std::sin(std::numbers::pi_v<float> * 2.0f * timer_ / kMotionTime);

	//行列を更新
	worldTransformFont_.UpdateMatrix();
	worldTransformPlayer_.UpdateMatrix();
}

//描画
void TitleScene::Draw() {
	//コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	//ここに3Dオブジェクトの描画処理を追加できる
	modelFont_->Draw(worldTransformFont_, viewProjection_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);

	//3Dオブジェクト描画後処理
	Model::PostDraw();
}
