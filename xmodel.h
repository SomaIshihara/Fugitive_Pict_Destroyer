//======================================================
//
//Xファイルモデルのヘッダ[xmodel.h]
//Author:石原颯馬
//
//======================================================
#ifndef _XMODEL_H_
#define _XMODEL_H_

#include "collision.h"

//Xモデルクラス
class CXModel
{
public:
	enum LOADRESULT
	{
		RES_OK = 0,
		RES_ERR_LIST_NOTFOUND,
		RES_ERR_MODEL_NOTFOUND,
		RES_MAX
	};

	//コンストラクタ・デストラクタ
	CXModel();
	~CXModel();

	//読み込み関係
	//モデル
	static CXModel* Load(const char* pPath);
	void Unload(void);
	static void UnloadAll(void);

	//モデルリスト
	static LOADRESULT LoadList(const char* pPath);
	static LOADRESULT SaveList(const char* pPath);

	//取得
	LPD3DXMESH GetMesh(void) { return m_pMesh; }
	LPD3DXBUFFER GetBufMat(void) { return m_pBuffMat; }
	DWORD GetNumMat(void) { return m_dwNumMatModel; }
	int* GetIdxTexture(void) { return m_pIdxtexture; }
	char* GetPath(void) { return m_pFilePath; }
	CCollision GetCollision(void) { return m_collision; }
	static int GetNumAll(void) { return m_nNumAll; }	

	//リスト取得
	static CXModel* GetTop(void) { return m_pTop; }
	CXModel* GetNext(void) { return m_pNext; }

private:
	//リスト
	static CXModel* m_pTop;		//先頭オブジェクト
	static CXModel* m_pCur;		//最後尾オブジェクト
	CXModel* m_pNext;			//次のオブジェクト
	CXModel* m_pPrev;			//前のオブジェクト
	bool m_bExclusion;			//除外フラグ
	static int m_nNumAll;		//総数

	LPD3DXMESH m_pMesh;			//メッシュ
	LPD3DXBUFFER m_pBuffMat;	//マテリアルポインタ
	DWORD m_dwNumMatModel;		//マテ数
	int* m_pIdxtexture;			//テクスチャ番号（動的確保）

	char* m_pFilePath;			//ファイルパス（文字数分動的確保）
	CCollision m_collision;		//コリジョン
};

#endif // !_XMODEL_H_