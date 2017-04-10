#pragma once

/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL �~������
OpenGL    PC Cg   �~������

*/

namespace sky { namespace lib { namespace graphic {

class   CDecal;
typedef SmartPointer< CDecal >		CDecalPtr;			//�X�}�[�g�|�C���^�^��`
typedef CList< u32 , CDecalPtr >	CDecalPtrList;		//���X�g��`

struct CDecalMember;

/// <summary>
/// �f�J�[���G�t�F�N�g�i�|���S���ɃV�[���|���S����\��j
/// </summary>
class CDecal : public CEffectObject
{
	friend class CDecalManager;
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CDecal();

	/// <summary>
	/// �e�N�X�`���֘A�t��
	/// </summary>
	void AttachTexture( const CTexturePtr &spTexture );

	/// <summary>
	/// �j�����Ԑݒ�
	/// </summary>
	void SetDestoryTime( updateTime destoryTime );

	/// <summary>
	/// �j������
	/// </summary>
	skyBool IsDestroy();

	/// <summary>
	/// �쐬
	/// </summary>
	void Create( const math::CBasicVector3 &pos , const math::CBasicVector3 &norm , const math::CBasicVector3 &tang , f32 w , f32 h , f32 d , const CMeshPtr &spMesh );

	/// <summary>
	/// �X�V
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// �`��
	/// </summary>
	virtual void vDraw();

private:

	/// <summary>
	/// �쐬
	/// </summary>
	static CDecalPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CDecalPtr spDecalt( NEW__( CDecal , CDecal() ) );
		if ( name.IsEmpty() == skyFalse ) spDecalt->Name.SetName( name );
		return spDecalt;
	}

	/// <summary>
	///�����o�ϐ�
	/// </summary>
	CDecalMember *m_pMember;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CDecal();

	/// <summary>
	///�`��i�����ƂɎ����j
	/// </summary>
	void DrawPass1();
};


} } }