//======================================================
//
//�`���[�g���A���V�[���̃w�b�_[tutorial.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "manager.h"

//�O���錾
class CPlayer;
class CMeshField;
class CSlider;
class CPause;
class CScore;
class CHaveNum;
class CMeshSky;
class CBG;
class CFade;

//�^�C�g���N���X
class CTutorial : public CScene
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CTutorial();
	~CTutorial();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	static CScore* GetScoreObj(void) { return m_pScore; }
	static CMeshSky* GetSkyObj(void) { return m_pSky; }
	CSlider* GetSlider(void) { return m_pSlider; }
	static CHaveNum** GetHaveNumObj(void) { return &m_pHaveNum[0]; }

private:
	void CulcScore(void);	//�X�R�A�v�Z
	static CPlayer* m_pPlayer;			//�v���C���[
	CSlider* m_pSlider;					//�X���C�_�[
	static CMeshField* m_pMeshField;	//���b�V���t�B�[���h
	static CScore* m_pScore;			//�X�R�A
	static CHaveNum* m_pHaveNum[3];		//������
	static CMeshSky* m_pSky;			//��I�u�W�F
	CBG* m_pWarning;				//�̗͌x���̔w�i�ł͂Ȃ����
	CFade* m_pFade;
};

#endif // !_TUTORIAL_H_
