//======================================================
//
//マルチ背景（2D）処理のヘッダ[bg.h]
//Author:石原颯馬
//
//======================================================
#ifndef _MULTIPLE_BG_H_
#define _MULTIPLE_BG_H_
#include "main.h"
#include "manager.h"
#include "object.h"

#define MAX_MULTIPLE_BG	(3)	//多重背景の個数
class CObject2D;

class CMultipleBG : public CObject	//このクラスは管理オブジェクトです。Unityでいう空のオブジェクトみたいなもの。
{
public:
	//コンストラクタ・デストラクタ
	CMultipleBG(int nPriority = PRIORITY_BG);				//デフォルト
	~CMultipleBG();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CMultipleBG* Create(float fSpeed0, float fSpeed1, float fSpeed2);	//オブジェクトを生成 fSpeed:スクロール速度

	//取得（純粋仮想関数の関係上実装しているが、こいつに位置やらサイズやらはいらないのですべて0を返す）
	D3DXVECTOR3 GetPos(void) { return VEC3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return VEC3_ZERO; }
	float GetWidth(void) { return FLOAT_ZERO; }
	float GetHeight(void) { return FLOAT_ZERO; }
	float GetDepth(void) { return FLOAT_ZERO; }

private:
	CObject2D* m_pObj2D[MAX_MULTIPLE_BG];	//2Dオブジェクト
	float m_aTexV[MAX_MULTIPLE_BG];			//背景のテクスチャ座標
	float m_aSpeed[MAX_MULTIPLE_BG];		//背景のスクロール速度
	int m_aIdxTexture[MAX_MULTIPLE_BG];		//テクスチャ番号
};

#endif // !_MULTIPLE_BG_H_