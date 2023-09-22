//======================================================
//
//一般人マネージャ処理[normanager.cpp]
//Author:石原颯馬
//
//======================================================
#include "normanager.h"
#include "manager.h"
#include "point.h"
#include "picto.h"

//静的メンバ変数
const int CNorManager::RESEARCH_NUM = 5;
const float CNorManager::NOBUILDING_TIME_PERCE = 0.5f;

//=================================
//コンストラクタ
//=================================
CNorManager::CNorManager(int nPriority) : CObject(nPriority)
{
	m_maxPicto = CManager::INT_ZERO;
	m_spawnTime = CManager::INT_ZERO;
	m_counter = CManager::INT_ZERO;
}

//=================================
//デストラクタ
//=================================
CNorManager::~CNorManager()
{
}

//=================================
//初期化
//=================================
HRESULT CNorManager::Init(void)
{
	return S_OK;
}

//=================================
//終了
//=================================
void CNorManager::Uninit(void)
{
	//管理オブジェクト破棄
	Release();
}

//=================================
//更新
//=================================
void CNorManager::Update(void)
{
	if (CPictoNormal::GetNumAll() < m_maxPicto)
	{//一般人の人数が上限以下
		m_counter++;	//カウント増やす
		if (m_counter >= m_spawnTime)
		{//沸きの時間
			int spawnPoint = rand() % CPoint::GetNumAll();	//適当に決める
			CPoint* pPoint = CPoint::GetTop();				//先頭取得

			//適当に決めた数字の回数分回す
			for (int cnt = 0; cnt < spawnPoint; cnt++)
			{
				pPoint = pPoint->GetNext();	//次入れる
			}

			//決めたポイントから沸かす
			CPictoNormal::Create(pPoint->GetPos());

			//リセット
			m_counter = 0;
		}
	}
}

//=================================
//描画
//=================================
void CNorManager::Draw(void)
{
	
}

//=================================
//生成処理
//=================================
CNorManager* CNorManager::Create(void)
{
	CNorManager* pObjNorManager = nullptr;

	if (pObjNorManager == nullptr)
	{
		//背景の生成
		pObjNorManager = new CNorManager;

		//初期化
		pObjNorManager->Init();

		return pObjNorManager;
	}
	else
	{
		return nullptr;
	}
}