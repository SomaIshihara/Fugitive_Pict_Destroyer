//==========================================
//
//カメラプログラムのヘッダ[camera.h]
//Author:石原颯馬
//
//==========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define CAMERA_MOVE_SPEED		(10.0f)		//カメラ移動速度
#define CAMERA_MOU_ROT_SPEED	(0.0012f)	//マウス移動での回転速度

//クラス化したものがこちらです
class CCamera
{
public:
	//コンストラクタ・デストラクタ
	CCamera();
	~CCamera();
	
	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//設定処理
	void SetCamera(void);

	//位置移動
	void SetPos(const D3DXVECTOR3 move);
	void SetRot(const D3DXVECTOR3 rot);

	//設定
	void SetLength(const float fLength) { m_fLength = fLength; }

	//取得
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	//リセット
	void ResetPos(void);
private:
	//角度修正
	void FixPosV(void);
	void FixPosR(void);
	void FixRot(void);

	//メンバ変数
	D3DXMATRIX m_mtxView;		//ビューマトリ
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリ
	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posR;			//注視点
	D3DXVECTOR3 m_vecU;			//上ベクトル
	D3DXVECTOR3 m_rot;			//角度
	float m_fLength;			//距離
};

#endif // !_CAMERA_H_
