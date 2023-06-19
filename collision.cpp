//======================================================
//
//�����蔻�菈����[collision.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "collision.h"
#include "Culc.h"
#include "manager.h"
#include "objectBillboard.h"

//=================================
//�R���X�g���N�^
//=================================
CCollision::CCollision()
{
	m_vtxMin = VEC3_ZERO;
	m_vtxMax = VEC3_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CCollision::~CCollision()
{
}

//=================================
//�����蔻��`�F�b�N
//=================================
bool CCollision::CollisionCheck(const D3DXVECTOR3 posOld, const D3DXVECTOR3 posNew, const D3DXVECTOR3 posCenter, const D3DXVECTOR3 rot)
{
	//�O����
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z)) == true)
	{
		return true;
	}

	//�E����
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z)) == true)
	{
		return true;
	}

	//������
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMax.z),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMax.z)) == true)
	{
		return true;
	}

	//������
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMax.z)) == true)
	{
		return true;
	}

	//�㑤��
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMax.z)) == true)
	{
		return true;
	}

	//������
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(0.0f, -1.0f, 0.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMax.z)) == true)
	{
		return true;
	}

	return false;
}

//=================================
//�|���S�����Ƃ̓����蔻��`�F�b�N
//=================================
bool CCollision::CollisionPolygon(const D3DXVECTOR3 posOld, const D3DXVECTOR3 posNew, const D3DXVECTOR3 posCenter, const D3DXVECTOR3 rot,
	const D3DXVECTOR3 vecNor, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3)
{
	//���_�̉�]�ɕK�v�ȕϐ�
	//�e���_�����߂�̂ɕK�v�ȕϐ�
	D3DXMATRIX mtxWorld;	//��]�̃x�[�X�ƂȂ郏�[���h�}�g��
	D3DXMATRIX mtxRot;		//��]�s��
	D3DXMATRIX mtxTrans;	//���ׂĕϊ���̍s��
	D3DXVECTOR3 vtxTrans;	//�ϊ���̓_

	//���_�̉�]
	//-mtx----------------------------------------------------------------------------------------------------------------------------
	//��]�s������
	D3DXMatrixIdentity(&mtxWorld);

	//�������f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixRotationY(&mtxRot, rot.y);
	D3DXMatrixMultiply(&mtxWorld, &mtxRot, &mtxWorld);

	//�ʒu���f
	mtxWorld._41 = posCenter.x;
	mtxWorld._42 = posCenter.y;
	mtxWorld._43 = posCenter.z;

	//-mtx----------------------------------------------------------------------------------------------------------------------------

	//-pos0---------------------------------------------------------------------------------------------------------------------------
	//��]�s������Ƃɒ��_����]����
	//�x�[�X���R�s�[
	mtxTrans = mtxWorld;

	//0�x�̂Ƃ��̓_��u��
	D3DXVECTOR3 vtxPos = pos0;

	//��]�s��Ƃ������킹��
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	pos0.x = vtxTrans.x;
	pos0.y = vtxTrans.y;
	pos0.z = vtxTrans.z;
	//-pos0---------------------------------------------------------------------------------------------------------------------------

	//-pos1---------------------------------------------------------------------------------------------------------------------------
	//��]�s������Ƃɒ��_����]����
	//�x�[�X���R�s�[
	mtxTrans = mtxWorld;

	//0�x�̂Ƃ��̓_��u��
	vtxPos = pos1;

	//��]�s��Ƃ������킹��
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//�ϊ���̓_�̏ꏊ����
	pos1.x = vtxTrans.x;
	pos1.y = vtxTrans.y;
	pos1.z = vtxTrans.z;
	//-pos1---------------------------------------------------------------------------------------------------------------------------

	//-pos2---------------------------------------------------------------------------------------------------------------------------
	//��]�s������Ƃɒ��_����]����
	//�x�[�X���R�s�[
	mtxTrans = mtxWorld;

	//0�x�̂Ƃ��̓_��u��
	vtxPos = pos2;

	//��]�s��Ƃ������킹��
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//�ϊ���̓_�̏ꏊ����
	pos2.x = vtxTrans.x;
	pos2.y = vtxTrans.y;
	pos2.z = vtxTrans.z;
	//-pos2---------------------------------------------------------------------------------------------------------------------------

	//-pos3---------------------------------------------------------------------------------------------------------------------------
	//��]�s������Ƃɒ��_����]����
	//�x�[�X���R�s�[
	mtxTrans = mtxWorld;

	//0�x�̂Ƃ��̓_��u��
	vtxPos = pos3;

	//��]�s��Ƃ������킹��
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//�ϊ���̓_�̏ꏊ����
	pos3.x = vtxTrans.x;
	pos3.y = vtxTrans.y;
	pos3.z = vtxTrans.z;
	//-pos3---------------------------------------------------------------------------------------------------------------------------

	//�����蔻��
	if (D3DXVec3Dot(&posOld, &vecNor) * D3DXVec3Dot(&posNew, &vecNor) <= 0)
	{
		//�|���S���ђʂ��Ă��邩
		D3DXVECTOR3 ray = posNew - posOld;			//���C
		D3DXVec3Normalize(&ray, &ray);				//���C�𐳋K��
		float fU, fV, fD;							//���ݔ�

		if (D3DXIntersectTri(&pos0, &pos1, &pos3, &posOld, &ray, &fU, &fV, &fD) == true || D3DXIntersectTri(&pos1, &pos2, &pos3, &posOld, &ray, &fU, &fV, &fD) == true)
		{//�l�p�`���O�p�`�ɕ������A�ǂ��炩true�A���Ă�����I���ł���
			return true;
		}
	}
	
	return false;
}