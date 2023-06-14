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

//優先順位
#define BLOCK_PRIORITY	(2)

//ブロッククラス
class CBlock : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CBlock(int nPriority = BLOCK_PRIORITY);	//デフォルト
	CBlock(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority = BLOCK_PRIORITY);	//オーバーロード
	~CBlock();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CBlock* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
private:
	int m_nIdxTexture;	//テクスチャ番号
};

#endif // !_BLOCK_H_
