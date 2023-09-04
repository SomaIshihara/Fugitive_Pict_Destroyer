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

//前方宣言
class CXModel;

//建物クラス
class CBuilding : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CBuilding();
	CBuilding(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);
	~CBuilding();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CBuilding* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//取得
	static CBuilding* GetBuilding(int nIdx) { return m_apBuilding[nIdx]; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	CXModel* GetModel(void) { return m_pModel; }
	int GetEndurance(void) { return m_nEndurance; }
	static int GetNumAll(void) { return m_nNumAll; }
	bool GetUnique(void) { return m_bUnique; }
	int GetLv(void) { return m_nLv; }
	float GetSigValue(void) { return m_fSigValue; }
	int GetPowValue(void) { return m_nPowValue; }
	float GetSigEndurance(void) { return m_fSigEndurance; }
	int GetPowEndurance(void) { return m_nPowEndurance; }
	int GetExp(void) { return m_nExp; }

	//設定
	void SetUnique(const bool bFrag) { m_bUnique = bFrag; }
	void SetLv(const int nLv);
	void SetSigValue(const float fSignificant) { m_fSigValue = fSignificant; }
	void SetPowValue(const int nPower) { m_nPowValue = nPower; }
	void SetSigEndurance(const float fSignificant);
	void SetPowEndurance(const int nPower);
	void SetExp(const int nExp) { m_nExp = nExp; }

	//ダメージ付与
	void AddDamage(int nDamage);

private:
	void CulcEndurance(void);

	static CBuilding* m_apBuilding[MAX_OBJ];
	static int m_nNumAll;			//建物クラス内での個数
	int m_nID;						//建物クラス内でのID
	int m_nEndurance;				//耐久値
	D3DXMATRIX mtxWorld;			//ワールドマトリ
	CXModel* m_pModel;				//モデルポインタ

	//位置類
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅(X)
	float m_fHeight;	//高さ(Y)
	float m_fDepth;		//奥行(Z)

	//パラメータ(m_bBreakable == true)
	bool m_bUnique;			//個別設定有無
	int m_nLv;				//レベル
	//↓m_bUnique == true
	float m_fSigValue;		//価値有効数字
	int m_nPowValue;		//価値べき乗
	float m_fSigEndurance;	//耐久有効数字
	int m_nPowEndurance;	//耐久べき乗
	int m_nExp;				//経験値

	//見た目
	float m_fRedAlpha;	//ダメージを食らった時の赤色具合
};

#endif // !_BUILDING_H_
