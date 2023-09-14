//======================================================
//
//テクスチャ保存クラス[texture.cpp]
//Author:石原颯馬
//
//======================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include <stdio.h>

//静的メンバ変数
int CTexture::m_nNumAll = 0;

//マクロ
#define TEXTURE_CODE_LENGTH	(128)	//テクスチャ読み込み文字の長さ
#define CODE_START			"SCRIPT_START"
#define CODE_END			"SCRIPT_END"
#define CODE_START_LOAD		"START_LOAD"
#define CODE_END_LOAD		"END_LOAD"
#define CODE_TEXTURE_NAME	"TEXTURE_NAME"
#define CODE_PATTERN_WIDTH	"PATTERN_WIDTH"
#define CODE_PATTERN_HEIGHT	"PATTERN_HEIGHT"

//=================================
//コンストラクタ
//=================================
CTexture::CTexture()
{
	for (int cnt = 0; cnt < MAX_TEXTURE; cnt++)
	{//全部空っぽにする
		m_apTexture[cnt].pTexture = nullptr;
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
HRESULT CTexture::Load(const char* pPath)
{
	FILE* pFile;
	char aCode[TEXTURE_CODE_LENGTH];
	bool bRead = false;
	bool bLoad = false;
	char aPath[TEXTURE_CODE_LENGTH];
	int nPatWidth, nPatHeight;

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//読み込みファイル設定
	pFile = fopen(pPath, "r");

	//敵配置情報を取得
	if (pFile != nullptr)
	{
		while (1)
		{
			fgets(&aCode[0], TEXTURE_CODE_LENGTH, pFile);

			//コメントアウトチェック
			char *pCharPos = strchr(&aCode[0], '#');
			if (pCharPos != nullptr)
			{//strchrの返り値がぬるぽではない
				*pCharPos = '\0';
			}

			//タブ消去
			while (aCode[0] == '\t')
			{
				char aCodeBackup[TEXTURE_CODE_LENGTH];
				strcpy(&aCodeBackup[0], &aCode[0]);
				strcpy(&aCode[0], &aCodeBackup[1]);
			}

			//文字列チェック
			if (strncmp(&aCode[0], CODE_START, sizeof CODE_START - 1) == 0)
			{//読み取り開始
				bRead = true;
			}
			else if (strncmp(&aCode[0], CODE_END, sizeof CODE_END - 1) == 0)
			{//読み取り終了
				bRead = false;
				break;
			}
			else if (aCode[0] == EOF)
			{//EOFかもしれない
				if (feof(pFile))
				{//いや、これはEOFだ
					bRead = false;
					break;
				}
			}
			else if (bRead == true)
			{//読み取り
				if (bLoad == true)
				{
					if (strncmp(&aCode[0], CODE_END_LOAD, sizeof CODE_END_LOAD - 1) == 0)
					{
						//テクスチャ読み込み
						D3DXCreateTextureFromFile(pDevice, &aPath[0],
							&m_apTexture[m_nNumAll].pTexture);

						//ファイルパスコピー
						strcpy(&m_aFilepath[m_nNumAll][0], &aPath[0]);

						//パターン幅高さ代入
						m_apTexture[m_nNumAll].nPatternWidth = nPatWidth;
						m_apTexture[m_nNumAll].nPatternHeight = nPatHeight;

						//総数増やす
						m_nNumAll++;
					}
					else if (strncmp(&aCode[0], CODE_TEXTURE_NAME, sizeof CODE_TEXTURE_NAME - 1) == 0)
					{
						char* pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//パス取得
						pSprit = strtok(nullptr, " =\n");

						//タブ文字が入っているところを消す
						char *pCharPos = strchr(pSprit, '\t');
						if (pCharPos != nullptr)
						{//strchrの返り値がぬるぽではない
							*pCharPos = '\0';
						}

						//文字列コピー
						strcpy(&aPath[0], pSprit);
					}
					else if (strncmp(&aCode[0], CODE_PATTERN_WIDTH, sizeof CODE_PATTERN_WIDTH - 1) == 0)
					{
						char* pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//データ
						pSprit = strtok(nullptr, " =\n");
						nPatWidth = atoi(pSprit);
						
					}
					else if (strncmp(&aCode[0], CODE_PATTERN_HEIGHT, sizeof CODE_PATTERN_HEIGHT - 1) == 0)
					{
						char* pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//データ
						pSprit = strtok(nullptr, " =\n");
						nPatHeight = atoi(pSprit);
					}
				}
				else
				{
					if (strncmp(&aCode[0], CODE_START_LOAD, sizeof CODE_START_LOAD - 1) == 0)
					{
						bLoad = true;
					}
				}
			}
		}
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=================================
//全部破棄
//=================================
void CTexture::Unload(void)
{
	for (int cnt = 0; cnt < m_nNumAll; cnt++)
	{//全部破棄
		if (m_apTexture[cnt].pTexture != nullptr)
		{
			m_apTexture[cnt].pTexture->Release();
			m_apTexture[cnt].pTexture = nullptr;
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
		return nullptr;	//返すものなんてない
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

