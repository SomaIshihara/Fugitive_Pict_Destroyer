//======================================================
//
//�Q�[���V�[���̃w�b�_[game.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _GAME_H_
#define _GAME_H_
#include "manager.h"

//�O���錾
class CPlayer;
class CMeshField;
class CSlider;
class CPause;
class CTimer;
class CScore;

//�^�C�g���N���X
class CGame : public CScene
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CGame();
	~CGame();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CSlider* GetSlider(void) { return m_pSlider; }
	static CMeshField* GetMeshField(void) { return m_pMeshField; }

private:
	CPause* m_pPause;					//�|�[�Y
	static CPlayer* m_pPlayer;			//�v���C���[
	static CSlider* m_pSlider;			//�X���C�_�[
	static CMeshField* m_pMeshField;	//���b�V���t�B�[���h
	static CTimer* m_pTimer;			//�^�C�}�[
	static CScore* m_pScore;			//�X�R�A
};

#endif // !_GAME_H_
