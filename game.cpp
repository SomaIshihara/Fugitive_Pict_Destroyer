//======================================================
//
//ゲームシーン[game.cpp]
//Author:石原颯馬
//
//======================================================
#include "game.h"
#include "ranking.h"
#include "pause.h"
#include "result.h"
#include "texture.h"
#include "player.h"
#include "meshField.h"
#include "slider.h"
#include "building.h"
#include "score.h"
#include "timer.h"
#include "objectX.h"
#include "picto.h"
#include "koban.h"
#include "meshsky.h"
#include "point.h"
#include "input.h"
#include "havenum.h"
#include "level.h"
#include "manager.h"
#include "camera.h"
#include "bg.h"
#include "countdown.h"
#include "normanager.h"
#include "policemanager.h"
#include "sound.h"

//静的メンバ変数
CPlayer* CGame::m_pPlayer = nullptr;
CMeshField* CGame::m_pMeshField = nullptr;
CTimer* CGame::m_pTimer = nullptr;
CScore* CGame::m_pScore = nullptr;
CHaveNum* CGame::m_pHaveNum[];
CLevel* CGame::m_pLevel[];
CMeshSky* CGame::m_pSky = nullptr;
int CGame::m_nATKBuilding = CManager::INT_ZERO;
int CGame::m_nDestBuilding = CManager::INT_ZERO;
const int CGame::CDSTART_TIME = MAX_FPS;

//=================================
//コンストラクタ
//=================================
CGame::CGame()
{
	m_pPause = nullptr;
	m_pWarning = nullptr;
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

	//カメラ位置リセット
	CManager::GetCamera()->ResetPos();

	//仮オブジェ生成
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(-1280.0f, 0.0f, 1280.0f), CManager::VEC3_ZERO, 64.0f, 64.0f, 40, 40);

	//オブジェクト生成+初期化
	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 24.0f, 32.0f, 0.0f), CManager::VEC3_ZERO, 48.0f, 72.0f);
	m_pTimer->Set(120, CTimer::COUNT_DOWN);
	m_pTimer->Stop();

	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), CManager::VEC3_ZERO, 40.0f, 64.0f);

	CObjectX* pAgit = CObjectX::Create(D3DXVECTOR3(600.0f,0.0f,0.0f), CManager::VEC3_ZERO, CManager::GetAgitModel());
	CPicto::SetAgit(pAgit);

	m_pSky = CMeshSky::Create(CManager::VEC3_ZERO, CManager::VEC3_ZERO, 10000.0f, 8, 8);

	m_pLevel[CPicto::TYPE_DESTROYER] = CLevel::Create(D3DXVECTOR3(SCREEN_WIDTH - 160.0f, 100.0f, 0.0f), CManager::VEC3_ZERO, 30.0f, 36.0f, 2, CTexture::PRELOAD_LV);
	m_pLevel[CPicto::TYPE_BLOCKER] = CLevel::Create(D3DXVECTOR3(SCREEN_WIDTH - 160.0f, 136.0f, 0.0f), CManager::VEC3_ZERO, 30.0f, 36.0f, 2, CTexture::PRELOAD_LV);

	m_pHaveNum[CPicto::TYPE_DESTROYER] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 100.0f, 0.0f), CManager::VEC3_ZERO, 30.0f, 36.0f, 2, CTexture::PRELOAD_HAVEICON_01);
	m_pHaveNum[CPicto::TYPE_BLOCKER] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 136.0f, 0.0f), CManager::VEC3_ZERO, 30.0f, 36.0f, 2, CTexture::PRELOAD_HAVEICON_02);
	m_pHaveNum[CPicto::TYPE_NORMAL] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 172.0f, 0.0f), CManager::VEC3_ZERO, 30.0f, 36.0f, 5, CTexture::PRELOAD_HAVEICON_03);
	m_pHaveNum[CPicto::TYPE_DESTROYER]->AddNum(2);
	m_pHaveNum[CPicto::TYPE_BLOCKER]->AddNum(2);
	m_pHaveNum[CPicto::TYPE_NORMAL]->AddNum(1000);

	m_pWarning = CBG::Create(PRIORITY_UI);
	m_pWarning->BindTexture(CTexture::PRELOAD_WARNING_LIFE);
	m_pWarning->SetEnable(false);	//いったん非表示

	m_pCountDown = CCountDown::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 150.0f, 180.0f, 3);

	//マップデータ読み込みと配置
	CObjectX::LoadData("data\\Fugitive_Picto_MapData_v120.ismd");

	//ポイント生成
	CPoint::Update();

	//警察マネージャ生成・パラメータ設定
	CPoliceManager::Create();
	//CPoliceManager::SetKobanParam(300, 1, 1);	//仮設定
	CPoliceManager::SetKobanParam(300, 15, 7);	//仮設定

	//一般人マネージャ
	CNorManager* pNorManager = CNorManager::Create();
	pNorManager->SetNum(15);
	pNorManager->SetTime(180);

	//レベル初期化
	CPictoDestroyer::ResetLevelExp();
	CPictoBlocker::ResetLevelExp();

	//BGM流す
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM);

	return S_OK;
}

