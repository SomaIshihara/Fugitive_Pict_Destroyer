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
	//コンストラクタ・デストラクタ
	CRenderer();
	~CRenderer();

	//基本処理
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
	LPDIRECT3D9 m_pD3D;						//Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Direct3Dデバイスへのポインタ
};

#endif // !_RENDERER_H_