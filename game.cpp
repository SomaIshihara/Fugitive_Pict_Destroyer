//======================================================
//
//ゲームシーン[game.cpp]
//Author:石原颯馬
//
//======================================================
#include "game.h"
#include "pause.h"
#include "result.h"
#include "player.h"
#include "meshField.h"
#include "slider.h"
#include "building.h"
#include "score.h"
#include "timer.h"
#include "objectX.h"
#include "pict.h"
#include "koban.h"
#include "item.h"
#include "meshsky.h"
#include "point.h"
#include "input.h"
#include "havenum.h"

//静的メンバ変数
CPlayer* CGame::m_pPlayer = nullptr;
CSlider* CGame::m_pSlider = nullptr;
CMeshField* CGame::m_pMeshField = nullptr;
CTimer* CGame::m_pTimer = nullptr;
CScore* CGame::m_pScore = nullptr;
CHaveNum* CGame::m_pHaveNum[];

//=================================
//コンストラクタ
//=================================
CGame::CGame()
{
	m_pPause = nullptr;
}

//=================================
//デストラクタ
//=================================
CGame::~CGame()
{
}

//=================================
//初期化
//=================================
HRESULT CGame::Init(void)
{
	m_pSlider = CSlider::Create(D3DXVECTOR3(100.0f, SCREEN_HEIGHT - 20.0f, 0.0f), 40.0f, 40.0f, 3);
	m_pPlayer = new CPlayer;

	//プレイヤー初期化
	if (FAILED(m_pPlayer->Init()))
	{
		return E_FAIL;
	}

	//仮オブジェ生成
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(-1280.0f, 0.0f, 1280.0f), VEC3_ZERO, 64.0f, 64.0f, 40, 40);

	//オブジェクト生成+初期化
	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 72.0f);
	m_pTimer->Set(120, CTimer::COUNT_DOWN);
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 40.0f, 64.0f);
	CObjectX* pAgit = CObjectX::Create(D3DXVECTOR3(600.0f,0.0f,0.0f), VEC3_ZERO, CManager::GetAgitModel());
	CPict::SetAgit(pAgit);
	CMeshSky::Create(VEC3_ZERO, VEC3_ZERO, 10000.0f, 8, 8);
	m_pHaveNum[0] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 100.0f, 0.0f), VEC3_ZERO, 60.0f, 72.0f, 2, 0);//アイコン番号仮
	m_pHaveNum[1] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 172.0f, 0.0f), VEC3_ZERO, 60.0f, 72.0f, 2, 0);//アイコン番号仮
	m_pHaveNum[2] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 244.0f, 0.0f), VEC3_ZERO, 60.0f, 72.0f, 5,0);//アイコン番号仮

	//仮
	CItemBullet::Create(D3DXVECTOR3(0.0f, 0.0f, 10.0f), VEC3_ZERO);

	//マップデータ読み込みと配置
	CObjectX::LoadData("data\\Fugitive_Pict_MapData_v120.ismd");

	//ポイント生成
	CPoint::Update();

	//交番パラメータ設定
	CKoban::SetKobanParam(300, 10);	//仮設定

	return S_OK;
}

//=================================
//終了
//=================================
void CGame::Uninit(void)
{
	CObject::ReleaseAll();

	//プレイヤー破棄
	if (m_pPlayer != nullptr)
	{//プレイヤー終了
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
	//リザルト破棄
	if (m_pResult != nullptr)
	{//プレイヤー終了
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
	}
	//ポーズ破棄（一応）
	if (m_pPause != nullptr)
	{//プレイヤー終了
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	m_pPlayer = nullptr;
	m_pSlider = nullptr;
	m_pMeshField = nullptr;
	m_pTimer = nullptr;
	m_pScore = nullptr;
}

//=================================
//更新
//=================================
void CGame::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//キーボード取得

	if (pKeyboard->GetTrigger(DIK_P) == true)
	{//ポーズ切り替え
		CManager::SetPause((CManager::GetPause() == true ? false : true));
	}

	if (CManager::GetPause() == true)
	{//ポーズしてる
		if (m_pPause == nullptr)
		{//ポーズがぬるぽ
			m_pPause = new CPause;		//ポーズ生成
			m_pPause->Init();			//ポーズ初期化
		}

		//ポーズ時の処理
		m_pPause->Update();
	}
	else
	{//ポーズしてない
		if (m_pPause != nullptr)
		{//なんか入ってる
			m_pPause->Uninit();	//終了
			delete m_pPause;	//破棄
			m_pPause = nullptr;	//ぬるぽ入れる
		}

		//普段の処理
		CKoban::CommonUpdate();	//交番共通更新処理
		m_pPlayer->Update();

		//スコア算出
		CulcScore();

		//時間管理
		if (m_pTimer->GetTime() <= 0)
		{//時間切れ
			//CManager::SetMode(CScene::MODE_RESULT);
		}
	}
}

//=================================
//描画
//=================================
void CGame::Draw(void)
{
	//普段はすべてCObjectクラス継承してるものが動いているので自動描画
	//ポーズの中身もすべてCObjectクラス継承してるので自動描画
	//よってここですることはない
}

//=================================
//スコア計算
//=================================
void CGame::CulcScore(void)
{
	//建物オブジェクト全検索
	m_pScore->Set(0);
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

			//スコア算出
			float fParcent;
			long long nScore;

			if (pBuilding->GetUnique() == false)
			{//計算算出
				fParcent = ((float)pBuilding->GetEndurance() / HAVE_LIFE(pBuilding->GetLv()));
				nScore = (1.0f - fParcent) * HAVE_VALUE(pBuilding->GetLv());
			}
			else
			{//個別
				int nEndurance = pBuilding->GetSigEndurance() * pow(10, pBuilding->GetPowEndurance());
				fParcent = ((float)pBuilding->GetEndurance() / nEndurance);
				long long nValue = pBuilding->GetSigValue() * pow(10, pBuilding->GetPowValue());
				nScore = (1.0f - fParcent) * nValue;
			}

			m_pScore->Add(nScore);
		}
		else
		{//もうない
			break;
		}
	}
}
