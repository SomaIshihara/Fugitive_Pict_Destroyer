//======================================================
//
//所持数処理[havenum.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "object.h"
#include "havenum.h"
#include "number.h"
#include "symbol.h"

//静的メンバ変数
const int CHaveNum::m_nSymbolX = 5;

//=================================
//コンストラクタ（デフォルト）
//=================================
CHaveNum::CHaveNum(int nPriority) : CObject(nPriority), m_nHaveNumDigit(1)
{
	//値クリア
	m_nIdxTexture = -1;
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_fOneWidth = CManager::FLOAT_ZERO;
	m_fOneHeight = CManager::FLOAT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CHaveNum::CHaveNum(const int nHaveNumDigit, int nPriority) : CObject(nPriority), m_nHaveNumDigit(nHaveNumDigit)
{
	//値設定
	m_nIdxTexture = -1;
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_fOneWidth = CManager::FLOAT_ZERO;
	m_fOneHeight = CManager::FLOAT_ZERO;
}

//=================================
//デストラクタ
//=================================
CHaveNum::~CHaveNum()
{
}

//=================================
//初期化
//=================================
HRESULT CHaveNum::Init(void)
{
	//数字オブジェクト動的確保
	m_ppNumber = new CNumber*[m_nHaveNumDigit];

	for (int cnt = 0; cnt < m_nHaveNumDigit; cnt++)
	{//数字オブジェクト初期化
		m_ppNumber[cnt] = nullptr;
	}

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = CTexture::PRELOAD_NUMBER;

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CHaveNum::Uninit(void)
{
	for (int cnt = 0; cnt < m_nHaveNumDigit; cnt++)
	{//数字オブジェクト終了
		if (m_ppNumber[cnt] != nullptr)
		{//大丈夫。中身はある
			m_ppNumber[cnt]->Uninit();
		}
	}

	//数字オブジェクト配列破棄
	delete[] m_ppNumber;

	//タイムオブジェクト破棄
	Release();
}

//=================================
//更新
//=================================
void CHaveNum::Update(void)
{
	for (int cnt = 0; cnt < m_nHaveNumDigit; cnt++)
	{//数字オブジェクト更新
		if (m_ppNumber[cnt] != nullptr)
		{//大丈夫。中身はある
			//数字の更新処理
			m_ppNumber[cnt]->Update();
		}
	}

	//数字分割
	CutNumber();
}

//=================================
//描画
//=================================
void CHaveNum::Draw(void)
{
	//勝手にやってくれます
}

//=================================
//生成処理
//=================================
CHaveNum* CHaveNum::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, const int nHaveNumDigit,
	const int nIconTexNum)
{
	CHaveNum* pHaveNum = nullptr;

	if (pHaveNum == nullptr)
	{
		//所持数管理オブジェクト生成
		pHaveNum = new CHaveNum(nHaveNumDigit,5);

		//タイマー管理オブジェクト初期化
		pHaveNum->Init();

		//記号オブジェ生成
		CTexture* pTexture = CManager::GetTexture();
		int ntexNum = CTexture::PRELOAD_SYMBOL;

		//記号仮置き
		CSymbol* pSymbol;

		//数字オブジェ生成
		int cnt;
		for (cnt = 0; cnt < nHaveNumDigit; cnt++)
		{//1枚分生成～テクスチャ設定（右から）
			//生成
			pHaveNum->m_ppNumber[cnt] = CNumber::Create(pos + D3DXVECTOR3(-fOneWidth * cnt,0.0f,0.0f), rot, fOneWidth, fOneHeight);

			//初期化
			pHaveNum->m_ppNumber[cnt]->Init();

			//テクスチャ設定
			pHaveNum->m_ppNumber[cnt]->BindTexture(pHaveNum->m_nIdxTexture);
		}

		//「×」
		pSymbol = CSymbol::Create(pos + D3DXVECTOR3(-fOneWidth * cnt, 0.0f, 0.0f), rot, fOneWidth, fOneHeight);
		pSymbol->Init();
		pSymbol->BindTexture(ntexNum);
		pSymbol->SetSymbol(m_nSymbolX);
		cnt++;	//アイコンの配置目安

		//アイコン
		CObject2D* pIcon = CObject2D::Create(pos + D3DXVECTOR3(-fOneWidth * cnt, 0.0f, 0.0f), CManager::VEC3_ZERO, fOneHeight, fOneHeight,PRIORITY_UI);
		pIcon->Init();
		pIcon->BindTexture(nIconTexNum);

		return pHaveNum;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//数字分割処理
//=================================
void CHaveNum::CutNumber(void)
{
	for (int cnt = 0; cnt < m_nHaveNumDigit; cnt++)
	{//数字オブジェクトに渡す
		if (m_ppNumber[cnt] != nullptr)
		{//大丈夫。中身はある
			m_ppNumber[cnt]->SetNumber(m_nHaveNum, cnt);
		}
	}
}