#pragma once

namespace sky { namespace lib { namespace graphic {

//===================================�yCommon�z====================================================>

//�e���v���[�gTypedef�S

class CViewPort;
typedef SmartPointer< CViewPort >				CViewPortPtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList		< u32 , CViewPortPtr >		CViewPortPtrList;	//���X�g��`
typedef CHash		< CViewPortPtr >			CViewPortPtrHash;	//Hash��`
typedef CHashMT		< CViewPortPtr >			CViewPortPtrHashMT;	//Hash��`

class CViewPort : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual                     ~CViewPort(){};

	/// <summary>
	/// ���ƍ����ƈʒu��ݒ肷��i��`�̃T�C�Y�ύX�́A�o�^������`�|�C���^�̒l��ύX����Ες��܂��B�폜������A�傫����ω������邽�߂ɓo�^�����Q�Ƃ͕ێ����Ă����Ă��������B�j
	/// </summary>
	void				        SetRect        ( const math::CBasicRect &rect );

	/// <summary>
	/// �T�C�Y�擾
	/// </summary>
	const math::CBasicRect&     GetRect        ();

	/// <summary>
	/// �T�C�Y�ύX
	/// </summary>
	void                        ReSize         ( u32 width , u32 height );
	void                        ReSizeSc       ( dectype wSc , dectype hSc );

	/// <summary>
	/// �[�x�̍ő�l�A�ŏ��l��ݒ�
	/// </summary>
	void                        SetDepth        ( dectype minDepth , dectype maxDepth );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	virtual void		        vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���㏈��
	/// </summary>
	virtual void                vPostRendering  ( CRenderingParam &param );

	/// <summary>
	/// �r���[�|�[�g�������_�����O�V�X�e���ɐݒ肷��i�ȍ~�r���[�|�[�g�͈͓̔��Ƀ����_�����O���s����j
	/// </summary>
	void                        SetRenderer();

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
	static CViewPortPtr	Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CViewPortPtr spViewPort( NEW__( CViewPort , CViewPort() ) );
		if ( name.IsEmpty() == skyFalse ) spViewPort->Name.SetName( name );
		return spViewPort;
	}

protected:

//---Member

	/// <summary>
	/// �`��̈��`
	/// </summary>
	math::CBasicRect    m_Rect;

	/// <summary>
	/// �`��[���ŏ��l
	/// </summary>
	dectype             m_MinDepth;

	/// <summary>
	/// �`��[���ő�l
	/// </summary>
	dectype             m_MaxDepth;

//---Member

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CViewPort() : m_MinDepth( 0.0f ) , m_MaxDepth( 1.0f ){}

};

#define CViewPortCreate_()	         sky::lib::graphic::CViewPort::Create()
#define CViewPortCreateName_( name ) sky::lib::graphic::CViewPort::Create( name )

} } }