//=================================
//終了
//=================================
void CGame::Uninit(void)
{
	CObject::ReleaseAll();
	CManager::GetSound()->Stop();

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

	//カウントダウン破棄（一応）
	if (m_pCountDown != nullptr)
	{
		m_pCountDown->Uninit();
		delete m_pCountDown;
		m_pCountDown = nullptr;
	}

	m_pPlayer = nullptr;
	m_pSlider = nullptr;
	m_pMeshField = nullptr;
	m_pTimer = nullptr;
	m_pScore = nullptr;
	m_pSky = nullptr;
	m_pWarning = nullptr;
}

//=================================
//更新
//=================================
void CGame::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//キーボード取得

	//カウントダウンの更新
	if (m_pCountDown != nullptr)
	{
		m_pCountDown->Update();
		//カウントダウン前
		if (m_counterCDStart == CDSTART_TIME)
		{
			m_pCountDown->Start();	//カウントダウン開始
			m_counterCDStart++;
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (m_counterCDStart < CDSTART_TIME)
		{
			m_counterCDStart++;		//開始時間までカウント
		}

		//カウントダウン後
		if (m_pCountDown->GetCount() <= -1)
		{
			m_pCountDown->Uninit();
			delete m_pCountDown;
			m_pCountDown = nullptr;
			m_pTimer->Start();
		}
	}
	else
	{//カウントダウンオブジェクトが消えた
		if (pKeyboard->GetTrigger(DIK_P) == true)
		{//ポーズ切り替え
			CManager::SetPause((CManager::GetPause() == true ? false : true));
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BUTTON);
		}

		if (m_pResult != nullptr)
		{//リザルト
			m_pResult->Update();
		}
		else
		{//ゲーム
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

				//体力
				bool bWarning = false;
				for (int cnt = 0; cnt < MAX_OBJ; cnt++)
				{//デストロイヤー
					CPictoDestroyer* pPicto = CPictoDestroyer::GetPicto(cnt);
					if (pPicto != nullptr)
					{
						CPicto::TYPE type = pPicto->GetType();
						if (pPicto->GetLife() <= HAVE_LIFE(pPicto->GetLv()) * CPictoTaxi::RESCUE_LIFE)
						{//危険
							bWarning = true;
							break;
						}
					}
				}
				for (int cnt = 0; cnt < MAX_OBJ; cnt++)
				{//ブロッカー
					CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);
					if (pPicto != nullptr)
					{
						CPicto::TYPE type = pPicto->GetType();
						if (pPicto->GetLife() <= HAVE_LIFE(pPicto->GetLv()) * CPictoTaxi::RESCUE_LIFE)
						{//危険
							bWarning = true;
							break;
						}
					}
				}
				//変更
				m_pWarning->SetEnable(bWarning);

				//普段の処理
				m_pPlayer->Update();

				//スコア算出
				CulcScore();

				//時間管理
				if (m_pTimer->GetTime() <= 0)
				{//時間切れ
					if (m_pResult == nullptr)
					{
						CRanking::Set(m_pScore->GetScore());
						m_pResult = CResult::Create();
					}
				}
			}
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
	//スコア類リセット
	m_pScore->Set(0);
	m_nATKBuilding = 0;
	m_nDestBuilding = 0;

	//建物オブジェクト全検索
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//建物オブジェクト取得
		if (pBuilding != nullptr)
		{//なんかある
			CXModel* pModel = CXModel::GetTop();
			int nModelNum = 0;
			while (pModel != nullptr && pModel != pBuilding->GetModel())
			{
				pModel = pModel->GetNext();
				nModelNum++;
			}

			//スコア算出
			float fParcent;
			long long nScore;

			if (pBuilding->GetUnique() == false)
			{//計算算出
				int nEndurance = pBuilding->GetEndurance();
				int nMaxEndurance = HAVE_LIFE(pBuilding->GetLv());

				if (nEndurance < nMaxEndurance)
				{//減っている
					m_nATKBuilding++;	//攻撃した

					if (nEndurance <= 0)
					{//全壊
						m_nDestBuilding++;
					}

					//被害額計算
					fParcent = ((float)nEndurance / nMaxEndurance);
					nScore = (1.0f - fParcent) * HAVE_VALUE(pBuilding->GetLv());
					m_pScore->Add(nScore);
				}
			}
			else
			{//個別
				int nMaxEndurance = pBuilding->GetSigEndurance() * pow(10, pBuilding->GetPowEndurance());
				int nEndurance = pBuilding->GetEndurance();

				if (nEndurance < nMaxEndurance)
				{//減っている
					m_nATKBuilding++;	//攻撃した

					if (nEndurance <= 0)
					{//全壊
						m_nDestBuilding++;
					}

					//被害額計算
					fParcent = ((float)nEndurance / nMaxEndurance);
					long long nValue = pBuilding->GetSigValue() * pow(10, pBuilding->GetPowValue());
					nScore = (1.0f - fParcent) * nValue;
					m_pScore->Add(nScore);
				}
			}
		}
		else
		{//もうない
			break;
		}
	}
}
