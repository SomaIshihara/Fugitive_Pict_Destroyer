//==========================================
//
//フェード表示プログラム[fade.cpp]
//Author:石原颯馬
//
//==========================================
#include "manager.h"
#include "renderer.h"
#include "fade.h"

//マクロ
#define FADE_SPEED	(0.032f)	//フェードの速さ

//========================
//コンストラクタ
//========================
CFade::CFade()
{
	m_fade = FADE_NONE;
	m_modeNext = CScene::MODE_MAX;
}

//========================
//デストラクタ
//========================
CFade::~CFade()
{
}

//========================
//フェード初期化処理
//========================
HRESULT CFade::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxbuff,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_2D *pVtx;	//設定用ポインタ
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//位置（全面なので固定）
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);

	//テクスチャ座標（一応）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);

	//頂点バッファをアンロック
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	m_fade = FADE_NONE;	//フェードなし
}

//========================
//フェード終了処理
//========================
void CFade::Uninit(void)
{
	if (m_pVtxbuff != nullptr)
	{//破棄
		m_pVtxbuff->Release();
		m_pVtxbuff = nullptr;
	}

	delete this;
}

//========================
//フェード更新処理
//========================
void CFade::Update(void)
{
	VERTEX_2D *pVtx;					//設定用ポインタ

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_fAlpha -= FADE_SPEED;
			if (m_fAlpha <= 0.0f)
			{
				m_fAlpha = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_fAlpha += FADE_SPEED;
			if (m_fAlpha >= 1.0f)
			{
				m_fAlpha = 1.0f;
				m_fade = FADE_IN;
				CManager::SetMode(m_modeNext);
			}
		}
	}

	//頂点バッファのロックと頂点情報へのポインタを取得
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		assert(false);
	}

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha);
	pVtx[1].col = D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha);
	pVtx[2].col = D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha);
	pVtx[3].col = D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha);

	//頂点バッファをアンロック
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		assert(false);
	}
}

//========================
//フェード描画処理
//========================
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetTexture();						//テクスチャオブジェクト取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxbuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, nullptr);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//フェード生成処理
//========================
CFade* CFade::Create(void)
{
	CFade* pFade = NULL;

	if (pFade == NULL)
	{
		//フェードの生成
		pFade = new CFade;

		//初期化
		pFade->Init();

		return pFade;
	}
	else
	{
		return NULL;
	}
}

//========================
//次のシーン設定処理
//========================
void CFade::SetNext(CScene::MODE sceneNext)
{
	m_modeNext = sceneNext;
	m_fade = FADE_IN;
	m_fAlpha = 0.0f;
}