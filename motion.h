//======================================================
//
//モーション処理のヘッダ[motion.h]
//Author:石原颯馬
//
//======================================================
#ifndef _MOTION_H_
#define _MOTION_H_

//マクロ
#define KEYINFO_NUM		(16)	//キー情報最大数（モデル数に依存）
#define INFO_NUM		(8)		//モーション情報最大数

//前方宣言
class CModel;

//モーションクラス
class CMotion
{
public:
	//キー構造体
	typedef struct
	{
		float m_fPosX;	//位置X
		float m_fPosY;	//位置Y
		float m_fPosZ;	//位置Z
		float m_fRotX;	//向きX
		float m_fRotY;	//向きY
		float m_fRotZ;	//向きZ
	} KEY;

	//キー情報の構造体
	typedef struct
	{
		int m_nFrame;			//再生フレーム
		KEY* m_pKey;			//モデルのキー要素（動的確保）
	} KEY_INFO;

	//モーション状態構造体
	typedef struct
	{
		bool m_bLoop;						//ループするか
		int m_nNumKey;						//キー総数
		KEY_INFO m_aKeyInfo[KEYINFO_NUM];	//キーの情報（16は適宜変えて）
	} INFO;

	//コンストラクタ・デストラクタ
	CMotion();
	~CMotion();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);

	//モーション設定・更新
	void Set(int nType);
	void Update(void);

	//取得
	int GetType(void) { return m_nType; }
	bool IsFinish(void) { return m_bFinish; }

	//必要な情報設定
	void SetInfo(INFO info);
	void SetModel(CModel** ppModel, int nNumModel);


private:
	INFO m_aInfo[INFO_NUM];	//モーション情報（配列仮）
	int m_nNumAll;					//モーション総数

	int m_nType;					//使用中モーションタイプ
	bool m_bLoop;					//ループするか
	int m_nNumKey;					//キー総数
	int m_nKey;						//現在のキー
	int m_nCounter;					//モーションカウンタ
	bool m_bFinish;					//モーション終了したか

	CModel** m_ppModel;				//モデルへのポインタ
	int m_nNumModel;				//モデル数
};

#endif // !_MOTION_H_
