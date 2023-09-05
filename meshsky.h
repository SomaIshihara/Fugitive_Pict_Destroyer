//======================================================
//
//メッシュ空処理のヘッダ[meshsky.h]
//Author:石原颯馬
//
//======================================================
#ifndef _MESHSKY_H_
#define _MESHSKY_H_

#include "object.h"
#include "manager.h"

//メッシュフィールドクラス
class CMeshSky : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CMeshSky();
	CMeshSky(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const int nBlockWidth, const int nBlockDepth);
	~CMeshSky();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CMeshSky* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const int nBlockWidth,const int nBlockDepth);

	//テクスチャ設定
	void BindTexture(int nIdx) { m_nIdxTexture = nIdx; }

	//設定
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }	//位置
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }	//向き

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return CManager::FLOAT_ZERO; }
	float GetHeight(void) { return m_fRadius; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }
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
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffTop;		//上部インデックスバッファ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMiddle;	//中部インデックスバッファ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffBottom;	//下部インデックスバッファ
	D3DXMATRIX m_mtxWorld;				//ワールドマトリ


	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fRadius;	//半径
	int m_nBlockVertical;	//ブロック幅
	int m_nBlockHorizontal;	//ブロック奥行
};

#endif // !_MESHFIELD_H_
