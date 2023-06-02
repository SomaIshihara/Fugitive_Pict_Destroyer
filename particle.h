//======================================================
//
//�p�[�e�B�N�������̃w�b�_[particle.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"
#include "manager.h"
#include "object.h"

#define MAX_EFFECT	(1024)	//�G�t�F�N�g�ő��
class CEffect;

class CParticle : public CObject	//���̃N���X�͊Ǘ��I�u�W�F�N�g�ł��BUnity�ł�����̃I�u�W�F�N�g�݂����Ȃ��́B
{
public:
	//�p�[�e�B�N���\���̒�`
	typedef struct
	{
		//�p�[�e�B�N���֌W
		D3DXVECTOR3 pos;		//�p�[�e�B�N�����o�ʒu
		int nLife;				//�p�[�e�B�N���̎���
		int nEffeceNum;			//��x�ɏo���G�t�F�N�g��
		float fSpeedBace;		//�X�s�[�h��{�l
		float fSpeedDegree;		//�X�s�[�h�U�ꕝ

		//�G�t�F�N�g�֌W
		D3DXCOLOR col;			//�G�t�F�N�g�̐F
		float fWidth;			//�G�t�F�N�g1�̕�
		float fHeight;			//�G�t�F�N�g1�̍���
	} Particle;
	//�R���X�g���N�^�E�f�X�g���N�^
	CParticle();				//�f�t�H���g
	CParticle(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
		const D3DXCOLOR col, const float fWidth, const float fHeight);				//�I�[�o�[���[�h
	~CParticle();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ǂݍ���
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//����
	static CParticle* Create(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
		const D3DXCOLOR col, const float fWidth, const float fHeight);	//�I�u�W�F�N�g�𐶐� fSpeed:�X�N���[�����x

	//�擾�i�������z�֐��̊֌W��������Ă��邪�A�����Ɉʒu���T�C�Y���͂���Ȃ��̂ł��ׂ�0��Ԃ��j
	D3DXVECTOR3 GetPos(void) { return VEC3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return VEC3_ZERO; }
	float GetWidth(void) { return FLOAT_ZERO; }
	float GetHeight(void) { return FLOAT_ZERO; }

private:
	//CEffect* m_pEffect[MAX_EFFECT];			//�G�t�F�N�g
	Particle m_particle;					//�p�[�e�B�N���\����
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
};

#endif // !_MULTIPLE_BG_H_