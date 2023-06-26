//======================================================
//
//ブロック処理のヘッダ[block.h]
//Author:石原颯馬
//
//======================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"
#include "object2D.h"
#include "objectX.h"

//優先順位
#define BLOCK_PRIORITY	(2)

//ブロック2Dクラス
class CBlock2D : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CBlock2D(int nPriority = BLOCK_PRIORITY);	//デフォルト
	CBlock2D(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority = BLOCK_PRIORITY);	//オーバーロード
	~CBlock2D();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CBlock2D* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
private:
	int m_nIdxTexture;	//テクスチャ番号
};

//ブロック2Dクラス
class CBlock3D : public CObjectX
{
public:
	//コンストラクタ・デストラクタ
	CBlock3D(int nPriority = BLOCK_PRIORITY);	//デフォルト
	CBlock3D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx, int nPriority = BLOCK_PRIORITY);	//オーバーロード
	~CBlock3D();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CBlock3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx);
private:
	int m_nIdxTexture;	//テクスチャ番号
};

#endif // !_BLOCK_H_
