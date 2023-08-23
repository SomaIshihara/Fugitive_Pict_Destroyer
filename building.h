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
	//建物パラメータ構造体
	typedef struct
	{
		char aName[64];
		int nEndurance;
		long long nValue;
	} BuildingParam;

	//コンストラクタ・デストラクタ
	CBuilding();
	CBuilding(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);
	~CBuilding();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//パラメータ読み込み
	static void LoadParam(const char* pPath);

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
	static BuildingParam GetBuildingParam(int nIdx) { return m_aBuildingParam[nIdx]; }

	//ダメージ付与
	void AddDamage(int nDamage);

private:
	static CBuilding* m_apBuilding[MAX_OBJ];
	static BuildingParam m_aBuildingParam[MAX_OBJ];
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

	//見た目
	float m_fRedAlpha;	//ダメージを食らった時の赤色具合
};

#endif // !_BUILDING_H_
