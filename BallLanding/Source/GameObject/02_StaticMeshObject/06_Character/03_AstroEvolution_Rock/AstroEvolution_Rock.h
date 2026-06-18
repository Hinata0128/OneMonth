#pragma once
#include "System/00_Manager/01_StaticMeshManager/StaticMeshManager.h"
#include "Collision/BoundingSphere/BoundingSphere.h"

#include "..//Character.h"

class AstroEvolution_Rock final
	: public Character
{
public:
	enum class MovePattern {
		UpDown,   // ã‰º
		LeftRight, // ¶‰E
		Wait,      // ‘Ò‹@
		Max        // ƒ‰ƒ“ƒ_ƒ€”ÍˆÍ—p
	};
public:
	AstroEvolution_Rock();
	~AstroEvolution_Rock() override;

	void Update() override;
	void Draw() override;

	void Init() override;

	bool GetDead() const override;

	std::shared_ptr<BoundingSphere> GetBoundingSphere()
	{
		return m_pSphere; 
	}	
	//“–‚½‚Á‚Ä‚¢‚é‚©‚ÌŠÖ”.
	void OnHit(int damage);
	//“G‚ª€‚ñ‚Å‚¢‚é‚©.
	bool IsDead() const;

	// ‰æ–Ê“à‚É‰f‚Á‚Ä‚¢‚é‚©”»’è‚·‚éŠÖ”
	bool IsVisibleOnScreen() const;


private:
	float m_AngleY;
	float m_MoveSpeed;
	float m_RotSpeed;

	std::shared_ptr<BoundingSphere> m_pSphere;

	MovePattern m_Pattern;
	float m_Timer;

	int m_HP;        // Œ»İ‚ÌHP (100)
	int m_MaxHP;     // Å‘åHP (100)
	int m_Phase;     // Œ»İ‚ÌŒ©‚½–Ú’iŠK (0:–³, 1:”¼‰ó, 2:‘å”j)
};
