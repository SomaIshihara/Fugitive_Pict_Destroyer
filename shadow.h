//======================================================
//
//影の処理のヘッダ[shadow.h]
//Author:石原颯馬
//
//======================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "object3D.h"

//影クラス
class CShadow : public CObject3D
{
public:
	//コンストラクタ・デストラクタ
	CShadow();
	~CShadow();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//付与（生成）
	static CShadow* Create(void);

	//破棄
	void Release(void);

	//設定
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
private:
	int m_nIdxTexture;	//テクスチャ番号
};

#endif // !_SHADOW_H_
