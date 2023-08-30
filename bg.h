//======================================================
//
//背景（2D）処理のヘッダ[bg.h]
//Author:石原颯馬
//
//======================================================
#ifndef _BG_H_
#define _BG_H_
#include "main.h"
#include "object2D.h"

class CBG : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CBG(int nPriority = PRIORITY_BG);
	~CBG();

	//生成
	static CBG* Create(const int nPriority = PRIORITY_BG);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nIdxTexture;	//テクスチャ番号
};

#endif // !_BG_H_