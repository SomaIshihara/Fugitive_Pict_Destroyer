//======================================================
//
//オブジェクト処理[object.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "object.h"

//静的変数初期化
CObject* CObject::m_apObject[PRIORITY_MAX][MAX_OBJ];
int CObject::m_nNumAll = 0;

//=================================
//コンストラクタ
//=================================
CObject::CObject(int nPriority)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apObject[nPriority][cnt] == NULL)
		{//空っぽ
			m_apObject[nPriority][cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nPriority = nPriority;	//優先順位覚える
			m_Type = TYPE_NONE;	//いったん何でもないものとする
			m_nNumAll++;	//総数増やす
			break;
		}
	}
}

//=================================
//デストラクタ
//=================================
CObject::~CObject()
{}

//=================================
//オブジェクトすべて破棄
//=================================
void CObject::ReleaseAll(void)
{
	for (int cntPriority = 0; cntPriority < PRIORITY_MAX; cntPriority++)
	{
		for (int cntObj = 0; cntObj < MAX_OBJ; cntObj++)
		{//すべて確認
			if (m_apObject[cntPriority][cntObj] != NULL)
			{//空っぽではない
				m_apObject[cntPriority][cntObj]->Uninit();
			}
		}
	}
}

//=================================
//オブジェクトすべて更新
//=================================
void CObject::UpdateAll(void)
{
	for (int cntPriority = 0; cntPriority < PRIORITY_MAX; cntPriority++)
	{
		for (int cntObj = 0; cntObj < MAX_OBJ; cntObj++)
		{//すべて確認
			if (m_apObject[cntPriority][cntObj] != NULL)
			{//空っぽではない
				m_apObject[cntPriority][cntObj]->Update();
			}
		}
	}
}

//=================================
//オブジェクトすべて描画
//=================================
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();	//カメラ
	pCamera->SetCamera();
	for (int cntPriority = 0; cntPriority < PRIORITY_MAX; cntPriority++)
	{
		for (int cntObj = 0; cntObj < MAX_OBJ; cntObj++)
		{//すべて確認
			if (m_apObject[cntPriority][cntObj] != NULL)
			{//空っぽではない
				m_apObject[cntPriority][cntObj]->Draw();
			}
		}
	}
}

//=================================
//オブジェクト破棄
//=================================
void CObject::Release(void)
{
	int nID = m_nID;				//deleteしたら使えなくなるので保管
	int nPriority = m_nPriority;
	if (m_apObject[nPriority][nID] != NULL)
	{//空っぽではない
		//自分自身破棄
		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = NULL;
		m_nNumAll--;	//総数減らす
	}
}