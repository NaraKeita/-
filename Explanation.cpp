#include "Explanation.h"
#include <numbers>

Explanation::Explanation() {
}

Explanation::~Explanation() {
	delete modelFont_;
}

// 初期化
void Explanation::Initialize() {
	modelFont_ = Model::CreateFromOBJ("explanation");
	
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	worldTransformFont_.Initialize();
	worldTransformPlayer_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();

	worldTransformFont_.scale_ = {2, 2, 2};    // 文字の大きさ
	
}

// 更新
void Explanation::Update() {
	finished_ = false;
	if (Input::GetInstance()->PushKey(DIK_S)) {
		finished_ = true;
	}

	// 行列を更新
	worldTransformFont_.UpdateMatrix();
	worldTransformPlayer_.UpdateMatrix();
}

// 描画
void Explanation::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	modelFont_->Draw(worldTransformFont_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
}
