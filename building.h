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
class CBuilding : public CObject
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
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	int GetModelIdx(void) { return m_nIdx; }

	//ダメージ付与
	void AddDamage(int nDamage);

private:
	static CBuilding* m_apBuilding[MAX_OBJ];
	static int m_nNumAll;			//建物クラス内での個数
	int m_nID;						//建物クラス内でのID
	int m_nEndurance;				//耐久値
	//const int m_nEnduranceMax;	//初期値
	//const int m_nValue;			//建物の価値
	D3DXMATRIX mtxWorld;			//ワールドマトリ
	int m_nIdx;						//モデル番号

	//位置類
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅(X)
	float m_fHeight;	//高さ(Y)
	float m_fDepth;		//奥行(Z)
};

#endif // !_BUILDING_H_
