//======================================================
//
//テクスチャ保存クラス[texture.cpp]
//Author:石原颯馬
//
//======================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//静的メンバ変数
int CTexture::m_nNumAll = 0;

//=================================
//コンストラクタ
//=================================
CTexture::CTexture()
{
	for (int cnt = 0; cnt < MAX_TEXTURE; cnt++)
	{//全部空っぽにする
		m_apTexture[cnt].pTexture = NULL;
		m_apTexture[cnt].nPatternWidth = 0;
		m_apTexture[cnt].nPatternHeight = 0;
	}
}

//=================================
//デストラクタ
//=================================
CTexture::~CTexture()
{
}

//=================================
//あらかじめ読み込み
//=================================
HRESULT CTexture::Load(void)
{
	return S_OK;
}

//=================================
//全部破棄
//=================================
void CTexture::Unload(void)
{
	for (int cnt = 0; cnt < m_nNumAll; cnt++)
	{//全部破棄
		if (m_apTexture[cnt].pTexture != NULL)
		{
			m_apTexture[cnt].pTexture->Release();
			m_apTexture[cnt].pTexture = NULL;
		}
	}
	m_nNumAll = 0;	//個数リセット
}

//=================================
//追加分登録
//=================================
int CTexture::Regist(const char * pPath, const int nPatWidth, const int nPatHeight)
{
	//重複チェック
	for (int cnt = 0; cnt < m_nNumAll; cnt++)
	{//ある分だけ回す
		if (strcmp(&m_aFilepath[cnt][0], pPath) == 0)
		{//一致した
			return cnt;
		}
	}

	//新規登録
	if (m_nNumAll < MAX_TEXTURE)
	{//テクスチャ保存限界以内
		 //デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, pPath,
			&m_apTexture[m_nNumAll].pTexture);

		//ファイルパスコピー
		strcpy(&m_aFilepath[m_nNumAll][0], pPath);

		//パターン幅高さ代入
		m_apTexture[m_nNumAll].nPatternWidth = nPatWidth;
		m_apTexture[m_nNumAll].nPatternHeight = nPatHeight;

		//総数増やす
		m_nNumAll++;

		return m_nNumAll - 1;
	}
	else
	{//もう入らない
		return -1;
	}
}

//=================================
//テクスチャ取得
//=================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (m_nNumAll > nIdx && nIdx >= 0)
	{//テクスチャ総数以内
		return m_apTexture[nIdx].pTexture;	//テクスチャポインタ返す
	}
	else
	{//そんなものはない
		return NULL;	//返すものなんてない
	}
}

//=================================
//テクスチャパターン幅取得
//=================================
int CTexture::GetPatWidth(int nIdx)
{
	if (m_nNumAll > nIdx && nIdx >= 0)
	{//テクスチャ総数以内
		return m_apTexture[nIdx].nPatternWidth;	//テクスチャポインタ返す
	}
	else
	{//そんなものはない
		return 0;	//返すものなんてない
	}
}

//=================================
//テクスチャパターン高さ取得
//=================================
int CTexture::GetPatHeight(int nIdx)
{
	if (m_nNumAll > nIdx && nIdx >= 0)
	{//テクスチャ総数以内
		return m_apTexture[nIdx].nPatternHeight;	//テクスチャポインタ返す
	}
	else
	{//そんなものはない
		return 0;	//返すものなんてない
	}
}

