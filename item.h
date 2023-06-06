//======================================================
//
//アイテム処理のヘッダ[item.h]
//Author:石原颯馬
//
//======================================================
#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"
#include "object2D.h"

//優先順位
#define ITEM_PRIORITY	(PRIORITY_DEFAULT)

//ブロッククラス
class CItem : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CItem(int nPriority = ITEM_PRIORITY);	//デフォルト
	CItem(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority = ITEM_PRIORITY);	//オーバーロード
	~CItem();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//読み込み
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//生成
	static CItem* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
};

#endif // !_BLOCK_H_
