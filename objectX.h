//======================================================
//
//オブジェクト（X）処理のヘッダ[objectX.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_
#include "main.h"
#include "manager.h"
#include "object.h"

//マクロ
#define X_MODEL_NUM		(64)	//モデル保管個数
#define X_TEXTURE_NUM	(16)	//テクスチャ使用数

//オブジェクトクラス
class CObjectX : public CObject
{
public:
	typedef struct
	{
		LPD3DXMESH m_pMesh;								//メッシュ
		LPD3DXBUFFER m_pBuffMat;						//マテリアルポインタ
		DWORD m_dwNumMatModel;							//マテ数
		LPDIRECT3DTEXTURE9 m_apTexture[X_TEXTURE_NUM];	//テクスチャポインタ
	} Model;
	//コンストラクタ・デストラクタ
	CObjectX(int nPriority = PRIORITY_DEFAULT);																	//デフォルト
	CObjectX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx, int nPriority = PRIORITY_DEFAULT);	//オーバーロード（位置向きサイズ）
	virtual ~CObjectX();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//読み込み
	static void Load(const char* pPath, const int nIdx);
	static void Unload(void);

	//生成
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }

private:
	//モデル
	static Model m_aModel[X_MODEL_NUM];	//モデル
	D3DXMATRIX mtxWorld;				//ワールドマトリ
	int m_nIdx;							//モデル番号

	//位置類
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅(X)
	float m_fHeight;	//高さ(Y)
	float m_fDepth;		//奥行(Z)
};

#endif // !_OBJECT_H_
