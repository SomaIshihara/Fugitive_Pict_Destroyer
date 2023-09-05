//======================================================
//
//パーティクル処理のヘッダ[particle.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"
#include "manager.h"
#include "object.h"

#define MAX_EFFECT	(1024)	//エフェクト最大個数

//========================================
//2Dパーティクル
//========================================
class CParticle2D : public CObject	//このクラスは管理オブジェクトです。Unityでいう空のオブジェクトみたいなもの。
{
public:
	//パーティクル構造体定義
	typedef struct
	{
		//パーティクル関係
		D3DXVECTOR3 pos;		//パーティクル放出位置
		int nLife;				//パーティクルの寿命
		int nEffeceNum;			//一度に出すエフェクト数
		float fSpeedBace;		//スピード基本値
		float fSpeedDegree;		//スピード振れ幅

		//エフェクト関係
		D3DXCOLOR col;			//エフェクトの色
		float fWidth;			//エフェクト1個の幅
		float fHeight;			//エフェクト1個の高さ
	} Particle;
	//コンストラクタ・デストラクタ
	CParticle2D();				//デフォルト
	CParticle2D(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
		const D3DXCOLOR col, const float fWidth, const float fHeight);				//オーバーロード
	~CParticle2D();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {}

	//生成
	static CParticle2D* Create(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
		const D3DXCOLOR col, const float fWidth, const float fHeight);	//オブジェクトを生成 fSpeed:スクロール速度

	//取得（純粋仮想関数の関係上実装しているが、こいつにサイズやらはいらないのですべて0を返す）
	D3DXVECTOR3 GetPos(void) { return m_particle.pos; }
	D3DXVECTOR3 GetRot(void) { return CManager::VEC3_ZERO; }
	float GetWidth(void) { return CManager::FLOAT_ZERO; }
	float GetHeight(void) { return CManager::FLOAT_ZERO; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }
	int GetModelIdx(void) { return -1; }

private:
	Particle m_particle;	//パーティクル構造体
};

//========================================
//ビルボードパーティクル
//========================================
class CParticleBillboard : public CObject	//このクラスは管理オブジェクトです。Unityでいう空のオブジェクトみたいなもの。
{
public:
	//パーティクル構造体定義
	typedef struct
	{
		//パーティクル関係
		D3DXVECTOR3 pos;		//パーティクル放出位置
		int nLife;				//パーティクルの寿命
		int nEffeceNum;			//一度に出すエフェクト数
		float fSpeedBace;		//スピード基本値
		float fSpeedDegree;		//スピード振れ幅

								//エフェクト関係
		D3DXCOLOR col;			//エフェクトの色
		float fWidth;			//エフェクト1個の幅
		float fHeight;			//エフェクト1個の高さ
	} Particle;
	//コンストラクタ・デストラクタ
	CParticleBillboard();				//デフォルト
	CParticleBillboard(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
		const D3DXCOLOR col, const float fWidth, const float fHeight);				//オーバーロード
	~CParticleBillboard();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {}

	//生成
	static CParticleBillboard* Create(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
		const D3DXCOLOR col, const float fWidth, const float fHeight);	//オブジェクトを生成 fSpeed:スクロール速度

	//取得（純粋仮想関数の関係上実装しているが、こいつにサイズやらはいらないのですべて0を返す）
	D3DXVECTOR3 GetPos(void) { return m_particle.pos; }
	D3DXVECTOR3 GetRot(void) { return CManager::VEC3_ZERO; }
	float GetWidth(void) { return CManager::FLOAT_ZERO; }
	float GetHeight(void) { return CManager::FLOAT_ZERO; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }
	int GetModelIdx(void) { return -1; }

private:
	Particle m_particle;	//パーティクル構造体
};

#endif // !_MULTIPLE_BG_H_