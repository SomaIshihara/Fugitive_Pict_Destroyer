//======================================================
//
//マルチ背景（2D）処理[bg.cpp]
//Author:石原颯馬
//
//======================================================
#include "multiplebg.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"

//テクスチャパス
const char* c_apTexturePathMultiBG[MAX_MULTIPLE_BG] =
{
	"data\\TEXTURE\\bg100.png",
	"data\\TEXTURE\\bg101.png",
	"data\\TEXTURE\\bg102.png"
};


//=================================
//コンストラクタ（デフォルト）
//=================================
CMultipleBG::CMultipleBG(int nPriority)
{
	//テクスチャ番号クリア
	for (int cnt = 0; cnt < MAX_MULTIPLE_BG; cnt++)
	{
		m_aIdxTexture[cnt] = -1;
	}
}

//=================================
//デストラクタ
//=================================
CMultipleBG::~CMultipleBG()
{
}

//=================================
//初期化
//=================================
HRESULT CMultipleBG::Init(void)
{
	CTexture* pTexture = CManager::GetTexture();	//テクスチャオブジェクト
	for (int cnt = 0; cnt < MAX_MULTIPLE_BG; cnt++)
	{//背景用オブジェクト2D初期化とテクスチャ読み込み
		m_pObj2D[cnt] = NULL;
		//テクスチャ読み込み
		m_aIdxTexture[cnt] = pTexture->Regist(c_apTexturePathMultiBG[cnt]);
	}

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CMultipleBG::Uninit(void)
{
	for (int cnt = 0; cnt < MAX_MULTIPLE_BG; cnt++)
	{//背景用オブジェクト2D終了
		if (m_pObj2D[cnt] != NULL)
		{//大丈夫。中身はある
			m_pObj2D[cnt]->Uninit();
		}
	}

	//多重背景管理オブジェクト破棄
	Release();
}

//=================================
//更新
//=================================
void CMultipleBG::Update(void)
{
	for (int cnt = 0; cnt < MAX_MULTIPLE_BG; cnt++)
	{//背景用オブジェクト2D更新
		if (m_pObj2D[cnt] != NULL)
		{//大丈夫。中身はある
			//オブジェクト2Dの更新処理
			m_pObj2D[cnt]->Update();

			//座標設定
			m_aTexV[cnt] = fmodf(m_aTexV[cnt] + 1.0f - m_aSpeed[cnt], 1.0f);
			//テクスチャ設定
			D3DXVECTOR2 tex0, tex3;
			tex0 = D3DXVECTOR2(0.0f, m_aTexV[cnt]);
			tex3 = D3DXVECTOR2(1.0f, m_aTexV[cnt] + 1.0f);
			m_pObj2D[cnt]->SetTex(tex0, tex3);
		}
	}
}

//=================================
//描画
//=================================
void CMultipleBG::Draw(void)
{
	for (int cnt = 0; cnt < MAX_MULTIPLE_BG; cnt++)
	{//背景用オブジェクト2D描画
		if (m_pObj2D[cnt] != NULL)
		{//大丈夫。中身はある
			m_pObj2D[cnt]->Draw();
		}
	}
}

//=================================
//生成処理
//=================================
CMultipleBG* CMultipleBG::Create(float fSpeed0, float fSpeed1, float fSpeed2)
{
	CMultipleBG* pObjMultipleBG = NULL;

	if (pObjMultipleBG == NULL)
	{
		//多重背景管理オブジェクト生成
		pObjMultipleBG = new CMultipleBG;

		//多重背景管理オブジェクト初期化
		pObjMultipleBG->Init();

		//背景用オブジェクト2D生成
		for (int cnt = 0; cnt < MAX_MULTIPLE_BG; cnt++)
		{//1枚分生成～テクスチャ設定
			//生成
			pObjMultipleBG->m_pObj2D[cnt] = pObjMultipleBG->m_pObj2D[cnt]->Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), VEC3_ZERO, 
				SCREEN_WIDTH, SCREEN_HEIGHT);

			//初期化
			pObjMultipleBG->m_pObj2D[cnt]->Init();

			//テクスチャ設定
			pObjMultipleBG->m_pObj2D[cnt]->BindTexture(pObjMultipleBG->m_aIdxTexture[cnt]);
		}

		//スピード設定
		pObjMultipleBG->m_aSpeed[0] = fSpeed0;
		pObjMultipleBG->m_aSpeed[1] = fSpeed1;
		pObjMultipleBG->m_aSpeed[2] = fSpeed2;

		return pObjMultipleBG;
	}
	else
	{
		return NULL;
	}
}