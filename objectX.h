//======================================================
//
//オブジェクト（X）処理のヘッダ[objectX.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_
#include "main.h"
#include "manager.h"
#include "object.h"

typedef unsigned char BINCODE;
//バイナリのコード内容
//システム(0b00xxxxxx)
#define BIN_CODE_SYSTEM				(0b00000000)
#define BIN_CODE_SCRIPT				(BIN_CODE_SYSTEM + 0b000000)
#define BIN_CODE_END_SCRIPT			(BIN_CODE_SYSTEM + 0b000001)
//モデル系(0b01xxxxxx)
#define BIN_CODE_MODEL				(0b01000000)
#define BIN_CODE_TEXTURE_FILENAME	(BIN_CODE_MODEL + 0b000000)
#define BIN_CODE_MODEL_FILENAME		(BIN_CODE_MODEL + 0b000001)
#define BIN_CODE_MODELSET			(BIN_CODE_MODEL + 0b000010)
#define BIN_CODE_TEXTURE_NUM		(BIN_CODE_MODEL + 0b000011)
#define BIN_CODE_MODEL_NUM			(BIN_CODE_MODEL + 0b000100)
//モーション系(0b10xxxxxx)
#define BIN_CODE_MOTION				(0b10000000)

//前方宣言
class CXModel;

//オブジェクトクラス
class CObjectX : public CObject
{
public:
	enum LOADRESULT
	{
		RES_OK = 0,
		RES_ERR_FILE_NOTFOUND,
		RES_MAX
	};

	enum STATE
	{
		STATE_NONE = 0,		//設定なし
		STATE_BREAKABLE,	//破壊可能設定
		STATE_KOBAN,		//交番設定
		STATE_MAX
	};

	//コンストラクタ・デストラクタ
	CObjectX(int nPriority = PRIORITY_DEFAULT);																	//デフォルト
	CObjectX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel, int nPriority = PRIORITY_DEFAULT);	//オーバーロード（位置向きサイズ）
	virtual ~CObjectX();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	static CObjectX* GetTop(void) { return m_pTop; }
	CObjectX* GetNext(void) { return m_pNext; }
	CXModel* GetModel(void) { return m_pModel; }
	bool GetBreakable(void) { return m_bBreakale; }
	bool GetStateKoban(void) { return m_bKoban; }

	//設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetBreakable(bool bFrag) { m_bBreakale = bFrag; }
	void SetStateKoban(bool bFrag) { m_bKoban = bFrag; }

	//使用モデル単位で消す
	static void Delete(CXModel* pTarget);

	//読み込み
	static LOADRESULT LoadData(const char* pPath);
	static LOADRESULT SaveData(const char* pPath);

	//死亡フラグが立っているオブジェを殺す
	static void Exclusion(void);

private:
	//モデル
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	CXModel* m_pModel;		//モデル

	//位置類
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	float m_fDepth;		//奥行

	//状態
	bool m_bBreakale;	//破壊可能設定
	bool m_bKoban;		//交番設定

	//リスト
	static CObjectX* m_pTop;	//先頭オブジェクト
	static CObjectX* m_pCur;	//最後尾オブジェクト
	CObjectX* m_pNext;			//次のオブジェクト
	CObjectX* m_pPrev;			//前のオブジェクト
	bool m_bExclusion;			//除外フラグ
	static int m_nNumAll;		//総数
};

#endif // !_OBJECT_H_
