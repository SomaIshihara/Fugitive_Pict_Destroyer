//==========================================================================================
//
//入力プログラムのヘッダ[input.h]
//Author:石原颯馬（振動部:平澤詩苑）
//
//==========================================================================================
//Ver.3.0.0（C++制作のもの）
//使用可能な入力:キーボード・マウス（カーソル位置・移動量）・ゲームパッド（1~4台）
//==========================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//==========================================================================================
//マクロ定義
//==========================================================================================
#define STICK_MAX			(32767)	//スティック最大値
#define MOUSE_CLICK_LEFT	(0)		//マウス左クリック
#define MOUSE_CLICK_RIGHT	(1)		//マウス右クリック
#define MAX_USE_GAMEPAD		(4)		//ゲームパッド使用可能台数
#define MOUSE_BUTTON_MAX	(3)		//マウスボタンの最大数

#define VIBE_POWER_MIN			(7000)	//最低振動量
#define VIBE_POWER_LEVEL_01		(10000)	//弱めの振動
#define VIBE_POWER_LEVEL_02		(23000)	//中くらいの振動
#define VIBE_POWER_LEVEL_03		(37000)	//強めの振動
#define VIBE_POWER_LEVEL_04		(50000)	//かなり強めの振動
#define VIBE_POWER_LEVEL_05		(65535)	//最大振動

//==========================================================================================
//構造体定義
//==========================================================================================
//リピート時間
typedef struct
{
	DWORD currentTime;	//現在の時間
	DWORD execLastTime;	//最後にtrueにした時間
} RepeateTime;

//**************************************************
//*						クラス					   *
//**************************************************
//==================================================
//						入力本体
//==================================================
class CInput
{
public:
	//コンストラクタ・デストラクタ
	CInput();
	virtual ~CInput();

	//基本処理
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected:
	static LPDIRECTINPUT8 m_pInput;	//オブジェクト
	LPDIRECTINPUTDEVICE8 m_pDevice;	//デバイス
private:
	static int m_nNumAll;			//使用コントローラ数
};

//==================================================
//						キーボード
//==================================================
class CInputKeyboard : public CInput
{
public:
	//キーボード構造体
	typedef struct
	{
		BYTE state;		//状態（保存用およびプレス情報）
		BYTE trigger;	//トリガー情報
		BYTE release;	//リリース情報
		BYTE repeate;	//リピート情報
		RepeateTime repeateTime;	//リピート時間
	} Keyboard;

	//コンストラクタ・デストラクタ
	CInputKeyboard();
	~CInputKeyboard();

	//基本処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	//取得
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeate(int nKey);

private:
	Keyboard m_aKeyState[256];	//キーボードすべての状態（トリガー等含む）
};

//==================================================
//						マウス
//==================================================
class CInputMouse : public CInput
{
public:
	//マウスのボタン部
	typedef struct
	{
		BYTE trigger;		//ボタンのトリガー情報
		BYTE release;		//ボタンのリリース情報
		BYTE repeate;		//リピート情報
		RepeateTime repeateTime;	//リピート時間
	} MouseButton;

	//マウス構造体
	typedef struct
	{
		MouseButton mb[MOUSE_BUTTON_MAX];	//マウスボタン
		DIMOUSESTATE state;					//状態およびプレス情報
		D3DXVECTOR3 pos;					//位置
		D3DXVECTOR3 move;					//移動量
	} Mouse;

	//コンストラクタ・デストラクタ
	CInputMouse();
	~CInputMouse();

	//基本処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	//クリック座標をワールド座標に変換
	D3DXVECTOR3 ConvertClickPosToWorld(float fZ);

	//取得
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeate(int nKey);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetMove(void);
	int GetWheel(void) { return m_mouse.state.lZ; }

private:
	Mouse m_mouse;	//マウスすべて
};

//==================================================
//					 ゲームパッド
//==================================================
class CInputGamePad : public CInput
{
public:
	//ゲームパッドの振動状態
	typedef enum
	{
		VIBE_STATE_00_STOP,	//停止
		VIBE_STATE_01_LOOP,	//ずっと同じ振動
		VIBE_STATE_02_FADE,	//徐々に落ち着いていく
		VIBE_STATE_MAX
	}VIBE_STATE;

	//ゲームパッド構造体
	typedef struct
	{
		XINPUT_STATE state;		//状態全部（およびプレス情報）
		WORD trigger;			//トリガー情報
		WORD release;			//リリース情報
		WORD repeate;			//リピート情報
		RepeateTime repeateTime;	//リピート時間

		VIBE_STATE Vibr_State;	//振動の状態
		WORD wVibrPower;		//振動の強さ
		int nVibrTime;			//振動させる時間
	} GamePad;


	//コンストラクタ・デストラクタ
	CInputGamePad();
	~CInputGamePad();

	//基本処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	//振動処理
	void UpdateVibration(void);
	void StopVibration(void);
	void SetPadVibration(WORD wPower, int nVibeTime, VIBE_STATE State);

	//取得
	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeate(int nButton);
	SHORT GetLStickX(void);
	SHORT GetLStickY(void);
	SHORT GetRStickX(void);
	SHORT GetRStickY(void);
private:
	static int m_nNumAll;//ゲームパッド数
	GamePad m_gamePad;	//ゲームパッドすべて
	int m_nID;			//ゲームパッドID
};

#endif // !_INPUT_H_
