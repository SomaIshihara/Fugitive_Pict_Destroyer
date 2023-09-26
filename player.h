//======================================================
//
//プレイヤー処理のヘッダ[player.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//前方宣言
class CObject;
class CButton2D;
class CProgressBar;

//プレイヤークラス
class CPlayer
{
public:
	//コンストラクタ・デストラクタ
	CPlayer();
	~CPlayer();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//攻撃処理
	void Attack(void);

	//収容処理
	void AddPicto(const int nDestroyer, const int nBlocker, const int nNormal);

private:
	//関数
	void Move(void);								//カメラ移動処理
	void Rotate(void);								//カメラ回転処理
	void Select(void);								//オブジェクト選択

	//変数
	CButton2D* m_pButtonATK;		//攻撃ボタン
	CProgressBar* m_pProgBar;		//進捗バー

	bool m_bControllPicto;			//ピクトさんを操縦するかどうか
	CObject* m_pObject;				//選択中の建物・ピクト

	D3DXVECTOR3 m_cursorPos;	//クリック前のカーソル位置
};

#endif