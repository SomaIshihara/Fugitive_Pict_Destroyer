//==========================================================================================
//
//入力プログラム[input.cpp]
//Author:石原颯馬（振動部:平澤詩苑）
//
//==========================================================================================
//Ver.3.0.0（C++制作のもの）
//使用可能な入力:キーボード・マウス（カーソル位置・移動量）・ゲームパッド（1~4台）
//==========================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX			(256)	//キーの最大数
#define REPEATE_TIME		(150)	//リピートの間隔
#define GAMEPAD_BUTTON_NUM	(14)	//ゲームパッドのボタン数
#define STICK_DEADZONE		(655)	//遊び
#define PAD_VIBE_FADE		(200)	//振動の減少量

//静的メンバ変数
LPDIRECTINPUT8 CInput::m_pInput = nullptr;
int CInput::m_nNumAll = 0;
int CInputGamePad::m_nNumAll = 0;

//==================================================
//						入力本体
//==================================================
//========================
//コンストラクタ
//========================
CInput::CInput()
{

}

//========================
//デストラクタ
//========================
CInput::~CInput()
{}

//========================
//初期化
//========================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		//DireceInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pInput, nullptr)))
		{
			return E_FAIL;
		}
	}

	//使用コントローラ数増やす
	m_nNumAll++;

	return S_OK;
}

//========================
//終了
//========================
void CInput::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	//DirectInputオブジェクトの破棄
	if (m_nNumAll <= 1 && m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}

	//使用コントローラ数減らす
	m_nNumAll--;
}

//==================================================
//						キーボード
//==================================================
//========================
//コンストラクタ
//========================
CInputKeyboard::CInputKeyboard()
{
	//キー情報・時間クリア
	for (int nCntInit = 0; nCntInit < NUM_KEY_MAX; nCntInit++)
	{
		//キー
		m_aKeyState[nCntInit].state = 0;
		m_aKeyState[nCntInit].trigger = 0;
		m_aKeyState[nCntInit].release = 0;
		m_aKeyState[nCntInit].repeate = 0;

		//時間
		m_aKeyState[nCntInit].repeateTime.currentTime = 0;
	}
}

//========================
//デストラクタ
//========================
CInputKeyboard::~CInputKeyboard()
{}

//========================
//初期化
//========================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//本体
	CInput::Init(hInstance, hWnd);
	
	//デバイス取得
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//データフォーマット設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モード設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	//時間初期化
	for (int nCntInit = 0; nCntInit < NUM_KEY_MAX; nCntInit++)
	{
		m_aKeyState[nCntInit].repeateTime.execLastTime = timeGetTime() - REPEATE_TIME;
	}

	return S_OK;
}

//========================
//終了
//========================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//========================
//更新
//========================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	//入力デバイスからデバイスを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyState[nCntKey].trigger = (m_aKeyState[nCntKey].state ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey].release = (m_aKeyState[nCntKey].state ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];

			//リピート
			m_aKeyState[nCntKey].repeateTime.currentTime = timeGetTime();
			if (aKeyState[nCntKey] && (m_aKeyState[nCntKey].repeateTime.currentTime - m_aKeyState[nCntKey].repeateTime.execLastTime) > REPEATE_TIME)
			{
				m_aKeyState[nCntKey].repeateTime.execLastTime = m_aKeyState[nCntKey].repeateTime.currentTime;
				m_aKeyState[nCntKey].repeate = aKeyState[nCntKey];
			}
			else
			{
				m_aKeyState[nCntKey].repeate = 0;
			}
			//プレス
			m_aKeyState[nCntKey].state = aKeyState[nCntKey];	//キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//========================
//キーボードのプレス情報を返す処理
//========================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey].state & 0x80) ? true : false;	//?が条件式作ることになってるみたいよ
}

//========================
//キーボードのトリガー情報を返す処理
//========================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyState[nKey].trigger & 0x80) ? true : false;
}

//========================
//キーボードのリリース情報を返す処理
//========================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyState[nKey].release & 0x80) ? true : false;
}

//========================
//キーボードのリピート情報を返す処理
//========================
bool CInputKeyboard::GetRepeate(int nKey)
{
	return (m_aKeyState[nKey].repeate & 0x80) ? true : false;
}

//==================================================
//						マウス
//==================================================
//========================
//コンストラクタ
//========================
CInputMouse::CInputMouse()
{
	//情報・時間初期化
	for (int nCntInit = 0; nCntInit < MOUSE_BUTTON_MAX; nCntInit++)
	{
		//情報
		m_mouse.mb[nCntInit].trigger = 0;
		m_mouse.mb[nCntInit].release = 0;
		m_mouse.mb[nCntInit].repeate = 0;

		//時間
		m_mouse.mb[nCntInit].repeateTime.currentTime = 0;
	}
}

