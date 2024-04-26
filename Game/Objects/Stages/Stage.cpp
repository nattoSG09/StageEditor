#include "Stage.h"

#include "StageObject.h"
#include "Components/TestMove.h"
#include "Components/InputMove.h"
#include "Components/Gravity.h"

#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/Json/JsonReader.h"
#include "../../../Engine/DirectX/Input.h"

namespace {
	// 文字列内の全ての "\\" を "/" に置換する関数
	void ReplaceBackslashes(string& str) {
		size_t found = str.find("\\");
		while (found != string::npos) {
			str.replace(found, 1, "/"); // "\\" を "/" に置換する
			found = str.find("\\", found + 1); // 次の "\\" を検索する
		}
	}

	// カレントディレクトリからの相対パスを取得する関数
	std::string GetAssetsRelativePath(const std::string& absolutePath) {
		const std::string assetsFolder = "Assets\\";
		// "Assets\\" の位置を検索
		size_t assetsPos = absolutePath.find(assetsFolder);
		if (assetsPos != std::string::npos) {
			// "Assets\\" の部分を除いた、それ以降の部分を取得
			std::string relativePath = absolutePath.substr(assetsPos + assetsFolder.size());
			return relativePath;
		}
		else {
			// "Assets\\" が見つからない場合は、元のファイルパスを返す
			return absolutePath;
		}
	}

}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),objects_{},selectedIndex_(-1)
{
}

void Stage::Initialize()
{
#ifndef _DEBUG
	this->Load("Data/TestStage.json");
#endif // !_DEBUG
}

void Stage::Update()
{
#ifdef _DEBUG
	// ワールドアウトライナーを表示
	DrawWorldOutliner();

	// 詳細ウィンドウを表示
	DrawDatails();
	
	//debugテキストウィンドウを表示
	DrawDebugTexts();

#endif // _DEBUG
}

void Stage::Draw()
{
}

void Stage::Release()
{
}

void Stage::DrawWorldOutliner()
{
	// ImGuiで表示するウィンドウの設定を行う
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, 0));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// ウィンドウを表示
	ImGui::Begin("World Outliner", NULL, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		// - ActionButtons ------------------------------------------ 
		ImGui::TextWrapped("StageObjects Edit");
		ImGui::SameLine();

		// 追加するボタンを表示
		if (ImGui::Button("Add")) {
			// 追加するオブジェクトの初期名を設定
			string name = "object" + std::to_string(objects_.size() + 1);

			//現在のカレントディレクトリを覚えておく
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// 追加するオブジェクトのモデルファイルパスを設定
			string filePath{}; {
				// 「ファイルを開く」ダイアログの設定用構造体を設定
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
					ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
					ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
					ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
					ofn.lpstrFile[0] = '\0'; // 初期化
					ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
					ofn.lpstrFilter = TEXT("FBXファイル(*.fbx)\0*.fbx\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
					ofn.nFilterIndex = 1; // 初期選択するフィルター
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
					ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
				}

				// ファイルを選択するダイアログの表示
				if (GetOpenFileName(&ofn) == TRUE) {
					// ファイルパスを取得
					filePath = ofn.lpstrFile;

					// カレントディレクトリからの相対パスを取得
					filePath = GetAssetsRelativePath(filePath);

					// 文字列内の"\\"を"/"に置換
					ReplaceBackslashes(filePath);

					// ディレクトリを戻す
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// 作成を中断
					ImGui::End();
					return;
				}
			}

			// オブジェクトを作成
			StageObject* obj = CreateStageObject(name, filePath, this);

			// オブジェクトのリストに追加
			this->Add(obj);
		}

		// 保存するボタンを表示
		ImGui::SameLine();
		if (ImGui::Button("Save")) {
			//現在のカレントディレクトリを覚えておく
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// 保存先のファイルパスを取得
			string filePath{}; {
				// 「ファイルを保存」ダイアログの設定用構造体を設定
				OPENFILENAME ofn; {
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0\0");
					char fileName[MAX_PATH] = "無題.json";
					ofn.lpstrFile = fileName;
					ofn.nMaxFile = MAX_PATH;
					ofn.Flags = OFN_OVERWRITEPROMPT;
					ofn.lpstrDefExt = "json";
					ofn.nFilterIndex = 1; // 初期選択するフィルター
					ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
				}
				
				// ファイルを保存するダイアログの表示
				if (GetSaveFileName(&ofn) == TRUE) {
					// ファイルパスを取得
					filePath = ofn.lpstrFile;

					// カレントディレクトリからの相対パスを取得
					filePath = GetAssetsRelativePath(filePath);

					// 文字列内の"\\"を"/"に置換
					ReplaceBackslashes(filePath);

					// ディレクトリを戻す
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// 保存を中断
					ImGui::End();
					return;
				}
			}

			// ファイルにステージ情報を保存
			this->Save(filePath);
		}

		// 読込するボタンを表示
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			//現在のカレントディレクトリを覚えておく
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// 読み込むファイルのパスを取得
			string filePath{}; {
				// 「ファイルを開く」ダイアログの設定用構造体を設定
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
					ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
					ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
					ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
					ofn.lpstrFile[0] = '\0'; // 初期化
					ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
					ofn.lpstrFilter = TEXT("JSONファイル(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
					ofn.nFilterIndex = 1; // 初期選択するフィルター
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
					ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
				}

				// ファイルを選択するダイアログの表示
				if (GetOpenFileName(&ofn) == TRUE) {
					// ファイルパスを取得
					filePath = ofn.lpstrFile;

					// カレントディレクトリからの相対パスを取得
					filePath = GetAssetsRelativePath(filePath);

					// 文字列内の"\\"を"/"に置換
					ReplaceBackslashes(filePath);

					// ディレクトリを戻す
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// 作成を中断
					ImGui::End();
					return;
				}
			}

			// ファイルを読み込みステージを生成
			this->Load(filePath);
		}
		
		// 削除するボタンを表示
		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			AllDelete();
		}

		// 区切りの為、ラインを表示
		ImGui::Separator();

		// - ObjectList ---------------------------------------------
		if (ImGui::TreeNodeEx("ObjectList", ImGuiTreeNodeFlags_DefaultOpen)) {
			// リストを表示
			for (int i = 0; i < objects_.size(); ++i) 
				if (ImGui::Selectable(objects_[i]->GetObjectName().c_str(), selectedIndex_ == i)) {
					selectedIndex_ = i;
				}
			ImGui::TreePop();
		}

		// ----------------------------------------------------------
	}
	ImGui::End();
}

