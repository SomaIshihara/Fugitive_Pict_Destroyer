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
class CXModel;
class CFade;
class CSlider;

//シーンクラス
class CScene
{
public:
	//モード列挙
	typedef enum
	{
		MODE_TITLE = 0,	//タイトル
		MODE_TUTORIAL,	//チュートリアル
		MODE_GAME,		//ゲーム（リザルト）
		MODE_RANKING,	//ランキング
		MODE_MAX
	} MODE;
	
	//コンストラクタ・デストラクタ
	CScene();
	~CScene();

	//基本処理
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void)  = 0;

	//生成
	static CScene* Create(MODE mode);

	//取得
	MODE GetMode(void) { return m_mode; }
	virtual CSlider* GetSlider(void) = 0;

private:
	MODE m_mode;	//モード
};
class CManager
{
public:
	//静的const
	static const int CManager::INT_ZERO;			//int型の0
	static const float CManager::FLOAT_ZERO;		//float型の0
	static const D3DXVECTOR2 VEC2_ZERO;				//vec2型の0
	static const D3DXVECTOR3 CManager::VEC3_ZERO;	//vec3型の0

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
	static CDebugProc* GetDebProc(void) { return m_pDebProc; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CLight* GetLight(void) { return m_pLight; }
	static CTexture* GetTexture(void) { return m_pTexture; }
	static bool GetPause(void) { return m_bPause; }
	//仮
	static CXModel* GetAgitModel(void) { return m_pAgitModel; }
	static CXModel* GetFragModel(void) { return m_pFragModel; }

	//シーン系
	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void) { return m_pScene->GetMode(); }
	static CScene* GetScene(void) { return m_pScene; }

	//設定
	static void SetPause(const bool bFrag) { m_bPause = bFrag; }

private:
	//変数
	static CInputKeyboard* m_pInputKeyboard;	//キーボード
	static CInputMouse* m_pInputMouse;			//マウス
	static CSound* m_pSound;					//サウンド
	static CRenderer* m_pRenderer;				//レンダラー
	static CDebugProc* m_pDebProc;				//デバッグ
	static CCamera* m_pCamera;					//カメラ
	static CLight* m_pLight;					//ライト
	static CTexture* m_pTexture;				//テクスチャ
	static CXModel* m_pAgitModel;				//仮：アジトモデルポインタ
	static int m_nFPS;							//FPS
	static DWORD m_dwFrameCount;				//回したフレーム数

	//仮
	static CXModel* m_pFragModel;	//旗モデル

	static CScene* m_pScene;	//シーン

	static bool m_bPause;		//ポーズ状態
};

#endif