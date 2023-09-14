//======================================================
//
//レベル処理[level.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "object.h"
#include "level.h"
#include "number.h"
#include "symbol.h"

//静的メンバ変数
const int CLevel::m_nSymbolX = 5;

//=================================
//コンストラクタ（デフォルト）
//=================================
CLevel::CLevel(int nPriority) : CObject(nPriority), m_nLevelDigit(1)
{
	//値クリア
	m_nIdxTexture = -1;
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_fOneWidth = CManager::FLOAT_ZERO;
	m_fOneHeight = CManager::FLOAT_ZERO;
	m_nLevel = CManager::INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CLevel::CLevel(const int nLevelDigit, int nPriority) : CObject(nPriority), m_nLevelDigit(nLevelDigit)
{
	//値設定
	m_nIdxTexture = -1;
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_fOneWidth = CManager::FLOAT_ZERO;
	m_fOneHeight = CManager::FLOAT_ZERO;
	m_nLevel = CManager::INT_ZERO;
}

//=================================
//デストラクタ
//=================================
CLevel::~CLevel()
{
}

//=================================
//初期化
//=================================
HRESULT CLevel::Init(void)
{
	//数字オブジェクト動的確保
	m_ppNumber = new CNumber*[m_nLevelDigit];

	for (int cnt = 0; cnt < m_nLevelDigit; cnt++)
	{//数字オブジェクト初期化
		m_ppNumber[cnt] = nullptr;
	}

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = CTexture::PRELOAD_NUMBER;
	m_nLevel = 1;

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CLevel::Uninit(void)
{
	for (int cnt = 0; cnt < m_nLevelDigit; cnt++)
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
void CLevel::Update(void)
{
	for (int cnt = 0; cnt < m_nLevelDigit; cnt++)
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
void CLevel::Draw(void)
{
	//勝手にやってくれます
}

//=================================
//生成処理
//=================================
CLevel* CLevel::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, const int nLevelDigit,
	const int nIconTexNum)
{
	CLevel* pLevel = nullptr;

	if (pLevel == nullptr)
	{
		//レベル管理オブジェクト生成
		pLevel = new CLevel(nLevelDigit,5);

		//タイマー管理オブジェクト初期化
		pLevel->Init();

		//記号オブジェ生成
		CTexture* pTexture = CManager::GetTexture();
		int ntexNum = CTexture::PRELOAD_SYMBOL;

		//記号仮置き
		CSymbol* pSymbol;

		//数字オブジェ生成
		int cnt;
		for (cnt = 0; cnt < nLevelDigit; cnt++)
		{//1枚分生成～テクスチャ設定（右から）
			//生成
			pLevel->m_ppNumber[cnt] = CNumber::Create(pos + D3DXVECTOR3(-fOneWidth * cnt,0.0f,0.0f), rot, fOneWidth, fOneHeight);

			//初期化
			pLevel->m_ppNumber[cnt]->Init();

			//テクスチャ設定
			pLevel->m_ppNumber[cnt]->BindTexture(pLevel->m_nIdxTexture);
		}

		//アイコン
		CObject2D* pIcon = CObject2D::Create(pos + D3DXVECTOR3(-fOneWidth * cnt, 0.0f, 0.0f), CManager::VEC3_ZERO, fOneHeight, fOneHeight,PRIORITY_UI);
		pIcon->Init();
		pIcon->BindTexture(nIconTexNum);

		return pLevel;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//数字分割処理
//=================================
void CLevel::CutNumber(void)
{
	for (int cnt = 0; cnt < m_nLevelDigit; cnt++)
	{//数字オブジェクトに渡す
		if (m_ppNumber[cnt] != nullptr)
		{//大丈夫。中身はある
			m_ppNumber[cnt]->SetNumber(m_nLevel, cnt);
		}
	}
}