#pragma once
#include "..//ManagerBase.h"
#include <string>

/********************************************************
*	敵の情報をまとめて管理するマネージャークラス.
**/

class Jabaran;
class ImGuiManager;

class JabaranManager final
	: public ManagerBase<JabaranManager>
{
public:
	//コンストラクタ.
	JabaranManager();
	//デストラクタ.
	~JabaranManager() override;
	//構築関数.
	void Create() override;
	//動作関数.
	void Update() override;
	//描画関数.
	void Draw() override;
	//解放関数.
	void Release() override;
	//初期化関数.
	void Init();

	//JSONファイルから敵を読み込んで配置する関数.
	void LoadJabaranFromSpownPosName(const std::string& FilePath);
private:
	//生成した敵(Jabaran)を管理する配列.
	std::vector<std::unique_ptr<Jabaran>> m_upJabaranList;
};