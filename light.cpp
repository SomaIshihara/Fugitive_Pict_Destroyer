//==========================================
//
//ライト関係プログラム[light.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"

//========================
//コンストラクタ
//========================
CLight::CLight()
{
	//ライト情報クリア
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9));
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));
	ZeroMemory(&m_aLight[2], sizeof(D3DLIGHT9));
	ZeroMemory(&m_aLight[3], sizeof(D3DLIGHT9));
}

//========================
//デストラクタ
//========================
CLight::~CLight()
{

}

//========================
//初期化処理
//========================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得
	D3DXVECTOR3 vecDir;	//設定用

	//ライト情報クリア
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9));
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));
	ZeroMemory(&m_aLight[2], sizeof(D3DLIGHT9));
	ZeroMemory(&m_aLight[3], sizeof(D3DLIGHT9));

	//ライト種類設定
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL;
	m_aLight[3].Type = D3DLIGHT_DIRECTIONAL;

	//ライト拡散光設定
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_aLight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_aLight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_aLight[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向
	//0
	vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);

	m_aLight[0].Direction = vecDir;

	//1
	vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);

	m_aLight[1].Direction = vecDir;

	//2
	vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);

	m_aLight[2].Direction = vecDir;

	//3
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	D3DXVec3Normalize(&vecDir, &vecDir);

	m_aLight[3].Direction = vecDir;

	//ライト設定
	pDevice->SetLight(0, &m_aLight[0]);
	pDevice->SetLight(1, &m_aLight[1]);
	pDevice->SetLight(2, &m_aLight[2]);

	//ライト有効化
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);

	//できた
	return S_OK;
}

//========================
//終了処理
//========================
void CLight::Uninit(void)
{
	//無
}

//========================
//更新処理
//========================
void CLight::Update(void)
{
	//無
}