void Stage::DrawDatails()
{
	// ImGuiで表示するウィンドウの設定を行う
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, Direct3D::screenHeight_ * 0.5f));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// ウィンドウを表示
	ImGui::Begin("Datails", NULL, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		// 選択されているオブジェクトの詳細を表示
		if (selectedIndex_ >= 0 && selectedIndex_ < objects_.size()) {

			// - name -------------------------------------------------------
			if (ImGui::TreeNode("name")) {
				ImGui::Text("Current name : %s", objects_[selectedIndex_]->objectName_.c_str());
				char buffer[256] = "";
				if (ImGui::InputTextWithHint("##Input", "Input New name...", buffer, IM_ARRAYSIZE(buffer)))
					objects_[selectedIndex_]->objectName_ = buffer;
				ImGui::TreePop();
			}

			// - transform --------------------------------------------------
			if (ImGui::TreeNode("Transform")) {
				if (ImGui::TreeNode("position_")) {
					ImGui::DragFloat("x", &objects_[selectedIndex_]->transform_.position_.x);
					ImGui::DragFloat("y", &objects_[selectedIndex_]->transform_.position_.y);
					ImGui::DragFloat("z", &objects_[selectedIndex_]->transform_.position_.z);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("rotate_")) {
					ImGui::DragFloat("x", &objects_[selectedIndex_]->transform_.rotate_.x);
					ImGui::DragFloat("y", &objects_[selectedIndex_]->transform_.rotate_.y);
					ImGui::DragFloat("z", &objects_[selectedIndex_]->transform_.rotate_.z);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("scale_")) {
					ImGui::DragFloat("x", &objects_[selectedIndex_]->transform_.scale_.x);
					ImGui::DragFloat("y", &objects_[selectedIndex_]->transform_.scale_.y);
					ImGui::DragFloat("z", &objects_[selectedIndex_]->transform_.scale_.z);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			// - component --------------------------------------------------
			if (ImGui::TreeNode("Component")) {
				static bool showComponentWindow = false;

				if (ImGui::Button("AddComponent")) {
					showComponentWindow = true;
				}

				// 追加できるコンポーネントリストのウィンドウ
				if (showComponentWindow) {
					ImGui::Begin("Component List", &showComponentWindow);

					// コンポーネントリストを表示し、選択されたコンポーネントを処理する
					for (int i = 0; i < COMP_MAX; i++) {
						if (ImGui::Selectable(ComponentTypeToString((ComponentType)i).c_str(), selectedComponentType_ == i)) {
							selectedComponentType_ = (ComponentType)i;

							switch (selectedComponentType_)
							{
							case COMP_TESTMOVE:objects_[selectedIndex_]->AddComponent(new TestMove(objects_[selectedIndex_])); break;
							case COMP_INPUTMOVE:objects_[selectedIndex_]->AddComponent(new InputMove(objects_[selectedIndex_])); break;
							case COMP_GRAVITY:objects_[selectedIndex_]->AddComponent(new Gravity(objects_[selectedIndex_]));break;
							case COMP_MAX:
								break;
							}

							showComponentWindow = false;
						}
					}

					ImGui::End();
				}


				if (ImGui::TreeNode("ComponentList")) {
					for (auto comp : objects_[selectedIndex_]->components_)
					{
						if (ImGui::TreeNode(comp->name_.c_str())) {

							comp->DrawDatails();

							if (ImGui::Button("delete")) {
								objects_[selectedIndex_]->DeleteComponent(comp);
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			// --------------------------------------------------------------
			ImGui::Separator();
			ImGui::Text("object delete"); ImGui::SameLine();
			if (ImGui::Button("delete"))Delete(objects_[selectedIndex_]);
		}
	}
	ImGui::End();
}

void Stage::DrawDebugTexts()
{
	// ImGuiで表示するウィンドウの設定を行う
	ImGui::SetNextWindowPos(ImVec2(0, Direct3D::screenHeight_ * 0.7f));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.7f, Direct3D::screenHeight_ * 0.3f));
	
	// ウィンドウを表示
	ImGui::Begin("Debug", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	ImGui::End();
}

void Stage::Add(StageObject* obj)
{
	objects_.push_back(obj);
}

bool Stage::Save(string filePath)
{
	// 保存データを作成
	json saveObj; {
		for (auto obj : objects_) {
			saveObj[obj->objectName_]["position_"]["x"] = obj->transform_.position_.x;
			saveObj[obj->objectName_]["position_"]["y"] = obj->transform_.position_.y;
			saveObj[obj->objectName_]["position_"]["z"] = obj->transform_.position_.z;

			saveObj[obj->objectName_]["rotate_"]["x"] = obj->transform_.rotate_.x;
			saveObj[obj->objectName_]["rotate_"]["y"] = obj->transform_.rotate_.y;
			saveObj[obj->objectName_]["rotate_"]["z"] = obj->transform_.rotate_.z;

			saveObj[obj->objectName_]["scale_"]["x"] = obj->transform_.scale_.x;
			saveObj[obj->objectName_]["scale_"]["y"] = obj->transform_.scale_.y;
			saveObj[obj->objectName_]["scale_"]["z"] = obj->transform_.scale_.z;

			saveObj[obj->objectName_]["modelFilePath_"] = obj->modelFilePath_;

			// コンポーネントの保存
			for (auto comp : obj->components_) {
				saveObj[obj->objectName_]["components"].push_back(comp->Save());
			}
		}
	}

	// 保存
	return JsonReader::Save(filePath, saveObj);
}

bool Stage::Load(string filePath)
{
	// 格納用オブジェクトを用意
	json loadObj;

	// ファイルを読み込む
	if (JsonReader::Load(filePath, loadObj) == false)return false;

	// 保存されていない現在のデータを削除
	AllDelete();

	// ロードしたデータからステージオブジェクトを復元する
	for (auto it = loadObj.begin(); it != loadObj.end(); ++it) {
		// オブジェクト名を復元
		string objectName = it.key();

		auto& data = it.value(); 
		// ファイルパスを復元
		string modelFilePath{};
		if (data.contains("modelFilePath_"))modelFilePath = data["modelFilePath_"];

		// 位置、回転、スケールを復元
		Transform transform;
		if (data.contains("position_")) {
			transform.position_.x = data["position_"]["x"];
			transform.position_.y = data["position_"]["y"];
			transform.position_.z = data["position_"]["z"];
		}
		if (data.contains("rotate_")) {
			transform.rotate_.x = data["rotate_"]["x"];
			transform.rotate_.y = data["rotate_"]["y"];
			transform.rotate_.z = data["rotate_"]["z"];
		}
		if (data.contains("scale_")) {
			transform.scale_.x = data["scale_"]["x"];
			transform.scale_.y = data["scale_"]["y"];
			transform.scale_.z = data["scale_"]["z"];
		}


		// 復元したデータを元にオブジェクトを作成
		StageObject* obj = CreateStageObject(objectName, modelFilePath, this);
		obj->SetTransform(transform);

		// コンポ―ネントのロード
		for (auto compData : data["components"]) {
			ComponentType type = static_cast<ComponentType>(compData["type"]);
			StageObjectComponent* comp = nullptr;

			switch (type) {
			case COMP_TESTMOVE:
				comp = new TestMove(obj);
				break;
			case COMP_INPUTMOVE:
				comp = new InputMove(obj);
				break;
			case COMP_GRAVITY:
				comp = new Gravity(obj);

			default:
				// 未知のコンポーネントタイプ
				break;
			}

			if (comp != nullptr) {
				obj->components_.push_back(comp);
			}
		}

		// オブジェクトをリストに追加
		this->Add(obj);
	}
	return this;
}

void Stage::Delete(StageObject* obj)
{
	// オブジェクトを削除する
	obj->KillMe();

	// オブジェクトのイテレータを取得する
	auto it = std::find(objects_.begin(), objects_.end(), obj);

	// イテレータが見つかった場合、ベクターから削除する
	if (it != objects_.end()) objects_.erase(it);
}

void Stage::AllDelete()
{
	for (auto obj : objects_) obj->KillMe();
	objects_.clear();
}
