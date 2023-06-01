//======================================================
//
//オブジェクト処理[object.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"

//静的変数初期化
CObject* CObject::m_apObject[] = {};
int CObject::m_nNumAll = 0;

//=================================
//コンストラクタ
//=================================
CObject::CObject()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apObject[cnt] == NULL)
		{//空っぽ
			m_apObject[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
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
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apObject[cnt] != NULL)
		{//空っぽではない
			m_apObject[cnt]->Uninit();
		}
	}
}

//=================================
//オブジェクトすべて更新
//=================================
void CObject::UpdateAll(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apObject[cnt] != NULL)
		{//空っぽではない
			m_apObject[cnt]->Update();
		}
	}
}

//=================================
//オブジェクトすべて描画
//=================================
void CObject::DrawAll(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apObject[cnt] != NULL)
		{//空っぽではない
			m_apObject[cnt]->Draw();
		}
	}
}

//=================================
//オブジェクト破棄
//=================================
void CObject::Release(void)
{
	int nID = m_nID;
	if (m_apObject[nID] != NULL)
	{//空っぽではない
		//自分自身破棄
		delete m_apObject[nID];
		m_apObject[nID] = NULL;
		m_nNumAll--;	//総数減らす
	}
}