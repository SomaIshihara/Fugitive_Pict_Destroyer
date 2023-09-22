//======================================================
//
//ランキングシーン[ranking.cpp]
//Author:石原颯馬
//
//======================================================
#include "precompile.h"
#include "texture.h"
#include "ranking.h"
#include "camera.h"
#include "input.h"
#include "fade.h"
#include "bg.h"
#include "number.h"
#include "symbol.h"
#include "score.h"
#include "sound.h"

//静的メンバ変数
const int CRanking::MAX_RANK = 10;
const int CRanking::RANK_DISP_X = 2;
const int CRanking::RANK_DISP_Y = 5;
const D3DXVECTOR3 CRanking::RANK_START_POS = D3DXVECTOR3(50.0f, 209.0f, 0.0f);
const float CRanking::RANK_X_DISTANCE = 640.0f;
const float CRanking::RANK_Y_DISTANCE = 90.0f;
const float CRanking::X_ONE_WIDTH = 40.0f;
const float CRanking::Y_ONE_HEIGHT = 72.0f;
const float CRanking::X_SCORE_DIST = 20.0f;
const int CRanking::SYMBOL_RANK_NUM = 8;

//=================================
//コンストラクタ
//=================================
CRanking::CRanking()
{
}

//=================================
//デストラクタ
//=================================
CRanking::~CRanking()
{
}

//=================================
//初期化
//=================================
HRESULT CRanking::Init(void)
{
	long long* pRanking = new long long[MAX_RANK];	//一応静的constも変数なので
	//カメラ位置リセット
	CManager::GetCamera()->ResetPos();

	//背景生成
	CBG* pBG = CBG::Create();
	pBG->BindTexture(CTexture::PRELOAD_FADE);
	pBG->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	//見出し生成
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 100.0f, 0.0f), CManager::VEC3_ZERO, 311.0f, 111.0f)->BindTexture(CTexture::PRELOAD_TITLE_RANK);

	//ランキング読み込み
	Load(pRanking);

	//ランキング表示に必要なオブジェクト生成
	for (int cntX = 0; cntX < RANK_DISP_X; cntX++)
	{
		for (int cntY = 0; cntY < RANK_DISP_Y; cntY++)
		{
			D3DXVECTOR3 posStart = RANK_START_POS + D3DXVECTOR3(RANK_X_DISTANCE * cntX, RANK_Y_DISTANCE * cntY, 0.0f);
			//順位（十）
			CNumber* pNumber = nullptr;
			pNumber = CNumber::Create(posStart, CManager::VEC3_ZERO, X_ONE_WIDTH, Y_ONE_HEIGHT);
			pNumber->BindTexture(CTexture::PRELOAD_NUMBER);
			pNumber->SetNumber((cntX * RANK_DISP_Y + cntY + 1), 1);

			//順位（一）
			pNumber = CNumber::Create(posStart + D3DXVECTOR3(X_ONE_WIDTH * 1,0.0f,0.0f), CManager::VEC3_ZERO, X_ONE_WIDTH, Y_ONE_HEIGHT);
			pNumber->BindTexture(CTexture::PRELOAD_NUMBER);
			pNumber->SetNumber((cntX * RANK_DISP_Y + cntY + 1), 0);

			//記号（位）
			CSymbol* pSymbol = nullptr;
			pSymbol = CSymbol::Create(posStart + D3DXVECTOR3(X_ONE_WIDTH * 2, 0.0f, 0.0f), CManager::VEC3_ZERO, X_ONE_WIDTH, Y_ONE_HEIGHT);
			pSymbol->BindTexture(CTexture::PRELOAD_SYMBOL);
			pSymbol->SetSymbol(SYMBOL_RANK_NUM);

			//スコア
			CScore* pScore = nullptr;
			pScore = CScore::Create(posStart + D3DXVECTOR3(X_ONE_WIDTH * 2 + X_SCORE_DIST + X_ONE_WIDTH * 11, 0.0f, 0.0f),
				CManager::VEC3_ZERO, X_ONE_WIDTH, Y_ONE_HEIGHT);
			pScore->Set(pRanking[cntX * RANK_DISP_Y + cntY]);
		}
	}

	delete[] pRanking;	//破棄

	return S_OK;
}

//=================================
//終了
//=================================
void CRanking::Uninit(void)
{
	CObject::ReleaseAll();
}

//=================================
//更新
//=================================
void CRanking::Update(void)
{
	CSound* pSound = CManager::GetSound();
	CInputMouse* pMouse = CManager::GetInputMouse();

	if (m_pFade == nullptr && pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
	{//タイトルに遷移
		m_pFade = CFade::Create(CScene::MODE_TITLE);
		pSound->Play(CSound::SOUND_LABEL_SE_BUTTON);
	}
}

//=================================
//描画
//=================================
void CRanking::Draw(void)
{
}

//=================================
//ランキング設定
//=================================
void CRanking::Set(const long long nScore)
{
	long long* pRanking = new long long[MAX_RANK];	//一応静的constも変数なので
	//ランキング読み込み
	Load(pRanking);

	//ソート
	if (pRanking[MAX_RANK - 1] < nScore)
	{//表示できる中で一番低い順位に入るか
		pRanking[MAX_RANK - 1] = nScore;	//とりあえず入れる

		for (int cnt = MAX_RANK - 1; cnt > 0; cnt--)
		{
			if (pRanking[cnt] > pRanking[cnt - 1])
			{//入れ替え
				long long nTemp = pRanking[cnt - 1];
				pRanking[cnt - 1] = pRanking[cnt];
				pRanking[cnt] = nTemp;
			}
		}
	}

	//ランキング書き込み
	Save(pRanking);

	delete[] pRanking;	//破棄
}

//=================================
//ランキング読み込み
//=================================
void CRanking::Load(long long* pRanking)
{
	FILE* pFile = nullptr;
	pFile = fopen("data\\ranking.bin", "rb");

	if (pFile != nullptr)
	{//ランキングファイルがある（読み込み）
		fread(&pRanking[0], sizeof(long long), MAX_RANK, pFile);
		fclose(pFile);	//閉じる
	}
	else
	{//ランキングファイルがない（新規作成）
		for (int cnt = 0; cnt < MAX_RANK; cnt++)
		{
			pRanking[cnt] = 0;	//とりあえず0で埋める
		}
		Save(pRanking);
	}
}

//=================================
//ランキング書き込み
//=================================
void CRanking::Save(long long* pRanking)
{
	FILE* pFile = nullptr;
	pFile = fopen("data\\ranking.bin", "wb");

	assert(("書き込みモードで開けなかったよ！奇跡だね！", pFile != nullptr));

	//開けた体で進める
	fwrite(pRanking, sizeof(long long), MAX_RANK, pFile);	//書き込み

	fclose(pFile);	//閉じる
}
