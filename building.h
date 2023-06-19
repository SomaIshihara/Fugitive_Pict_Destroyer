//======================================================
//
//建物処理のヘッダ[building.h]
//Author:石原颯馬
//
//======================================================
#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "main.h"
#include "objectX.h"

//建物クラス
class CBuilding : public CObjectX
{
public:
	//コンストラクタ・デストラクタ
	CBuilding();
	CBuilding(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx);
	~CBuilding();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CBuilding* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx);

	//取得
	static CBuilding* GetBuilding(int nIdx) { return m_apBuilding[nIdx]; }

	//ダメージ付与
	void AddDamage(int nDamage);

private:
	static CBuilding* m_apBuilding[MAX_OBJ];
	static int m_nNumAll;			//建物クラス内での個数
	int m_nID;						//建物クラス内でのID
	int m_nEndurance;				//耐久値
	//const int m_nEnduranceMax;	//初期値
	//const int m_nValue;			//建物の価値
};

#endif // !_BUILDING_H_
