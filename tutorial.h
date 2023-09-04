//======================================================
//
//チュートリアルシーンのヘッダ[tutorial.h]
//Author:石原颯馬
//
//======================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//タイトルクラス
class CTutorial : public CScene
{
public:
	//コンストラクタ・デストラクタ
	CTutorial();
	~CTutorial();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif // !_TUTORIAL_H_
