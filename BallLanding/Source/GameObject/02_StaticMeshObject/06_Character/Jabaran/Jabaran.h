#pragma once
#include "GameObject//02_StaticMeshObject//06_Character//Character.h"

class StaticMeshManager;

class Jabaran
	: public Character
{
public:
	Jabaran();
	~Jabaran() override;
	//動作関数.
	void Update() override;
	//描画関数.
	void Draw() override;
	//初期化関数.
	void Init() override;
	//死亡しているかのGet関数.
	bool GetDead() const override;


private:

};
