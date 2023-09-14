//==========================================
//
//フェード表示プログラム[fade.cpp]
//Author:石原颯馬
//
//==========================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "fade.h"

//静的メンバ変数
const float CFade::FADE_SPEED = 0.032f;

//========================
//コンストラクタ
//========================
CFade::CFade() : CBG(PRIORITY_FADE)
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
	CBG::Init();
	m_fade = FADE_NONE;	//フェードなし
	return S_OK;
}

//========================
//フェード終了処理
//========================
void CFade::Uninit(void)
{
	CBG::Uninit();
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
	else
	{//フェード終了
		Uninit();
		return;
	}

	//色変える
	CObject2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

	CBG::Update();
}

//========================
//フェード描画処理
//========================
void CFade::Draw(void)
{
	CBG::Draw();
}

//========================
//フェード生成処理
//========================
CFade* CFade::Create(CScene::MODE sceneNext)
{
	CFade* pFade = nullptr;

	if (pFade == nullptr)
	{
		//フェードの生成
		pFade = new CFade;

		//初期化
		pFade->Init();

		//テクスチャやっぱいる
		pFade->BindTexture(CTexture::PRELOAD_FADE);

		//ちかちか防止
		pFade->SetCol(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f));

		//次のシーン設定
		pFade->SetNext(sceneNext);

		return pFade;
	}
	else
	{
		return nullptr;
	}
}

//========================
//次のシーン設定処理
//========================
void CFade::SetNext(CScene::MODE sceneNext)
{
	m_modeNext = sceneNext;
	m_fade = FADE_OUT;
	m_fAlpha = 0.0f;
}