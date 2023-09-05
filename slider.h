//======================================================
//
//スライダー処理のヘッダ[slider.h]
//Author:石原颯馬
//
//======================================================
#ifndef _SLIDER_H_
#define _SLIDER_H

#include "object.h"
#include "manager.h"

//前方宣言
class CObject2D;

//スライダークラス
class CSlider : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CSlider();
	CSlider(const D3DXVECTOR3 pos,const float fOneWidth,const float fOneHeight, const int nIdxNum);
	~CSlider();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CSlider* Create(const D3DXVECTOR3 pos, const float fOneWidth, const float fOneHeight, const int nIdxNum);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return CManager::VEC3_ZERO; }
	float GetWidth(void) { return m_fOneWidth * m_nIdxNum; }
	float GetHeight(void) { return m_fOneHeight; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }
	int GetModelIdx(void) { return CManager::INT_ZERO; }
	int GetSelectIdx(void) { return m_nSelect; }

	//設定
	void SetSelectIdx(int nIdx) { m_nSelect = (nIdx + m_nIdxNum) % m_nIdxNum; }

private:
	//オブジェクト
	CObject2D* m_pObjFrame;		//枠
	CObject2D* m_pObjMove;		//移動部
	CObject2D** m_ppObjIcon;	//アイコン（個数可変）

	//値
	D3DXVECTOR3 m_pos;		//中心位置
	float m_fOneWidth;		//1枠当たりの幅
	float m_fOneHeight;		//1枠当たりの高さ
	int m_nIdxNum;			//枠数
	int m_nSelect;			//選択中の番号
	bool m_bClick;			//クリック中か
};

#endif // !_SLIDER_H_
