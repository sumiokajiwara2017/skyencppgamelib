#pragma once

/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL �~������
OpenGL    PC Cg   �~������

*/

namespace sky { namespace lib { namespace graphic {

class   CBillboardBeam;
typedef SmartPointer< CBillboardBeam >		CBillboardBeamPtr;			//�X�}�[�g�|�C���^�^��`

struct CBillboardBeamMember;

/// <summary>
/// �r�[���`���[�W�G�t�F�N�g�N���X
/// </summary>
class CBillboardBeam : public CEffectObject
{
	friend class CBillboardBeamUtil;
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CBillboardBeam();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void vDraw();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize( vrtxtype nodeNum , dectype width , const CTexturePtr &spTexture );

	/// <summary>
	/// ������
	/// </summary>
	void Dispose();

	/// <summary>
	/// �m�[�h���W�Z�b�g
	/// </summary>
	void SetNodePos( u32 no , const math::CBasicVector3 &pos );

	/// <summary>
	/// �쐬
	/// </summary>
	static CBillboardBeamPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CBillboardBeamPtr spBillboardBeamt( NEW__( CBillboardBeam , CBillboardBeam() ) );
		if ( name.IsEmpty() == skyFalse ) spBillboardBeamt->Name.SetName( name );
		return spBillboardBeamt;
	}

private:

	/// <summary>
	///�����o�ϐ�
	/// </summary>
	CBillboardBeamMember *m_pMember;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CBillboardBeam();

	/// <summary>
	///�`��i�����ƂɎ����j
	/// </summary>
	void DrawPass1();
};

/// <summary>
/// �r�[���`���[�W�G�t�F�N�g�֗������Q
/// </summary>
class CBillboardBeamUtil
{

public:

	/// <summary>
	/// ���̓���
	/// </summary>
	static void Move_Thunder( const CBillboardBeamPtr &spEffect );

	/// <summary>
	/// ��]
	/// </summary>
	static void Move_Rot( const CBillboardBeamPtr &spEffect );
};


} } }