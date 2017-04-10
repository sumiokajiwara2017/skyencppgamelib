#pragma once

/*

�y ������ �z
DirectX11 PC HLSL �~�ڐA���s�i�ꉞ�\�[�X�͎c���j
OpenGL    PC GLSL �~������
OpenGL    PC Cg   �~������

*/

namespace sky { namespace lib { namespace graphic {

class   CBeamChargeEffect;
typedef SmartPointer< CBeamChargeEffect >		CBeamChargeEffectPtr;			//�X�}�[�g�|�C���^�^��`

struct CBeamChargeEffectMember;

/// <summary>
/// �r�[���`���[�W�G�t�F�N�g�N���X
/// </summary>
class CBeamChargeEffect : public CEffectObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CBeamChargeEffect();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void vDraw();

	/// <summary>
	/// ���\�[�X�̓ǂݍ��݁i�Q�d�ǂݍ��݂͓����ŃX���[���܂��j
	/// </summary>
	void Load();                //�C���X�^���X���ɕK�v�ȃ��\�[�X�p
	static void LoadStatic();   //�N���X���ɕK�v�ȃ��\�[�X�p

	/// <summary>
	/// ���\�[�X�̔j��
	/// </summary>
	void Destroy();
	static void DestroyStatic();

	/// <summary>
	/// �쐬
	/// </summary>
	static CBeamChargeEffectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CBeamChargeEffectPtr spBeamChargeEffectt( NEW__( CBeamChargeEffect , CBeamChargeEffect() ) );
		if ( name.IsEmpty() == skyFalse ) spBeamChargeEffectt->Name.SetName( name );
		return spBeamChargeEffectt;
	}

private:

	/// <summary>
	///�����o�ϐ�
	/// </summary>
	CBeamChargeEffectMember *m_pMember;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CBeamChargeEffect();

	/// <summary>
	///�`��i�����ƂɎ����j
	/// </summary>
	void DrawPass1();
};

} } }

#define CBeamChargeEffectObjectCreate_()		sky::lib::graphic::CBeamChargeEffect::Create()
#define CBeamChargeEffectCreateName_( name )	sky::lib::graphic::CBeamChargeEffect::Create( name )
