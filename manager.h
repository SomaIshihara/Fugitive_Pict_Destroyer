//======================================================
//
//マネージャのヘッダ[manager.h]
//Author:石原颯馬
//
//======================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"

//前方宣言
class CInputKeyboard;
class CRenderer;
class CPlayer;
class CDebugProc;
class CSound;

//マクロ
#define INT_ZERO	(0)							//int型の0
#define FLOAT_ZERO	(0.0f)						//float型の0
#define VEC2_ZERO	D3DXVECTOR2(0.0f,0.0f)		//vec2型の0
#define VEC3_ZERO	D3DXVECTOR3(0.0f,0.0f,0.0f)	//vec3型の0

class CManager
{
public:
	//コンストラクタ・デストラクタ
	CManager();
	~CManager();

	//基本処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	//FPS計測
	void CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime);

	//取得
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CSound* GetSound(void) { return m_pSound; }
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CDebugProc* GetDebProc(void) { return m_pDebProc; }

private:
	static CInputKeyboard* m_pInputKeyboard;	//キーボード
	static CSound* m_pSound;					//サウンド
	static CRenderer* m_pRenderer;				//レンダラー
	static CDebugProc* m_pDebProc;				//デバッグ
	static CPlayer* m_pPlayer;					//プレイヤー
	static int m_nFPS;							//FPS
	static DWORD m_dwFrameCount;				//回したフレーム数
};

#endif