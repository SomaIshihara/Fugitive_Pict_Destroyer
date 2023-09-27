//======================================================
//
//ボタン処理[button.cpp]
//Author:石原颯馬
//
//======================================================
#include "button.h"
#include "input.h"
#include "Culc.h"
#include "manager.h"
#include "renderer.h"

//**************************************************
//2Dボタン
//**************************************************
//========================
//コンストラクタ
//========================
CButton2D::CButton2D()
{
	//値クリア
	m_bHold = false;
	m_bPress = false;
	m_bTrigger = false;
	m_bRelease = false;
	m_bRepeate = false;
}

//========================
//コンストラクタ（オーバーロード）
//========================
CButton2D::CButton2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority)
	: CObject2D(pos, rot, fWidth, fHeight, nPriority)
{
	//値クリア
	m_bHold = false;
	m_bPress = false;
	m_bTrigger = false;
	m_bRelease = false;
	m_bRepeate = false;
}

//========================
//デストラクタ
//========================
CButton2D::~CButton2D()
{
}

//========================
//初期化
//========================
HRESULT CButton2D::Init(void)
{
	//親処理
	CObject2D::Init();

	return S_OK;
}

//========================
//終了
//========================
void CButton2D::Uninit(void)
{
	//親処理
	CObject2D::Uninit();
}

//========================
//更新
//========================
void CButton2D::Update(void)
{
	//ボタン処理
	CInputMouse* mouse = CManager::GetInputMouse();	//マウス取得

	//マウス取得
	if (mouse != nullptr)
	{
		D3DXVECTOR3 buttonPos = GetPos();			//ボタン位置取得
		float fWidth = GetWidth();					//ボタン幅取得
		float fHeight = GetHeight();				//ボタン高さ取得
		D3DXVECTOR3 cursorPos = mouse->GetPos();	//カーソル位置取得

		if (cursorPos.x >= buttonPos.x - fWidth * 0.5f &&
			cursorPos.x <= buttonPos.x + fWidth * 0.5f &&
			cursorPos.y >= buttonPos.y - fHeight * 0.5f &&
			cursorPos.y <= buttonPos.y + fHeight * 0.5f)
		{//範囲内
			m_bHold = true;										//かざし
			m_bPress = mouse->GetPress(MOUSE_CLICK_LEFT);		//プレス
			m_bTrigger = mouse->GetTrigger(MOUSE_CLICK_LEFT);	//トリガー
			m_bRelease = mouse->GetRelease(MOUSE_CLICK_LEFT);	//リリース
			m_bRepeate = mouse->GetRepeate(MOUSE_CLICK_LEFT);	//リピート
		}
		else
		{//範囲外
			m_bHold = false;	//かざし
			m_bPress = false;	//プレス
			m_bTrigger = false;	//トリガー
			m_bRelease = false;	//リリース
			m_bRepeate = false;	//リピート
		}
	}
	else
	{//マウスが取得できない
		m_bHold = false;	//かざし
		m_bPress = false;	//プレス
		m_bTrigger = false;	//トリガー
		m_bRelease = false;	//リリース
		m_bRepeate = false;	//リピート
	}

	//親処理
	CObject2D::Update();
}

//========================
//描画
//========================
void CButton2D::Draw(void)
{
	//親処理
	CObject2D::Draw();
}

//========================
//生成
//========================
CButton2D * CButton2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority)
{
	CButton2D* pButton2D = nullptr;

	if (pButton2D == nullptr)
	{
		//オブジェクト2Dの生成
		pButton2D = new CButton2D(pos, rot, fWidth, fHeight);

		//初期化
		pButton2D->Init();

		return pButton2D;
	}
	else
	{
		return nullptr;
	}
}

//**************************************************
//3Dボタン
//**************************************************
//========================
//コンストラクタ
//========================
CButton3D::CButton3D()
{
	//値クリア
	m_bHold = false;
	m_bPress = false;
	m_bTrigger = false;
	m_bRelease = false;
	m_bRepeate = false;
}

//========================
//コンストラクタ（オーバーロード）
//========================
CButton3D::CButton3D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority)
	: CObject3D(pos, rot, fWidth, fHeight, nPriority)
{
	//値クリア
	m_bHold = false;
	m_bPress = false;
	m_bTrigger = false;
	m_bRelease = false;
	m_bRepeate = false;
}

