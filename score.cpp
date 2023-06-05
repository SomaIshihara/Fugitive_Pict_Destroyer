//======================================================
//
//スコア処理[score.cpp]
//Author:石原颯馬
//
//======================================================
#include "multiplebg.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "object.h"
#include "score.h"
#include "number.h"

//静的メンバ変数
PatternTexture CScore::m_patTexture = { NULL,0,0 };

//=================================
//コンストラクタ（デフォルト）
//=================================
CScore::CScore()
{
	//値クリア
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fOneWidth = FLOAT_ZERO;
	m_fOneHeight = FLOAT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CScore::CScore(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight)
{
	//値設定
	m_pos = pos;
	m_rot = rot;
	m_fOneWidth = fOneWidth;
	m_fOneHeight = fOneHeight;
}

//=================================
//デストラクタ
//=================================
CScore::~CScore()
{
}

//=================================
//初期化
//=================================
HRESULT CScore::Init(void)
{
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト初期化
		m_pNumber[cnt] = NULL;
	}

	//スコア設定
	Set(0);

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CScore::Uninit(void)
{
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト終了
		if (m_pNumber[cnt] != NULL)
		{//大丈夫。中身はある
			m_pNumber[cnt]->Uninit();
		}
	}

	//スコアオブジェクト破棄
	Release();
}

//=================================
//更新
//=================================
void CScore::Update(void)
{
#ifdef _DEBUG
	//[debug]数字設定
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		Set(12345678);	//デバッグ用だからこれでいいよね
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{
		Add(100);	//デバッグ用だからこれでいいよね
	}
#endif // DEBUG
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト更新
		if (m_pNumber[cnt] != NULL)
		{//大丈夫。中身はある
			//オブジェクト2Dの更新処理
			m_pNumber[cnt]->Update();
		}
	}

	//数字分割
	CutNumber();
}

//=================================
//描画
//=================================
void CScore::Draw(void)
{
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト描画
		if (m_pNumber[cnt] != NULL)
		{//大丈夫。中身はある
			m_pNumber[cnt]->Draw();
		}
	}
}

//=================================
//生成処理
//=================================
CScore* CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight)
{
	CScore* pScore = NULL;

	if (pScore == NULL)
	{
		//多重背景管理オブジェクト生成
		pScore = new CScore;

		//多重背景管理オブジェクト初期化
		pScore->Init();

		//背景用オブジェクト2D生成
		for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
		{//1枚分生成～テクスチャ設定
			//生成
			pScore->m_pNumber[cnt] = pScore->m_pNumber[cnt]->Create(pos + D3DXVECTOR3(-fOneWidth * cnt,0.0f,0.0f), rot, fOneWidth, fOneHeight);

			//初期化
			pScore->m_pNumber[cnt]->Init();

			//テクスチャ設定
			pScore->m_pNumber[cnt]->BindPatternTexture(m_patTexture);
		}

		return pScore;
	}
	else
	{
		return NULL;
	}
}

//=================================
//テクスチャ読み込み処理
//=================================
HRESULT CScore::Load(const char* pPath, int nPatWidth, int nPatHeight)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得

	//テクスチャ読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		pPath,
		&m_patTexture.pTexture)))
	{//失敗
		return E_FAIL;
	}

	//パターン幅高さ設定
	m_patTexture.nPatternWidth = nPatWidth;
	m_patTexture.nPatternHeight = nPatHeight;

	//成功
	return S_OK;
}

//=================================
//テクスチャ破棄処理
//=================================
void CScore::Unload(void)
{
	//テクスチャ破棄
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//1枚ずつ破棄
		if (m_patTexture.pTexture != NULL)
		{
			m_patTexture.pTexture->Release();
			m_patTexture.pTexture = NULL;
		}
	}
}

//=================================
//スコア設定処理
//=================================
void CScore::Set(const int nScore)
{
	//数字設定
	m_nScore = nScore;

	//数字分割
	CutNumber();
}

//=================================
//スコア加算処理
//=================================
void CScore::Add(const int nAdd)
{
	//数字加算
	m_nScore += nAdd;

	//数字分割
	CutNumber();
}

//=================================
//数字分割処理
//=================================
void CScore::CutNumber(void)
{
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクトに渡す
		if (m_pNumber[cnt] != NULL)
		{//大丈夫。中身はある
			m_pNumber[cnt]->SetNumber(m_nScore, cnt);
		}
	}
}