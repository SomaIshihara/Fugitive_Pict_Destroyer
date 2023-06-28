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
#include "building.h"

//マクロ
#define SCORE_INVISIBLE	(4)	//何桁分かを非表示にする

//静的メンバ変数
long long CScore::m_nScore = 0;

//=================================
//コンストラクタ（デフォルト）
//=================================
CScore::CScore(int nPriority) : CObject(nPriority)
{
	//値クリア
	m_nIdxTexture = -1;
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
	m_nIdxTexture = -1;
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
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\tex_Score_01.png", 10, 1);

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
#ifdef _DEBUG
	//[debug]数字設定
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		Set(12345678);	//デバッグ用だからこれでいいよね
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true)
	{
		Add(100);	//デバッグ用だからこれでいいよね
	}
#endif // DEBUG

	//建物オブジェクト全検索
	Set(0);
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//建物オブジェクト取得
		if (pBuilding != NULL)
		{//なんかある
			CBuilding::BuildingParam buildingParam = CBuilding::GetBuildingParam(pBuilding->GetModelIdx());		//建物パラメータ取得
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
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//数字オブジェクト描画
		if (m_pNumberTenHundred[cnt] != NULL)
		{//大丈夫。中身はある
			m_pNumberTenHundred[cnt]->Draw();
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
			pScore->m_pNumberTenHundred[cnt] = pScore->m_pNumberTenHundred[cnt]->Create(pos + D3DXVECTOR3(-fOneWidth * cnt,0.0f,0.0f), rot, fOneWidth, fOneHeight);

			//初期化
			pScore->m_pNumberTenHundred[cnt]->Init();

			//テクスチャ設定
			pScore->m_pNumberTenHundred[cnt]->BindTexture(pScore->m_nIdxTexture);
		}

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
			m_pNumberTenHundred[cnt]->SetNumber(m_nScore, cnt + SCORE_INVISIBLE);
		}
	}
}