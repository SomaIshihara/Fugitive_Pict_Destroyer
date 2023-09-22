//======================================================
//
//チュートリアルシーン[tutorial.cpp]
//Author:石原颯馬
//
//======================================================
#include "tutorial.h"
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
#include "manager.h"
#include "camera.h"
#include "bg.h"
#include "fade.h"
#include "button.h"
#include "policemanager.h"
#include "sound.h"

//静的メンバ変数
CPlayer* CTutorial::m_pPlayer = nullptr;
CMeshField* CTutorial::m_pMeshField = nullptr;
CScore* CTutorial::m_pScore = nullptr;
CHaveNum* CTutorial::m_pHaveNum[];
CMeshSky* CTutorial::m_pSky = nullptr;
const int CTutorial::TUTORIAL_DELETE_TIME = 20;
const float CTutorial::TUTORIAL_ALPHA_DEF = 0.4f;

//=================================
//コンストラクタ
//=================================
CTutorial::CTutorial()
{
	m_pWarning = nullptr;
}

//=================================
//デストラクタ
//=================================
CTutorial::~CTutorial()
{
}

//=================================
//初期化
//=================================
HRESULT CTutorial::Init(void)
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
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), CManager::VEC3_ZERO, 40.0f, 64.0f);

	CObjectX* pAgit = CObjectX::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), CManager::VEC3_ZERO, CManager::GetAgitModel());
	CPicto::SetAgit(pAgit);

	m_pSky = CMeshSky::Create(CManager::VEC3_ZERO, CManager::VEC3_ZERO, 10000.0f, 8, 8);

	m_pHaveNum[CPicto::TYPE_DESTROYER] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 100.0f, 0.0f), CManager::VEC3_ZERO, 30.0f, 36.0f, 2, CTexture::PRELOAD_HAVEICON_01);
	m_pHaveNum[CPicto::TYPE_BLOCKER] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 136.0f, 0.0f), CManager::VEC3_ZERO, 30.0f, 36.0f, 2, CTexture::PRELOAD_HAVEICON_02);
	m_pHaveNum[CPicto::TYPE_NORMAL] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 172.0f, 0.0f), CManager::VEC3_ZERO, 30.0f, 36.0f, 5, CTexture::PRELOAD_HAVEICON_03);
	m_pHaveNum[CPicto::TYPE_DESTROYER]->AddNum(10);
	m_pHaveNum[CPicto::TYPE_BLOCKER]->AddNum(10);
	m_pHaveNum[CPicto::TYPE_NORMAL]->AddNum(0);

	//タイトル
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 61.5f, 0.0f), CManager::VEC3_ZERO, 311.0f, 111.0f, PRIORITY_UI)->BindTexture(CTexture::PRELOAD_TUTORIAL);

	//チュートリアル
	m_pTutorial[3] = CButton2D::Create(D3DXVECTOR3(146.0f, 91.2f, 0.0f), CManager::VEC3_ZERO, 260.0f, 166.4f, PRIORITY_UI);
	m_pTutorial[3]->BindTexture(CTexture::PRELOAD_TUTORIAL_04);
	m_pTutorial[0] = CButton2D::Create(D3DXVECTOR3(102.8f, 262.4f, 0.0f), CManager::VEC3_ZERO, 165.6f, 160.0f, PRIORITY_UI);
	m_pTutorial[0]->BindTexture(CTexture::PRELOAD_TUTORIAL_01);
	m_pTutorial[1] = CButton2D::Create(D3DXVECTOR3(78.4f, 426.8f, 0.0f), CManager::VEC3_ZERO, 116.8f, 152.8f, PRIORITY_UI);
	m_pTutorial[1]->BindTexture(CTexture::PRELOAD_TUTORIAL_02);
	m_pTutorial[2] = CButton2D::Create(D3DXVECTOR3(135.2f, 596.4f, 0.0f), CManager::VEC3_ZERO, 238.4f, 170.4f, PRIORITY_UI);
	m_pTutorial[2]->BindTexture(CTexture::PRELOAD_TUTORIAL_03);

	for (int cnt = 0; cnt < TUTORIAL_NUM; cnt++)
	{
		m_nTutorialDelCnt[cnt] = CManager::INT_ZERO;
	}

	m_pWarning = CBG::Create(PRIORITY_UI);
	m_pWarning->BindTexture(CTexture::PRELOAD_WARNING_LIFE);
	m_pWarning->SetEnable(false);	//いったん非表示

	//マップデータ読み込みと配置
	CObjectX::LoadData("data\\Fugitive_Picto_MapData_Tutorial.ismd");

	//ポイント生成
	CPoint::Update();

	//警察マネージャ生成・パラメータ設定
	CPoliceManager::Create();
	CPoliceManager::SetKobanParam(300, 2, 1);	//設定

	return S_OK;
}

//=================================
//終了
//=================================
void CTutorial::Uninit(void)
{
	CObject::ReleaseAll();

	//プレイヤー破棄
	if (m_pPlayer != nullptr)
	{//プレイヤー終了
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

	m_pPlayer = nullptr;
	m_pSlider = nullptr;
	m_pMeshField = nullptr;
	m_pScore = nullptr;
	m_pSky = nullptr;
	m_pWarning = nullptr;
}

//=================================
//更新
//=================================
void CTutorial::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//キーボード取得

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

	//チュートリアル消す処理
	for (int cnt = 0; cnt < TUTORIAL_NUM; cnt++)
	{
		if (m_pTutorial[cnt] != nullptr)
		{
			if (m_pTutorial[cnt]->IsClickPress() == true)
			{//クリック（削除）
				m_nTutorialDelCnt[cnt]++;	//カウント増やす
				m_pTutorial[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, TUTORIAL_ALPHA_DEF * (1.0f - (float)m_nTutorialDelCnt[cnt] / TUTORIAL_DELETE_TIME)));
				if (m_nTutorialDelCnt[cnt] >= TUTORIAL_DELETE_TIME)
				{//カウント過ぎた
					m_pTutorial[cnt]->Uninit();
					m_pTutorial[cnt] = nullptr;
				}
			}
			else if (m_pTutorial[cnt]->IsHold() == true)
			{//かざす（半透明）
				m_pTutorial[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, TUTORIAL_ALPHA_DEF));
				m_nTutorialDelCnt[cnt] = 0;	//カウントリセット
			}
			else
			{//色戻す
				m_pTutorial[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_nTutorialDelCnt[cnt] = 0;	//カウントリセット
			}
		}
	}

	//普段の処理
	m_pPlayer->Update();

	//スコア算出
	CulcScore();

	//気が済んだ
	if (m_pFade == nullptr && pKeyboard->GetTrigger(DIK_P) == true)
	{
		m_pFade = CFade::Create(CScene::MODE_GAME);
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BUTTON);
	}
}

//=================================
//描画
//=================================
void CTutorial::Draw(void)
{
	//普段はすべてCObjectクラス継承してるものが動いているので自動描画
	//ポーズの中身もすべてCObjectクラス継承してるので自動描画
	//よってここですることはない
}

//=================================
//スコア計算
//=================================
void CTutorial::CulcScore(void)
{
	//スコア類リセット
	m_pScore->Set(0);

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
