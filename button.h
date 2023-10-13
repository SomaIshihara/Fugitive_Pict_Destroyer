//======================================================
//
//ボタン処理のヘッダ[button.h]
//Author:石原颯馬
//
//======================================================
#ifndef _BUTTON_H_
#define _BUTTON_H_
#include "object2D.h"
#include "object3D.h"

//2Dボタンクラス
class CButton2D : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CButton2D();
	CButton2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority = PRIORITY_UI);
	~CButton2D();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CButton2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority = PRIORITY_UI);

	//状態取得処理
	bool IsHold(void) { return m_bHold; }
	bool IsClickPress(void) { return m_bPress; }
	bool IsClickTrigger(void) { return m_bTrigger; }
	bool IsClickRelease(void) { return m_bRelease; }
	bool IsClickRepeate(void) { return m_bRepeate; }
private:
	bool m_bHold;				//かざしている
	bool m_bPress;				//プレス
	bool m_bTrigger;			//トリガー
	bool m_bRelease;			//リリース
	bool m_bRepeate;			//リピート
};

//3Dボタンクラス
class CButton3D : public CObject3D
{
public:
	//コンストラクタ・デストラクタ
	CButton3D();
	CButton3D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority = PRIORITY_UI);
	~CButton3D();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CButton3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);

	//状態取得処理
	bool IsHold(void) { return m_bHold; }
	bool IsClickPress(void) { return m_bPress; }
	bool IsClickTrigger(void) { return m_bTrigger; }
	bool IsClickRelease(void) { return m_bRelease; }
	bool IsClickRepeate(void) { return m_bRepeate; }
private:
	bool m_bHold;				//かざしている
	bool m_bPress;				//プレス
	bool m_bTrigger;			//トリガー
	bool m_bRelease;			//リリース
	bool m_bRepeate;			//リピート
};

#endif // !_BUTTON_H_
