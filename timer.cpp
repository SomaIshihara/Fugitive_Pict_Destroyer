//======================================================
//
//タイマー処理[timer.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "object.h"
#include "timer.h"
#include "number.h"

//静的メンバ変数
PatternTexture CTimer::m_patTexture = { NULL,0,0 };

//=================================
//コンストラクタ（デフォルト）
//=================================
CTimer::CTimer(int nPriority) : CObject(nPriority)
{
	//値クリア
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fOneWidth = FLOAT_ZERO;
	m_fOneHeight = FLOAT_ZERO;
	m_nCounter = 0;
	m_count = COUNT_DOWN;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CTimer::CTimer(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, int nPriority) : CObject(nPriority)
{
	//値設定
	m_pos = pos;
	m_rot = rot;
	m_fOneWidth = fOneWidth;
	m_fOneHeight = fOneHeight;
	m_nCounter = 0;
	m_count = COUNT_DOWN;
}

//=================================
//デストラクタ
//=================================
CTimer::~CTimer()
{
}

//=================================
//初期化
//=================================
HRESULT CTimer::Init(void)
{
	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
	{//数字オブジェクト初期化
		m_pNumber[cnt] = NULL;
	}

	//タイマーカウンタリセット
	m_nCounter = 0;
	m_count = COUNT_DOWN;

	//タイム設定
	Set(0, COUNT_UP);

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CTimer::Uninit(void)
{
	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
	{//数字オブジェクト終了
		if (m_pNumber[cnt] != NULL)
		{//大丈夫。中身はある
			m_pNumber[cnt]->Uninit();
		}
	}

	//タイムオブジェクト破棄
	Release();
}

//=================================
//更新
//=================================
void CTimer::Update(void)
{
//#ifdef _DEBUG
	//[debug]数字設定
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true)
	{
		Set(120,COUNT_DOWN);	//デバッグ用だからこれでいいよね
	}
//#endif // DEBUG
	//タイマー更新
	m_nCounter++;

	if (m_nCounter >= MAX_FPS)
	{
		//カウンタリセット
		m_nCounter = 0;

		switch (m_count)
		{
		case COUNT_UP:
			m_nTime++;	//加算
			if (m_nTime >= (int)pow(10, TIME_DIGIT))
			{//桁数の限界を超えた
				m_nTime = (int)pow(10, TIME_DIGIT) - 1;
			}
			break;

		case COUNT_DOWN:
			m_nTime--;	//減算
			if (m_nTime < INT_ZERO)
			{//0を下回った
				m_nTime = INT_ZERO;
			}
			break;
		}
	}

	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
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
void CTimer::Draw(void)
{
	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
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
CTimer* CTimer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight)
{
	CTimer* pScore = NULL;

	if (pScore == NULL)
	{
		//多重背景管理オブジェクト生成
		pScore = new CTimer;

		//多重背景管理オブジェクト初期化
		pScore->Init();

		//背景用オブジェクト2D生成
		for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
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
HRESULT CTimer::Load(const char* pPath, int nPatWidth, int nPatHeight)
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
void CTimer::Unload(void)
{
	//テクスチャ破棄
	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
	{//1枚ずつ破棄
		if (m_patTexture.pTexture != NULL)
		{
			m_patTexture.pTexture->Release();
			m_patTexture.pTexture = NULL;
		}
	}
}

//=================================
//タイム設定処理
//=================================
void CTimer::Set(const int nScore,COUNT type)
{
	//数字設定
	m_nTime = nScore;

	//数字分割
	CutNumber();
}

//=================================
//数字分割処理
//=================================
void CTimer::CutNumber(void)
{
	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
	{//数字オブジェクトに渡す
		if (m_pNumber[cnt] != NULL)
		{//大丈夫。中身はある
			m_pNumber[cnt]->SetNumber(m_nTime, cnt);
		}
	}
}