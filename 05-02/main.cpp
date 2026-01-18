#include <Novice.h>
#include <list>

class IStageSceneCommand {
public:
	virtual ~IStageSceneCommand() = default;
	virtual void Exec() = 0;
	virtual void Undo() = 0;
};

class Selector;

class Unit {
	int mapX_ = 0;
	int mapY_ = 0;
	int mapSize_ = 32;
	int id = 0;
	bool isSelected_ = false;

public:
	Unit() = default;
	void Init(int idref);
	void Update();
	void Draw();

	void Move(int x, int y);
	void MoveEnd(Selector* selector);

	int GetX() const { return mapX_; }
	int GetY() const { return mapY_; }
	void SetSelected(bool s) { isSelected_ = s; }
	bool IsSelected() const { return isSelected_; }
};

class Selector {
private:
	int mapX_;
	int mapY_;
	int mapSize_;

	int selectMode_;

	Unit* unit_;
	Unit* selectedUnit_;
public:
	Selector();

	void Inti();
	void Update();
	void Draw();

	void Move(int x, int y);
	void SelectUnit();

	// ゲッターとセッター
	void SetSelectedUnit(Unit* unit) { selectedUnit_ = unit; }
	Unit* GetSelectedUnit() const { return selectedUnit_; }

	void SetTargetUnit(Unit* unit) { unit_ = unit; }
	Unit* GetTargetUnit() const { return unit_; }

	int GetX()const { return mapX_; }
	int GetY()const { return mapY_; }
};

class UnitMoveCommand
	:public IStageSceneCommand
{
public:
	UnitMoveCommand(Unit* unit, int x, int y);
	void Exec()override;
	void Undo()override;

private:
	Unit* unit_;
	int x_;
	int y_;
};

class UnitMoveEndCommand
	:public IStageSceneCommand
{
public:
	UnitMoveEndCommand(Unit* unit, Selector* selector);
	void Exec()override;

private:
	Unit* unit_;
	Selector* selector_;
};

class SelectorMoveCommand
	: public IStageSceneCommand
{
public:
	SelectorMoveCommand(Selector* selector, int x, int y);
	void Exec()override;
	void Undo()override;
private:
	Selector* selector_;
	int x_;
	int y_;
};

class SelectUnitCommand
	:public IStageSceneCommand
{
public:
	SelectUnitCommand(Selector* selector);
	void Exec()override;
	void Undo()override;
private:
	Selector* selector_;
};

class StageSceneInputHandler {
private:
	char keys_[255] = { 0 };
	char preKeys_[255] = { 0 };

	Selector* selector_;
	Unit* selectedUnit_;

public:
	StageSceneInputHandler();
	void UpdateKeyState();

	IStageSceneCommand* SelectorHandleInput(Selector* selector);
	// 変更: ユニット操作時は Selector を渡してスペースで解除できるようにする
	IStageSceneCommand* UnitHandleInput(Selector* selector);

	bool  CheckUndoStatus();
};