//========================
//デストラクタ
//========================
CButton3D::~CButton3D()
{
}

//========================
//初期化
//========================
HRESULT CButton3D::Init(void)
{
	//親処理
	CObject3D::Init();

	return S_OK;
}

//========================
//終了
//========================
void CButton3D::Uninit(void)
{
	//親処理
	CObject3D::Uninit();
}

//========================
//更新
//========================
void CButton3D::Update(void)
{
	//ボタン処理
	CInputMouse* mouse = CManager::GetInputMouse();	//マウス取得

	//マウス取得
	if (mouse != nullptr)
	{
		D3DXVECTOR3 buttonPos = GetPos();			//ボタン位置取得
		D3DXVECTOR3 buttonRot = GetRot();			//ボタン角度取得
		float fWidth = GetWidth();					//ボタン幅取得
		float fDepth = GetDepth();					//ボタン高さ取得
		D3DXVECTOR3 cursorPos = mouse->GetPos();	//カーソル位置取得

		//オブジェクト選択（0.0～1.0）
		D3DXVECTOR3 posNear = mouse->ConvertClickPosToWorld(0.0f);
		D3DXVECTOR3 posFar = mouse->ConvertClickPosToWorld(1.0f);

		//頂点計算
		D3DXVECTOR3 pos0 = D3DXVECTOR3(-fWidth * 0.5f, 0.0f, fDepth * 0.5f);
		D3DXVECTOR3 pos1 = D3DXVECTOR3(fWidth * 0.5f, 0.0f, fDepth * 0.5f);
		D3DXVECTOR3 pos2 = D3DXVECTOR3(-fWidth * 0.5f, 0.0f, -fDepth * 0.5f);
		D3DXVECTOR3 pos3 = D3DXVECTOR3(fWidth * 0.5f, 0.0f, -fDepth * 0.5f);
		pos0 = RotateVtx(pos0, buttonRot, buttonPos);
		pos1 = RotateVtx(pos1, buttonRot, buttonPos);
		pos2 = RotateVtx(pos2, buttonRot, buttonPos);
		pos3 = RotateVtx(pos3, buttonRot, buttonPos);

		float fDust;
		D3DXVECTOR3 nor = (posFar - posNear);
		D3DXVec3Normalize(&nor, &nor);
		if (D3DXIntersectTri(&pos2, &pos1, &pos0, &posNear, &nor, &fDust, &fDust, &fDust) == true ||
			D3DXIntersectTri(&pos1, &pos2, &pos3, &posNear, &nor, &fDust, &fDust, &fDust) == true)
		{//範囲内
			m_bHold = true;										//かざし
			m_bPress = mouse->GetPress(MOUSE_CLICK_LEFT);		//プレス
			m_bTrigger = mouse->GetTrigger(MOUSE_CLICK_LEFT);	//トリガー
			m_bRelease = mouse->GetRelease(MOUSE_CLICK_LEFT);	//リリース
			m_bRepeate = mouse->GetRepeate(MOUSE_CLICK_LEFT);	//リピート
		}
		else
		{//範囲外
			m_bHold = false;	//かざし
			m_bPress = false;	//プレス
			m_bTrigger = false;	//トリガー
			m_bRelease = false;	//リリース
			m_bRepeate = false;	//リピート
		}
	}
	else
	{//マウスが取得できない
		m_bHold = false;	//かざし
		m_bPress = false;	//プレス
		m_bTrigger = false;	//トリガー
		m_bRelease = false;	//リリース
		m_bRepeate = false;	//リピート
	}

	//親処理
	CObject3D::Update();
}

//========================
//描画
//========================
void CButton3D::Draw(void)
{
	//親処理
	CObject3D::Draw();
}

//========================
//生成
//========================
CButton3D * CButton3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	CButton3D* pButton3D = nullptr;

	if (pButton3D == nullptr)
	{
		//オブジェクト2Dの生成
		pButton3D = new CButton3D(pos, rot, fWidth, fHeight);

		//初期化
		pButton3D->Init();

		return pButton3D;
	}
	else
	{
		return nullptr;
	}
}