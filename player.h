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
class CBuilding;
class CPict;
class CButton2D;

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
	void AddPict(const int nDestroyer, const int nBlocker, const int nNormal);

private:
	//関数
	void Move(void);								//カメラ移動処理
	void Rotate(void);								//カメラ回転処理
	void Select(void);								//オブジェクト選択

	//変数
	CButton2D* m_pButtonATK;		//攻撃ボタン

	bool m_bControllPict;			//ピクトさんを操縦するかどうか
	CBuilding* m_pSelectBuilding;	//選択中の建物
	CPict* m_pSelectPict;			//選択中のピクトさん

	int m_nHaveDestroyer;	//デストロイヤー所持数
	int m_nHaveBlocker;		//ブロッカー所持数
	int m_nHaveNormal;		//一般人所持数

	D3DXVECTOR3 m_cursorPos;	//クリック前のカーソル位置
};

#endif