//========================
//デストラクタ
//========================
CInputMouse::~CInputMouse()
{}

//========================
//初期化
//========================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//本体
	CInput::Init(hInstance, hWnd);

	//デバイス取得
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//データフォーマット設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//協調モード設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//マウスへのアクセス権を獲得
	m_pDevice->Acquire();

	//時間初期化
	for (int nCntInit = 0; nCntInit < MOUSE_BUTTON_MAX; nCntInit++)
	{
		m_mouse.mb[nCntInit].repeateTime.execLastTime = timeGetTime() - REPEATE_TIME;
	}

	return S_OK;
}

//========================
//終了
//========================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//========================
//更新
//========================
void CInputMouse::Update(void)
{
	DIMOUSESTATE MouseState;
	POINT point;

	//マウス取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState)))
	{
		for (int nCntKey = 0; nCntKey < MOUSE_BUTTON_MAX; nCntKey++)
		{
			//トリガーとリリース
			m_mouse.mb[nCntKey].trigger = (m_mouse.state.rgbButtons[nCntKey] ^ MouseState.rgbButtons[nCntKey]) & MouseState.rgbButtons[nCntKey];
			m_mouse.mb[nCntKey].release = (m_mouse.state.rgbButtons[nCntKey] ^ MouseState.rgbButtons[nCntKey]) & ~MouseState.rgbButtons[nCntKey];

			//リピート
			m_mouse.mb[nCntKey].repeateTime.currentTime = timeGetTime();
			if (MouseState.rgbButtons[nCntKey] && (m_mouse.mb[nCntKey].repeateTime.currentTime - m_mouse.mb[nCntKey].repeateTime.execLastTime) > REPEATE_TIME)
			{
				m_mouse.mb[nCntKey].repeateTime.execLastTime = m_mouse.mb[nCntKey].repeateTime.currentTime;
				m_mouse.mb[nCntKey].repeate = MouseState.rgbButtons[nCntKey];
			}
			else
			{
				m_mouse.mb[nCntKey].repeate = 0;
			}
		}

		//プレス
		m_mouse.state = MouseState;	//キーボードのプレス情報を保存

									//移動量
		m_mouse.move = D3DXVECTOR3((float)m_mouse.state.lX, (float)m_mouse.state.lY, CManager::FLOAT_ZERO);
	}
	else
	{
		m_pDevice->Acquire();
	}

	//マウス座標取得
	GetCursorPos(&point);
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &point);
	m_mouse.pos.x = (float)point.x;
	m_mouse.pos.y = (float)point.y;
}

//=================================
//クリックした座標からワールド座標に変換
//=================================
D3DXVECTOR3 CInputMouse::ConvertClickPosToWorld(float fZ)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得
	CCamera* pCamera = CManager::GetCamera();

	D3DXMATRIX mtxView, mtxProj;
	D3DXMATRIX mtxViewPort;
	D3DXMATRIX mtx;
	D3DXVECTOR3 posClick = m_mouse.pos;
	posClick.z = fZ;	//Z座標指定
	D3DXVECTOR3 posWorld;

	//ビューマトリ取得・逆行列化
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixInverse(&mtxView, nullptr, &mtxView);

	//プロジェクションマトリ取得・逆行列化
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	D3DXMatrixInverse(&mtxProj, nullptr, &mtxProj);

	//ビューポートマトリ設定・逆行列化
	D3DXMatrixIdentity(&mtxViewPort);
	mtxViewPort._11 = SCREEN_WIDTH * 0.5;
	mtxViewPort._22 = -SCREEN_HEIGHT * 0.5;
	mtxViewPort._41 = SCREEN_WIDTH * 0.5;
	mtxViewPort._42 = SCREEN_HEIGHT * 0.5;
	D3DXMatrixInverse(&mtxViewPort, nullptr, &mtxViewPort);

	//全部掛ける
	mtx = mtxViewPort * mtxProj * mtxView;	//内部でD3DXMatrixMultiplyやってるみたい

	//これでワールド座標に変換できた
	D3DXVec3TransformCoord(&posWorld, &posClick, &mtx);

	return posWorld;
}

//========================
//マウスのプレス情報を返す処理
//=======================
bool CInputMouse::GetPress(int nButton)
{
	return m_mouse.state.rgbButtons[nButton] & 0x80 ? true : false;
}

//========================
//マウスのトリガー情報を返す処理
//=======================
bool CInputMouse::GetTrigger(int nButton)
{
	return m_mouse.mb[nButton].trigger & 0x80 ? true : false;
}