const char kWindowTitle[] = "LE2B_19_ハマダカズヤ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	StageSceneInputHandler* inputHandler = new StageSceneInputHandler();
	IStageSceneCommand* command = nullptr;
	std::list<IStageSceneCommand*> commandHistory;

	Selector* selector = new Selector();
	Unit* unit = new Unit();

	selector->SetTargetUnit(unit);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		inputHandler->UpdateKeyState();

		// セレクターがユニットを選んでいるならユニット操作、そうでなければセレクター操作
		if (selector->GetSelectedUnit() != nullptr) {
			command = inputHandler->UnitHandleInput(selector);
		}
		else {
			command = inputHandler->SelectorHandleInput(selector);
		}

		///
		/// ↓更新処理ここから
		///

		if (command) {
			// 「選択中のときだけ履歴に残す」方針
			bool shouldRecord = false;

			// SelectUnitCommand の場合は「これから選択されるなら」履歴に残す
			SelectUnitCommand* selectCmd = dynamic_cast<SelectUnitCommand*>(command);
			if (selectCmd) {
				// 現在未選択で、ターゲットユニットが存在かつセレクター位置が一致すれば
				if (selector->GetSelectedUnit() == nullptr) {
					Unit* target = selector->GetTargetUnit();
					if (target != nullptr && target->GetX() == selector->GetX() && target->GetY() == selector->GetY()) {
						shouldRecord = true; // 選択アクション（選択状態に遷移）を記録
					}
				}
				// 既に選択中で「解除」する場合は記録しない（要望通り: 選択中のみ記録）
			}
			else {
				// それ以外（ユニット移動）は「現在選択中かどうか」で判定
				if (selector->GetSelectedUnit() != nullptr) {
					shouldRecord = true;
				}
			}

			// 実行
			command->Exec();

			// 履歴に残すか破棄するか
			if (shouldRecord) {
				commandHistory.push_back(command);
			}
			else {
				delete command;
			}
		}

		if (inputHandler->CheckUndoStatus()) {
			if (!commandHistory.empty()) {
				// 履歴の最後（最新の操作）を取得してUndo
				IStageSceneCommand* lastCommand = commandHistory.back();
				lastCommand->Undo();
				commandHistory.pop_back();
				delete lastCommand;
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		unit->Draw();
		selector->Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void Unit::Init(int idref) {
	idref;
}

void Unit::Update() {

}

void Unit::Draw() {
	// 選択中は色を変えて描画
	if (isSelected_) {
		Novice::DrawBox(mapX_, mapY_, mapSize_, mapSize_, 0.0f, BLUE, kFillModeSolid);
	}
	else {
		Novice::DrawBox(mapX_, mapY_, mapSize_, mapSize_, 0.0f, WHITE, kFillModeSolid);
	}
}

void Unit::Move(int x, int y) {
	mapX_ += x;
	mapY_ += y;
}

void Unit::MoveEnd(Selector* selector) {
	selector;
}

Selector::Selector() {
	mapX_ = 0;
	mapY_ = 0;
	mapSize_ = 32;
	unit_ = nullptr;
	selectedUnit_ = nullptr;
}

void Selector::Inti() {

}

void Selector::Update() {

}

void Selector::Draw() {
	Novice::DrawBox(mapX_, mapY_, mapSize_, mapSize_, 0.0f, RED, kFillModeWireFrame);
}

void Selector::Move(int x, int y) {
	mapX_ += x;
	mapY_ += y;
}

void Selector::SelectUnit() {
	// セレクター位置とターゲットユニットが一致する場合に選択/解除を切り替える
	if (selectedUnit_ == nullptr) {
		if (unit_ != nullptr && unit_->GetX() == mapX_ && unit_->GetY() == mapY_) {
			selectedUnit_ = unit_;
			selectedUnit_->SetSelected(true);
		}
	}
	else {
		// 既に選択されている場合は解除
		selectedUnit_->SetSelected(false);
		selectedUnit_ = nullptr;
	}
}

UnitMoveCommand::UnitMoveCommand(Unit* unit, int x, int y)
	:unit_(unit), x_(x), y_(y) {
}

void UnitMoveCommand::Exec() {
	if (unit_) {
		unit_->Move(x_, y_);
	}
}

void UnitMoveCommand::Undo() {
	if (unit_) {
		unit_->Move(-x_, -y_);
	}
}

UnitMoveEndCommand::UnitMoveEndCommand(Unit* unit, Selector* selector)
	: unit_(unit), selector_(selector) {
}

void UnitMoveEndCommand::Exec() {
	if (selector_ && unit_) {
		selector_->SetSelectedUnit(nullptr);
		unit_->SetSelected(false);
	}
}

SelectorMoveCommand::SelectorMoveCommand(Selector* selector, int x, int y)
	: selector_(selector), x_(x), y_(y) {
}

void SelectorMoveCommand::Exec() {
	if (selector_) {
		selector_->Move(x_, y_);
	}
}

void SelectorMoveCommand::Undo() {
	if (selector_) {
		selector_->Move(-x_, -y_);
	}
}

SelectUnitCommand::SelectUnitCommand(Selector* selector)
	: selector_(selector) {
}

void SelectUnitCommand::Exec() {
	if (selector_) {
		selector_->SelectUnit();
	}
}

void SelectUnitCommand::Undo() {
	// Undoは選択をトグルする形で実装（選択状態を元に戻す）
	if (selector_) {
		selector_->SelectUnit();
	}
}

StageSceneInputHandler::StageSceneInputHandler() {

}

void StageSceneInputHandler::UpdateKeyState() {
	memcpy(preKeys_, keys_, 256);
	Novice::GetHitKeyStateAll(keys_);
}

IStageSceneCommand* StageSceneInputHandler::SelectorHandleInput(Selector* selector) {
	if ((keys_[DIK_UP] && !preKeys_[DIK_UP]) || (keys_[DIK_W] && !preKeys_[DIK_W])) {
		return new SelectorMoveCommand(selector, 0, -32);
	}
	if ((keys_[DIK_DOWN] && !preKeys_[DIK_DOWN]) || (keys_[DIK_S] && !preKeys_[DIK_S])) {
		return new SelectorMoveCommand(selector, 0, 32);
	}
	if ((keys_[DIK_LEFT] && !preKeys_[DIK_LEFT]) || (keys_[DIK_A] && !preKeys_[DIK_A])) {
		return new SelectorMoveCommand(selector, -32, 0);
	}
	if ((keys_[DIK_RIGHT] && !preKeys_[DIK_RIGHT]) || (keys_[DIK_D] && !preKeys_[DIK_D])) {
		return new SelectorMoveCommand(selector, 32, 0);
	}

	// スペースキーで選択/解除
	if ((keys_[DIK_SPACE] && !preKeys_[DIK_SPACE])) {
		return new SelectUnitCommand(selector);
	}

	return nullptr;
}

IStageSceneCommand* StageSceneInputHandler::UnitHandleInput(Selector* selector) {
	Unit* unit = selector ? selector->GetSelectedUnit() : nullptr;
	if (unit == nullptr) {
		return nullptr;
	}

	if ((keys_[DIK_UP] && !preKeys_[DIK_UP]) || (keys_[DIK_W] && !preKeys_[DIK_W])) {
		return new UnitMoveCommand(unit, 0, -32);
	}
	if ((keys_[DIK_DOWN] && !preKeys_[DIK_DOWN]) || (keys_[DIK_S] && !preKeys_[DIK_S])) {
		return new UnitMoveCommand(unit, 0, 32);
	}
	if ((keys_[DIK_LEFT] && !preKeys_[DIK_LEFT]) || (keys_[DIK_A] && !preKeys_[DIK_A])) {
		return new UnitMoveCommand(unit, -32, 0);
	}
	if ((keys_[DIK_RIGHT] && !preKeys_[DIK_RIGHT]) || (keys_[DIK_D] && !preKeys_[DIK_D])) {
		return new UnitMoveCommand(unit, 32, 0);
	}

	// 選択解除（スペース）でユニット操作終了（セレクターのSelectUnitでトグル）
	if ((keys_[DIK_SPACE] && !preKeys_[DIK_SPACE])) {
		return new SelectUnitCommand(selector);
	}

	return nullptr;
}

bool StageSceneInputHandler::CheckUndoStatus() {
	return keys_[DIK_Z] && !preKeys_[DIK_Z];
}