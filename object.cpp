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
CObject* CObject::m_pTop = NULL;
CObject* CObject::m_pCur = NULL;
CObject* CObject::m_pProcessNext = NULL;
int CObject::m_nNumAll = 0;

//=================================
//コンストラクタ
//=================================
CObject::CObject(int nPriority)
{
	if (m_pCur == NULL)
	{//最後尾がいない（すなわち先頭もいない）
		m_pTop = this;	//俺が先頭
		m_pPrev = NULL;	//前後誰もいない
		m_pNext = NULL;
	}
	else
	{//最後尾がいる
		m_pPrev = m_pCur;		//最後尾が自分の前のオブジェ
		m_pCur->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = NULL;			//自分の次のオブジェはいない
	}
	m_pCur = this;	//俺が最後尾
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
	CObject* pObject = m_pTop;	//先頭を入れる

#if 0
	while (pObject != NULL)
	{//最後尾まで回し続ける
		CObject* pObjectNext = pObject->m_pNext;	//次のオブジェ保存
		pObject->Uninit();		//破棄
		pObject = pObjectNext;	//次を入れる
	}
#endif
	while (pObject != NULL)
	{//最後尾まで回し続ける
		 //CObject* pObjectNext = pObject->m_pNext;	//次のオブジェ保存
		m_pProcessNext = pObject->m_pNext;	//次のオブジェ保存
		pObject->Uninit();		//終了
		//pObject = pObjectNext;	//次を入れる
		pObject = m_pProcessNext;	//次を入れる
	}
}

//=================================
//オブジェクトすべて更新
//=================================
void CObject::UpdateAll(void)
{
	CObject* pObject = m_pTop;	//先頭を入れる
#if 0
	while (pObject != NULL)
	{//最後尾まで回し続ける
		CObject* pObjectNext = pObject->m_pNext;	//次のオブジェ保存
		pObject->Update();		//更新
		pObject = pObjectNext;	//次を入れる
	}
#endif
	while (pObject != NULL)
	{//最後尾まで回し続ける
		//CObject* pObjectNext = pObject->m_pNext;	//次のオブジェ保存
		m_pProcessNext = pObject->m_pNext;	//次のオブジェ保存
		pObject->Update();		//更新
		//pObject = pObjectNext;	//次を入れる
		pObject = m_pProcessNext;	//次を入れる
	}
}

//=================================
//オブジェクトすべて描画
//=================================
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();	//カメラ
	pCamera->SetCamera();

	CObject* pObject = m_pTop;	//先頭を入れる

	while (pObject != NULL)
	{//最後尾まで回し続ける
		//CObject* pObjectNext = pObject->m_pNext;	//次のオブジェ保存
		m_pProcessNext = pObject->m_pNext;	//次のオブジェ保存
		pObject->Draw();		//描画
		//pObject = pObjectNext;	//次を入れる
		pObject = m_pProcessNext;	//次を入れる
	}
}

//=================================
//オブジェクト破棄
//=================================
void CObject::Release(void)
{
	int nPriority = m_nPriority;

	if (m_pPrev != NULL)
	{//前にオブジェがいる
		m_pPrev->m_pNext = m_pNext;	//前のオブジェの次のオブジェは自分の次のオブジェ
	}
	if (m_pNext != NULL)
	{
		m_pNext->m_pPrev = m_pPrev;	//次のオブジェの前のオブジェは自分の前のオブジェ
	}

	if (m_pCur == this)
	{//最後尾でした
		m_pCur = m_pPrev;	//最後尾を自分の前のオブジェにする
	}
	if (m_pTop == this)
	{
		m_pTop = m_pNext;	//先頭を自分の次のオブジェにする
	}
	if (m_pProcessNext == this)
	{
		m_pProcessNext = m_pPrev->m_pNext;
	}

	//成仏	
	delete this;	//自分自身破棄
	m_nNumAll--;	//総数減らす
}