//========================
//マウスのリリース情報を返す処理
//=======================
bool CInputMouse::GetRelease(int nButton)
{
	return m_mouse.mb[nButton].release & 0x80 ? true : false;
}

//========================
//マウスのリピート情報を返す処理
//=======================
bool CInputMouse::GetRepeate(int nButton)
{
	return m_mouse.mb[nButton].repeate & 0x80 ? true : false;
}

//========================
//マウスのカーソル位置を返す処理
//=======================
D3DXVECTOR3 CInputMouse::GetPos(void)
{
	return m_mouse.pos;
}

//========================
//マウスのカーソル移動量を返す処理
//=======================
D3DXVECTOR3 CInputMouse::GetMove(void)
{
	return m_mouse.move;
}

//==================================================
//						ゲームパッド
//==================================================
//========================
//コンストラクタ
//========================
CInputGamePad::CInputGamePad()
{
	//値クリア
	m_gamePad.state = {};
	m_gamePad.trigger = 0;
	m_gamePad.release = 0;
	m_gamePad.repeate = 0;
	m_gamePad.repeateTime.currentTime = 0;
	m_gamePad.wVibrPower = 0;
	m_gamePad.nVibrTime = 0;
	m_gamePad.Vibr_State = VIBE_STATE_00_STOP;
}

//========================
//デストラクタ
//========================
CInputGamePad::~CInputGamePad()
{}

//========================
//初期化
//========================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_nNumAll < MAX_USE_GAMEPAD)
	{//使用可能数以下
		//ゲームパッド類
		m_gamePad.repeateTime.execLastTime = timeGetTime();

		//ID割り振り
		m_nID = m_nNumAll;

		m_nNumAll++;
	}
	else
	{//使用可能数以上
		return E_FAIL;
	}

	return S_OK;
}

//========================
//終了
//========================
void CInputGamePad::Uninit(void)
{
	//コントローラーの振動停止
	StopVibration();

	//XInput終了
	XInputEnable(false);

	//本体
	CInput::Uninit();
}

//========================
//更新
//========================
void CInputGamePad::Update(void)
{
	XINPUT_STATE xInputState;

	//ゲームパッドから情報取得
	for (int nCntGPad = 0; nCntGPad < MAX_USE_GAMEPAD; nCntGPad++)
	{
		if (XInputGetState(nCntGPad, &xInputState) == ERROR_SUCCESS)
		{
			//ボタントリガー情報取得
			m_gamePad.trigger = (m_gamePad.state.Gamepad.wButtons ^ xInputState.Gamepad.wButtons) & xInputState.Gamepad.wButtons;

			//ボタンリリース情報取得
			m_gamePad.release = (m_gamePad.state.Gamepad.wButtons ^ xInputState.Gamepad.wButtons) & ~xInputState.Gamepad.wButtons;

			//リピート情報生成
			m_gamePad.repeateTime.currentTime = timeGetTime();
			for (int nCntKey = 0; nCntKey < GAMEPAD_BUTTON_NUM; nCntKey++)
			{
				if (m_gamePad.state.Gamepad.wButtons & 1 << nCntKey && (m_gamePad.repeateTime.currentTime - m_gamePad.repeateTime.execLastTime) > REPEATE_TIME)
				{
					m_gamePad.repeateTime.execLastTime = m_gamePad.repeateTime.currentTime;
					m_gamePad.repeate += 1 << nCntKey;
				}
			}

			//プレス情報その他もろもろ設定
			m_gamePad.state = xInputState;

			//コントローラーの振動状態更新
			UpdateVibration();
		}
	}
}

//========================
//ゲームパッド振動状態の更新
//========================
void CInputGamePad::UpdateVibration(void)
{
	//振動停止中
	if (m_gamePad.nVibrTime <= 0)
	{
		m_gamePad.wVibrPower = 0;					// 振動量    ０   に設定
		m_gamePad.nVibrTime = 0;					//継続時間   ０   に設定
		m_gamePad.Vibr_State = VIBE_STATE_00_STOP;	//  状態  「停止」に設定

		StopVibration();	//対象のコントローラーの振動停止
	}

	//振動中
	else
	{
		//継続時間減少
		m_gamePad.nVibrTime--;

		//バイブの状態が、徐々に弱まっていくものである
		if (m_gamePad.Vibr_State == VIBE_STATE_02_FADE)
		{
			//振動量が最低値を下回ったら
			if (m_gamePad.wVibrPower <= VIBE_POWER_MIN)
			{
				//振動を止める
				m_gamePad.wVibrPower = 0;
				m_gamePad.nVibrTime = 0;
				m_gamePad.Vibr_State = VIBE_STATE_00_STOP;

				StopVibration();	//対象のコントローラーの振動停止
			}
			else
			{
				//徐々に減少させる
				m_gamePad.wVibrPower -= PAD_VIBE_FADE;
			}
		}

		XINPUT_VIBRATION Vibration;

		Vibration.wLeftMotorSpeed = m_gamePad.wVibrPower;	//左のモーターの回転速度を設定
		Vibration.wRightMotorSpeed = m_gamePad.wVibrPower;	//右のモーターの回転速度を設定

		XInputSetState(m_nID, &Vibration);	//対象のコントローラーに振動を設定
	}
}

