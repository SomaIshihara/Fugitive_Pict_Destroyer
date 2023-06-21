//======================================================
//
//レンダリング処理のヘッダ[renderer.h]
//Author:石原颯馬
//
//======================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

class CRenderer
{
public:
	//ブレンディング種類列挙
	typedef enum
	{
		BLENDTYPE_NORMAL = 0,	//通常
		BLENDTYPE_ADD,			//加算	
		BLENDTYPE_SUB,			//減算
		BLENDTYPE_MAX
	} BLENDTYPE;

	//コンストラクタ・デストラクタ
	CRenderer();
	~CRenderer();

	//基本処理
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定
	void SetBlendType(BLENDTYPE type);
	void SetEnableZTest(bool bEnable);
	void SetEnableAlplaTest(bool bEnable);

	//取得
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
	LPDIRECT3D9 m_pD3D;						//Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Direct3Dデバイスへのポインタ
};

#endif // !_RENDERER_H_