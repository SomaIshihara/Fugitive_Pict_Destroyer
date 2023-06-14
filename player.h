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

private:
	//関数
	void Move(void);								//カメラ移動処理
	void Rotate(void);								//カメラ回転処理
	D3DXVECTOR3 GenerateClickRay(void);				//クリックした座標のレイ生成
	D3DXVECTOR3 ConvertClickPosToWorld(float fZ);	//クリック座標をワールド座標に変換
	void Select(void);								//オブジェクト選択

	//変数
};

#endif