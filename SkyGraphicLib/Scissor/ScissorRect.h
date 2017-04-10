#pragma once

namespace sky { namespace lib { namespace graphic {

namespace scissor
{
	//�V�U�[��`�ő吔
	const u32 SCISSOR_MAX_NUM = 16;
}

//�e���v���[�gTypedef�S
class CScissorRect;
typedef SmartPointer< CScissorRect >	CScissorRectPtr;		//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �V�U�����O��`�N���X
/// </summary>
class CScissorRect : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CScissorRect(){}

	/// <summary>
	/// �V�U�[��`�̐ݒ�
	/// </summary>
	void				        SetRect        ( const math::CBasicRect &rect );

	/// <summary>
	/// �V�U�[��`�̐ݒ�
	/// </summary>
	math::CBasicRect&           GetRect        (){ return m_Rect; }

	/// <summary>
	/// �T�C�Y�ύX
	/// </summary>
	void                        ReSizeSc       ( dectype wSc , dectype hSc );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	virtual void		        vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���㏈��
	/// </summary>
	virtual void                vPostRendering  ( CRenderingParam &param );

	/// <summary>
	/// �V�U�����O��`�������_�����O�V�X�e���ɐݒ肷��i�ȍ~�V�U�����O��`�͈͓̔��Ƀ����_�����O���s����j
	/// </summary>
	void                        SetRenderer();

	/// <summary>
	/// �����_�����O�V�X�e���̃V�U�����O��`���N���A����
	/// </summary>
	void                        ClearRenderer();

	/// <summary>
	/// Window�T�C�Y�̐ݒ�(OpenGL�ł͍������_�̂��߁A�������_���W�����㌴�_���W�ɕϊ�����ɂ͑S�̂̑傫�����K�v�ɂȂ�j
	/// </summary>
	void                        SetWindowSize( const math::CBasicSize size ){ m_WindowSize = size; };

//---Serialize
			
	/// <summary>
	/// �o�C�i���ɃZ�[�u
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// �o�C�i�����烍�[�h
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������
	/// </summary>
	serialize::CSerializeInterfacePtr 		  Clone();
	serialize::CSerializeInterfacePtr         CloneSharing();

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CScissorRectPtr	Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CScissorRectPtr spScissorRect( NEW__( CScissorRect , CScissorRect() ) );
		if ( name.IsEmpty() == skyFalse ) spScissorRect->Name.SetName( name );
		return spScissorRect;
	}

#ifdef SW_SKYLIB_DEBUG_ON

    /// <summary>
	///  �f�o�b�O�v�����g
    /// </summary>
	virtual void NodeTreePrint( u32 indent = 0 , eRelationKind eKind = eRelationKind_Root  );

#endif

private:

	/// <summary>
	/// ��`���X�g
	/// </summary>
	math::CBasicRect           m_Rect;

	/// <summary>
	/// �������������Ă���Window�̕��ƍ����i�V�[���O���t�̃g���o�[�X���Ɏ擾�j
	/// </summary>
	math::CBasicSize           m_WindowSize;

};

} } }

#define CScissorRectCreate_()	sky::lib::graphic::CScissorRect::Create()
#define CScissorRectCreateName_( name ) sky::lib::graphic::CScissorRect::Create( name )
