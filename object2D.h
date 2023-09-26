//======================================================
//
//オブジェクト（2D）処理のヘッダ[object2D.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_
#include "main.h"
#include "manager.h"
#include "object.h"

//パターンテクスチャ構造体定義
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture;	//テクスチャポインタ
	int nPatternWidth;				//パターン幅
	int nPatternHeight;				//パターン高さ
} PatternTexture;

//オブジェクトクラス
class CObject2D : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CObject2D(int nPriority = PRIORITY_DEFAULT);																						//デフォルト
	CObject2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority = PRIORITY_DEFAULT);	//オーバーロード（位置向きサイズ）
	virtual ~CObject2D();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CObject2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nPriority = PRIORITY_DEFAULT);

	//テクスチャ設定
	void BindTexture(int nIdx) { m_nIdxTexture = nIdx; }

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }
	float GetLength(void) { return m_fLength; }
	int GetIdxTexture(void) { return m_nIdxTexture; }

	//設定
	HRESULT SetPos(const D3DXVECTOR3 pos);	//位置
	HRESULT SetRot(const D3DXVECTOR3 rot);	//向き
	HRESULT SetCol(const D3DXCOLOR col);	//色
	HRESULT SetTex(const D3DXVECTOR2 tex0, const D3DXVECTOR2 tex3);	//テクスチャ座標
	HRESULT SetSize(const float fWidth, const float fHeight);		//サイズ
	void SetEnable(const bool bFrag) { m_bEnable = bFrag; }

private:
	//頂点位置設定
	HRESULT SetVtxPos(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;		//頂点バッファ
	int m_nIdxTexture;	//テクスチャ番号
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fLength;	//長さ
	float m_fAngle;		//角度
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	bool m_bEnable;		//有効・無効設定
};

#endif // !_OBJECT_H_
