//======================================================
//
//オブジェクト処理のヘッダ[object.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "main.h"

#define MAX_OBJ				(256)	//オブジェクト最大数
#define DEATH_LIFE			(0)		//死亡体力

//優先順位参考表
typedef enum
{
	PRIORITY_BG = 0,		//BG
	PRIORITY_DEFAULT = 3,	//デフォルト
	PRIORITY_UI = 5,		//UI
	PRIORITY_PAUSE,			//ポーズ
	PRIORITY_FADE,			//フェード
	PRIORITY_MAX			//最大優先順位（これで足りるらしい）
} PRIORITY;

//オブジェクトクラス
class CObject
{
public:
	//種類列挙
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_BULLET,
		TYPE_BLOCK,
		TYPE_ITEM,
		TYPE_BUILDING,
		TYPE_PICT,
		TYPE_MAX
	} TYPE;

	//コンストラクタ・デストラクタ
	CObject(int nPriority = PRIORITY_DEFAULT);
	virtual ~CObject();

	//基本処理
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//全オブジェクト系
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	//取得・設定
	void SetType(TYPE type) { m_Type = type; }
	TYPE GetType(void) { return m_Type; }
	static CObject* GetObject(int nPriority, int nIdx) { return m_apObject[nPriority][nIdx]; }

	//純粋仮想取得
	virtual D3DXVECTOR3 GetPos(void) = 0;
	virtual D3DXVECTOR3 GetRot(void) = 0;
	virtual float GetWidth(void) = 0;
	virtual float GetHeight(void) = 0;
	virtual float GetDepth(void) = 0;
	virtual int GetModelIdx(void) = 0;

protected:
	void Release(void);

private:
	static CObject* m_apObject[PRIORITY_MAX][MAX_OBJ];	//ポインタ
	static int m_nNumAll;					//総数
	int m_nID;								//自分自身のID
	int m_nPriority;						//優先順位
	TYPE m_Type;							//種類
};

#endif // !_OBJECT_H_
