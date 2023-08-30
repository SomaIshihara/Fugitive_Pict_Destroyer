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
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fOneWidth = FLOAT_ZERO;
	m_fOneHeight = FLOAT_ZERO;
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
		m_pNumberTenHundred[cnt] = NULL;
	}

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTextureNumber = pTexture->Regist("data\\TEXTURE\\tex_Score_01.png", 10, 1);
	m_nIdxtextureSymbol = pTexture->Regist("data\\TEXTURE\\tex_Symbol_01.png", 12, 1);

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
		if (m_pNumberTenHundred[cnt] != NULL)
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
	//建物オブジェクト全検索
	Set(0);
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//建物オブジェクト取得
		if (pBuilding != NULL)
		{//なんかある
			CXModel* pModel = CXModel::GetTop();
			int nModelNum = 0;
			while (pModel != NULL && pModel != pBuilding->GetModel())
			{
				pModel = pModel->GetNext();
				nModelNum++;
			}
			CBuilding::BuildingParam buildingParam = CBuilding::GetBuildingParam(nModelNum);		//建物パラメータ取得
			float fParcent = ((float)pBuilding->GetEndurance() / buildingParam.nEndurance);
			long long nScore = (1.0f - fParcent) * buildingParam.nValue;	//スコア算出
			Add(nScore);
		}
		else
		{//もうない
			break;
		}
	}

	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト更新
		if (m_pNumberHMillion[cnt] != NULL)
		{//大丈夫。中身はある
			//オブジェクト2Dの更新処理
			m_pNumberHMillion[cnt]->Update();
		}
	}
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト更新
		if (m_pNumberTenHundred[cnt] != NULL)
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
	m_pSymbolPic->Draw();
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト更新
		if (m_pNumberHMillion[cnt] != NULL)
		{//大丈夫。中身はある
			//オブジェクト2Dの更新処理
			m_pNumberHMillion[cnt]->Draw();
		}
	}
	m_pSymbolHMillion->Draw();
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト描画
		if (m_pNumberTenHundred[cnt] != NULL)
		{//大丈夫。中身はある
			m_pNumberTenHundred[cnt]->Draw();
		}
	}
	m_pSymbolTenHundred->Draw();
}

//=================================
//生成処理
//=================================
CScore* CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight)
{
	CScore* pScore = NULL;

	if (pScore == NULL)
	{
		//スコア管理オブジェクト生成
		pScore = new CScore;

		//スコア管理オブジェクト初期化
		pScore->Init();

		//生成

		//万
		pScore->m_pSymbolTenHundred = pScore->m_pSymbolPic->Create(pos, rot, fOneWidth, fOneHeight);	//数字8桁+記号2
		pScore->m_pSymbolTenHundred->Init();
		pScore->m_pSymbolTenHundred->BindTexture(pScore->m_nIdxtextureSymbol);
		pScore->m_pSymbolTenHundred->SetSymbol(CScore::TYPE_TEN_HUNDRED);

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
		pScore->m_pSymbolHMillion = pScore->m_pSymbolPic->Create(pos + D3DXVECTOR3(-fOneWidth * 5, 0.0f, 0.0f), rot, fOneWidth, fOneHeight);	//数字4桁+記号
		pScore->m_pSymbolHMillion->Init();
		pScore->m_pSymbolHMillion->BindTexture(pScore->m_nIdxtextureSymbol);
		pScore->m_pSymbolHMillion->SetSymbol(CScore::TYPE_HMILLION);

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
		pScore->m_pSymbolPic = CSymbol::Create(pos + D3DXVECTOR3(-fOneWidth * 10, 0.0f, 0.0f), rot, fOneWidth, fOneHeight);
		pScore->m_pSymbolPic->Init();
		pScore->m_pSymbolPic->BindTexture(pScore->m_nIdxtextureSymbol);
		pScore->m_pSymbolPic->SetSymbol(CScore::TYPE_PIC);

		return pScore;
	}
	else
	{
		return NULL;
	}
}

//=================================
//スコア設定処理
//=================================
void CScore::Set(const int nScore)
{
	//数字設定
	m_nScore = nScore;
}

//=================================
//スコア加算処理
//=================================
void CScore::Add(const int nAdd)
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
		if (m_pNumberTenHundred[cnt] != NULL)
		{//大丈夫。中身はある
			m_pNumberHMillion[cnt]->SetNumber(m_nScore, cnt + SCORE_INVISIBLE + 4);
		}
	}
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクトに渡す
		if (m_pNumberTenHundred[cnt] != NULL)
		{//大丈夫。中身はある
			m_pNumberTenHundred[cnt]->SetNumber(m_nScore, cnt + SCORE_INVISIBLE);
		}
	}
}