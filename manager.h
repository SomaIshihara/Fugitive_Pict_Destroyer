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
class CInputMouse;
class CRenderer;
class CPlayer;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CTexture;
class CPlayer;
class CMeshField;
class CButton3D;

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
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; }
	static CSound* GetSound(void) { return m_pSound; }
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CDebugProc* GetDebProc(void) { return m_pDebProc; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CLight* GetLight(void) { return m_pLight; }
	static CTexture* GetTexture(void) { return m_pTexture; }
	//仮
	static CMeshField* GetMeshField(void) { return m_pMeshField; }

private:
	//再配置
	void ResetObj(void);

	//変数
	static CInputKeyboard* m_pInputKeyboard;	//キーボード
	static CInputMouse* m_pInputMouse;			//マウス
	static CSound* m_pSound;					//サウンド
	static CRenderer* m_pRenderer;				//レンダラー
	static CDebugProc* m_pDebProc;				//デバッグ
	static CPlayer* m_pPlayer;					//プレイヤー
	static CCamera* m_pCamera;					//カメラ
	static CLight* m_pLight;					//ライト
	static CTexture* m_pTexture;				//テクスチャ
	static CButton3D* m_pButton3D;				//3Dボタン
	//仮
	static CMeshField* m_pMeshField;			//仮：メッシュフィールド
	//仮
	static int m_nFPS;							//FPS
	static DWORD m_dwFrameCount;				//回したフレーム数
};

#endif