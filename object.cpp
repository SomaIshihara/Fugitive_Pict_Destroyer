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
CObject* CObject::m_apTop[PRIORITY_MAX] = {};
CObject* CObject::m_apCur[PRIORITY_MAX] = {};
int CObject::m_nNumAll = 0;

//=================================
//コンストラクタ
//=================================
CObject::CObject(int nPriority)
{
	if (m_apCur[nPriority] == nullptr)
	{//最後尾がいない（すなわち先頭もいない）
		m_apTop[nPriority] = this;	//俺が先頭
		m_pPrev = nullptr;				//前後誰もいない
		m_pNext = nullptr;
	}
	else
	{//最後尾がいる
		m_pPrev = m_apCur[nPriority];		//最後尾が自分の前のオブジェ
		m_apCur[nPriority]->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = nullptr;						//自分の次のオブジェはいない
	}
	m_nPriority = nPriority;	//優先順位入れる
	m_apCur[nPriority] = this;	//俺が最後尾
	m_bDeath = false;			//生きてる
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
	for (int cnt = 0; cnt < PRIORITY_FADE; cnt++)	//フェードの破棄をしないため一つ手前まで行う
	{
		CObject* pObject = m_apTop[cnt];	//先頭を入れる

		while (pObject != nullptr)
		{//最後尾まで回し続ける
			CObject* pObjectNext = pObject->m_pNext;	//次のオブジェ保存
			pObject->Uninit();		//破棄
			pObject = pObjectNext;	//次を入れる
		}
	}

	//殺す
	Death();
}

//=================================
//オブジェクトすべて更新
//=================================
void CObject::UpdateAll(void)
{
	for (int cnt = 0; cnt < PRIORITY_MAX; cnt++)
	{
		CObject* pObject = m_apTop[cnt];	//先頭を入れる

		while (pObject != nullptr)
		{//最後尾まで回し続ける
			CObject* pObjectNext = pObject->m_pNext;	//次のオブジェ保存
			
			if (pObject->m_bDeath == false)
			{
				pObject->Update();		//更新
			}

			pObject = pObjectNext;	//次を入れる
		}
	}

	//殺す
	Death();
}

//=================================
//オブジェクトすべて描画
//=================================
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();	//カメラ
	pCamera->SetCamera();

	for (int cnt = 0; cnt < PRIORITY_MAX; cnt++)
	{
		CObject* pObject = m_apTop[cnt];	//先頭を入れる

		while (pObject != nullptr)
		{//最後尾まで回し続ける
			CObject* pObjectNext = pObject->m_pNext;	//次のオブジェ保存
			pObject->Draw();		//描画
			pObject = pObjectNext;	//次を入れる
		}
	}
}

//=================================
//オブジェクト破棄
//=================================
void CObject::Release(void)
{
	m_bDeath = true;	//死亡フラグが立ちました。
}

//=================================
//ぶっ殺す
//=================================
void CObject::Death(void)
{
	for (int cnt = 0; cnt < PRIORITY_MAX; cnt++)
	{
		CObject* pObject = m_apTop[cnt];	//先頭を入れる

		while (pObject != nullptr)
		{//最後尾まで回し続ける
			CObject* pObjectNext = pObject->m_pNext;	//次のオブジェ保存

			if (pObject->m_bDeath == true)
			{//死亡フラグが立ってる
				if (pObject->m_pPrev != nullptr)
				{//前にオブジェがいる
					pObject->m_pPrev->m_pNext = pObject->m_pNext;	//前のオブジェの次のオブジェは自分の次のオブジェ
				}
				if (pObject->m_pNext != nullptr)
				{
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;	//次のオブジェの前のオブジェは自分の前のオブジェ
				}

				if (m_apCur[cnt] == pObject)
				{//最後尾でした
					m_apCur[cnt] = pObject->m_pPrev;	//最後尾を自分の前のオブジェにする
				}
				if (m_apTop[cnt] == pObject)
				{
					m_apTop[cnt] = pObject->m_pNext;	//先頭を自分の次のオブジェにする
				}

				//成仏	
				delete pObject;	//自分自身破棄
				m_nNumAll--;	//総数減らす
			}

			pObject = pObjectNext;	//次を入れる
		}
	}
}