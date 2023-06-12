//======================================================
//
//オブジェクト（3D）処理のヘッダ[object3D.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_
#include "main.h"
#include "manager.h"
#include "object.h"

//オブジェクトクラス
class CObject3D : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CObject3D(int nPriority = PRIORITY_DEFAULT);																						//デフォルト
	CObject3D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority = PRIORITY_DEFAULT);	//オーバーロード（位置向きサイズ）
	virtual ~CObject3D();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CObject3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return FLOAT_ZERO; }
	float GetDepth(void) { return m_fDepth; }

private:
	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;	//頂点バッファ
	D3DXMATRIX m_mtxWorld;				//ワールドマトリ

	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅(X)
	float m_fDepth;		//奥行(Z)
};

#endif // !_OBJECT_H_
