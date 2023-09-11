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
#include "objectX.h"

//前方宣言
class CPictoTaxi;
class CXModel;

//優先順位
#define ITEM_PRIORITY	(PRIORITY_DEFAULT)

//アイテムクラス
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

	//生成
	static CItem* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
private:
	int m_nIdxTexture;	//テクスチャ番号
};

//アイテム弾クラス
class CItemBullet : public CObjectX
{
public:
	//コンストラクタ・デストラクタ
	CItemBullet();
	CItemBullet(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	~CItemBullet();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CItemBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

	//設定
	static void SetModel(CXModel* nModel) { m_pModel = nModel; }

	//取得
	int GetBulletNum(void) { return m_nBulletNum; }
	static CItemBullet* GetItemBullet(const int nIdx) { return m_pItemBullet[nIdx]; }

	//取り込み
	void PickBullet(CPictoTaxi* taxi);

private:
	static CItemBullet* m_pItemBullet[MAX_OBJ];
	int m_nID;
	static int m_nNumAll;
	int m_nBulletNum;	//弾数
	static CXModel* m_pModel;	//弾薬箱モデルの番号
};

#endif // !_BLOCK_H_
