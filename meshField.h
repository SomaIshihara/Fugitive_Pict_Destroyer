//======================================================
//
//メッシュフィールド処理のヘッダ[meshField.h
//Author:石原颯馬
//
//======================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "object.h"
#include "manager.h"

//メッシュフィールドクラス
class CMeshField : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CMeshField();
	CMeshField(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth,
		const int nBlockWidth, const int nBlockDepth);
	~CMeshField();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CMeshField* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth,
		const int nBlockWidth,const int nBlockDepth);

	//テクスチャ設定
	void BindTexture(int nIdx) { m_nIdxTexture = nIdx; }

	//設定
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }	//位置
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }	//向き

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return CManager::FLOAT_ZERO; }
	float GetDepth(void) { return m_fDepth; }
	int GetIdxTexture(void) { return m_nIdxTexture; }
	int GetModelIdx(void) { return -1; }

	//高さ求める
	float GetHeight(D3DXVECTOR3 posNew);
private:
	//関数
	WORD* SetIdxSwaingField(int nCntWidth, WORD *pIdx);

	//変数
	int m_nIdxTexture;					//テクスチャ番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;	//頂点バッファ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファ
	D3DXMATRIX m_mtxWorld;				//ワールドマトリ


	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅
	float m_fDepth;		//奥行
	int m_nBlockWidth;	//ブロック幅
	int m_nBlockDepth;	//ブロック奥行
};

#endif // !_MESHFIELD_H_