//========================
//対象のコントローラーの振動停止
//========================
void CInputGamePad::StopVibration(void)
{
	XINPUT_VIBRATION Vibration = {0,0};

	//対象のコントローラーの振動をOFF
	XInputSetState(m_nID, &Vibration);
}

//========================
//ゲームパッドのプレス情報を返す処理
//=======================
bool CInputGamePad::GetPress(int nButton)
{
	return m_gamePad.state.Gamepad.wButtons & nButton ? true : false;
}

//========================
//ゲームパッドのトリガー情報を返す処理
//=======================
bool CInputGamePad::GetTrigger(int nButton)
{
	return m_gamePad.trigger & nButton ? true : false;
}

//========================
//ゲームパッドのリリース情報を返す処理
//=======================
bool CInputGamePad::GetRelease(int nButton)
{
	return m_gamePad.release & nButton ? true : false;
}

//========================
//ゲームパッドのリピート情報を返す処理
//=======================
bool CInputGamePad::GetRepeate(int nButton)
{
	return m_gamePad.repeate & nButton ? true : false;
}

//========================
//左スティックの横軸を返す処理
//=======================
SHORT CInputGamePad::GetLStickX(void)
{
	//-値最大なら+値最大に合わせる
	if (m_gamePad.state.Gamepad.sThumbLX < -STICK_MAX)
	{
		m_gamePad.state.Gamepad.sThumbLX = -STICK_MAX;
	}

	if (abs(m_gamePad.state.Gamepad.sThumbLX) > STICK_DEADZONE)
	{
		return m_gamePad.state.Gamepad.sThumbLX;
	}
	else
	{
		return 0;
	}
}

//========================
//左スティックの縦軸を返す処理
//=======================
SHORT CInputGamePad::GetLStickY(void)
{
	//-値最大なら+値最大に合わせる
	if (m_gamePad.state.Gamepad.sThumbLY < -STICK_MAX)
	{
		m_gamePad.state.Gamepad.sThumbLY = -STICK_MAX;
	}

	if (abs(m_gamePad.state.Gamepad.sThumbLY) > STICK_DEADZONE)
	{
		return m_gamePad.state.Gamepad.sThumbLY;
	}
	else
	{
		return 0;
	}
}

//========================
//右スティックの横軸を返す処理
//=======================
SHORT CInputGamePad::GetRStickX(void)
{
	//-値最大なら+値最大に合わせる
	if (m_gamePad.state.Gamepad.sThumbRX < -STICK_MAX)
	{
		m_gamePad.state.Gamepad.sThumbRX = -STICK_MAX;
	}

	if (abs(m_gamePad.state.Gamepad.sThumbRX) > STICK_DEADZONE)
	{
		return m_gamePad.state.Gamepad.sThumbRX;
	}
	else
	{
		return 0;
	}
}

//========================
//右スティックの縦軸を返す処理
//=======================
SHORT CInputGamePad::GetRStickY(void)
{
	//-値最大なら+値最大に合わせる
	if (m_gamePad.state.Gamepad.sThumbRY < -STICK_MAX)
	{
		m_gamePad.state.Gamepad.sThumbRY = -STICK_MAX;
	}

	if (abs(m_gamePad.state.Gamepad.sThumbRY) > STICK_DEADZONE)
	{
		return m_gamePad.state.Gamepad.sThumbRY;
	}
	else
	{
		return 0;
	}
}

//========================
//ゲームパッド振動設定
//========================
void CInputGamePad::SetPadVibration(WORD wPower, int nVibeTime, VIBE_STATE State)
{
	m_gamePad.wVibrPower = wPower;		//強さ設定
	m_gamePad.nVibrTime = nVibeTime;	//時間設定
	m_gamePad.Vibr_State = State;		//状態設定

	XINPUT_VIBRATION Vibration;

	Vibration.wLeftMotorSpeed = wPower;		//左のモーターの回転速度を設定
	Vibration.wRightMotorSpeed = wPower;	//右のモーターの回転速度を設定

	XInputSetState(m_nID, &Vibration);	//対象のコントローラーに振動を設定
}