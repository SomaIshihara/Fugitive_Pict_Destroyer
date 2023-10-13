//======================================================
//
//スコア処理[score.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "object.h"
#include "score.h"
#include "number.h"
#include "symbol.h"
#include "building.h"
#include "xmodel.h"

//マクロ
#define SCORE_INVISIBLE	(4)	//何桁分かを非表示にする

//=================================
//コンストラクタ（デフォルト）
//=================================
CScore::CScore(int nPriority) : CObject(nPriority)
{
	//値クリア
	m_nIdxTextureNumber = -1;
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_fOneWidth = CManager::FLOAT_ZERO;
	m_fOneHeight = CManager::FLOAT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CScore::CScore(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, int nPriority) : CObject(nPriority)
{
	//値設定
	m_nIdxTextureNumber = -1;
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
		m_pNumberTenHundred[cnt] = nullptr;
	}

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTextureNumber = CTexture::PRELOAD_NUMBER;
	m_nIdxtextureSymbol = CTexture::PRELOAD_SYMBOL;

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
		if (m_pNumberTenHundred[cnt] != nullptr)
		{//大丈夫。中身はある
			m_pNumberTenHundred[cnt]->Uninit();
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
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト更新
		if (m_pNumberHMillion[cnt] != nullptr)
		{//大丈夫。中身はある
			//オブジェクト2Dの更新処理
			m_pNumberHMillion[cnt]->Update();
		}
	}
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト更新
		if (m_pNumberTenHundred[cnt] != nullptr)
		{//大丈夫。中身はある
			//オブジェクト2Dの更新処理
			m_pNumberTenHundred[cnt]->Update();
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
	//描画は勝手にやってくれます
}

//=================================
//生成処理
//=================================
CScore* CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight)
{
	CScore* pScore = nullptr;

	if (pScore == nullptr)
	{
		//スコア管理オブジェクト生成
		pScore = new CScore;

		//スコア管理オブジェクト初期化
		pScore->Init();

		//生成
		CSymbol* pSymbol;

		//万
		pSymbol = CSymbol::Create(pos, rot, fOneWidth, fOneHeight);
		pSymbol->Init();
		pSymbol->BindTexture(pScore->m_nIdxtextureSymbol);
		pSymbol->SetSymbol(CScore::TYPE_TEN_HUNDRED);

		//数字8桁+記号2
		for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
		{
			//生成
			pScore->m_pNumberTenHundred[cnt] = pScore->m_pNumberTenHundred[cnt]->Create(pos + D3DXVECTOR3(-fOneWidth * (cnt + 1), 0.0f, 0.0f), rot, fOneWidth, fOneHeight);	//数字4桁+記号2

			//初期化
			pScore->m_pNumberTenHundred[cnt]->Init();

			//テクスチャ設定
			pScore->m_pNumberTenHundred[cnt]->BindTexture(pScore->m_nIdxTextureNumber);
		}
		
		//億
		pSymbol = CSymbol::Create(pos + D3DXVECTOR3(-fOneWidth * 5, 0.0f, 0.0f), rot, fOneWidth, fOneHeight);	//数字4桁+記号
		pSymbol->Init();
		pSymbol->BindTexture(pScore->m_nIdxtextureSymbol);
		pSymbol->SetSymbol(CScore::TYPE_HMILLION);

		for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
		{
			//生成
			pScore->m_pNumberHMillion[cnt] = CNumber::Create(pos + D3DXVECTOR3(-fOneWidth * (cnt + 6), 0.0f, 0.0f), rot, fOneWidth, fOneHeight);

			//初期化
			pScore->m_pNumberHMillion[cnt]->Init();

			//テクスチャ設定
			pScore->m_pNumberHMillion[cnt]->BindTexture(pScore->m_nIdxTextureNumber);
		}

		//通貨記号
		pSymbol = CSymbol::Create(pos + D3DXVECTOR3(-fOneWidth * 10, 0.0f, 0.0f), rot, fOneWidth, fOneHeight);
		pSymbol->Init();
		pSymbol->BindTexture(pScore->m_nIdxtextureSymbol);
		pSymbol->SetSymbol(CScore::TYPE_PIC);

		return pScore;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//スコア設定処理
//=================================
void CScore::Set(const long long nScore)
{
	//数字設定
	m_nScore = nScore;
}

//=================================
//スコア加算処理
//=================================
void CScore::Add(const long long nAdd)
{
	//数字加算
	m_nScore += nAdd;
}

//=================================
//数字分割処理
//=================================
void CScore::CutNumber(void)
{
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクトに渡す
		if (m_pNumberTenHundred[cnt] != nullptr)
		{//大丈夫。中身はある
			m_pNumberHMillion[cnt]->SetNumber(m_nScore, cnt + SCORE_INVISIBLE + 4);
		}
	}
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクトに渡す
		if (m_pNumberTenHundred[cnt] != nullptr)
		{//大丈夫。中身はある
			m_pNumberTenHundred[cnt]->SetNumber(m_nScore, cnt + SCORE_INVISIBLE);
		}
	}
}