//======================================================
//
//プレイヤー処理のヘッダ[player.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

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

	//追従設定
	void Chace(int nPictNum) { m_nChacePictNum = nPictNum; }
	void Unchace(void) { m_nChacePictNum = -1; }

private:
	//関数
	void Move(void);								//カメラ移動処理
	void Rotate(void);								//カメラ回転処理
	void Select(void);								//オブジェクト選択

	//変数
	int m_nChacePictNum;	//追従するピクトさんの番号
	bool m_bControllPict;	//ピクトさんを操縦するかどうか
};

#endif