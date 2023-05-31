//======================================================
//
//マネージャのヘッダ[manager.h]
//Author:石原颯馬
//
//======================================================
#include "main.h"

class CInputKeyboard;
class CRenderer;
class CPlayer;
class CDebugProc;

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
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CDebugProc* GetDebProc(void) { return m_pDebProc; }

private:
	static CInputKeyboard* m_pInputKeyboard;	//キーボード
	static CRenderer* m_pRenderer;				//レンダラー
	static CDebugProc* m_pDebProc;				//デバッグ
	static CPlayer* m_pPlayer;					//プレイヤー
	static int m_nFPS;							//FPS
	static DWORD m_dwFrameCount;				//回したフレーム数
};