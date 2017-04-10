#pragma once

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)
struct CDepthStencilBufferMember;

//�e���v���[�gTypedef�S
class CDepthStencilBuffer;
typedef SmartPointer< CDepthStencilBuffer >	  CDepthStencilBufferPtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CDepthStencilBufferPtr > CDepthStencilBufferPtrList;

/// <summary>
/// �[�x�X�e���V���m�[�h
/// </summary>
class CDepthStencilBuffer : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual               ~CDepthStencilBuffer ();

	/// <summary>
	/// ����
	/// </summary>
	void                  SetUp       ( const math::CBasicSize &size ); 

	/// <summary>
	/// �T�C�Y�̎擾
	/// </summary>
	math::CBasicSize      GetSize        (){ return m_Size; }

	/// <summary>
	/// �T�C�Y�̐ݒ�
	/// </summary>
	void                  SetSize        ( const math::CBasicSize &size ){ m_Size = size; }

	/// <summary>
	/// �T�C�Y�̕ύX
	/// </summary>
	void                  ReSize         ( u32 newWidth , u32 newHeight );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	void				  vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���㏈��
	/// </summary>
	void				  vPostRendering   ( CRenderingParam &param );

	/// <summary>
	/// �J��
	/// </summary>
	void                   Release();

	/// <summary>
	/// �[�x�^�X�e���V���o�b�t�@�̃N���A
	/// </summary>
    void                   ClearDepthStencilBuffer();

	/// <summary>
	/// �[�x�o�b�t�@�̃N���A
	/// </summary>
    void                   ClearDepthBuffer();

	/// <summary>
	/// �X�e���V���o�b�t�@�̃N���A
	/// </summary>
    void                   ClearStencilBuffer();

	/// <summary>
	/// �[�x�^�X�e���V�����e�N�X�`���Ƃ��Ď擾����B�����SetUpFrameBuffer�Ő������ꂽ�����_�����O�^�[�Q�b�g�ȊO�s���Ȃ��j
	/// </summary>
	CTexturePtr            GetDepthStencilTexture();

	/// <summary>
	/// �����o�ϐ��̎擾
	/// </summary>
	inline CDepthStencilBufferMember* GetMember(){ return m_pMember; }


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
	serialize::CSerializeInterfacePtr 		  CloneSharing();

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	void Activate();

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	void Deactivate();

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CDepthStencilBufferPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CDepthStencilBufferPtr spDepthStencilBuffer( NEW__( CDepthStencilBuffer , CDepthStencilBuffer() ) );
		if ( name.IsEmpty() == skyFalse ) spDepthStencilBuffer->Name.SetName( name );
		return spDepthStencilBuffer;
	}

protected:

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CDepthStencilBufferMember*    m_pMember;

//---Member

	/// <summary>
	/// ��ʏc���̒���
	/// </summary>
	math::CBasicSize	          m_Size;

//---Member

	/// <summary>
	/// �[�x�o�b�t�@�e�N�X�`��
	/// </summary>
	CTexturePtr                   m_spTexture;

	/// <summary>
	/// ��������N���[�������C���X�^���X�̃��X�g
	/// </summary>
	CDepthStencilBufferPtrList    m_spCloneList;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CDepthStencilBuffer();

	/// <summary>
	/// �ċA�����[�X
	/// </summary>
   static void Release_( CDepthStencilBuffer *pNode );

	/// <summary>
	/// �ċA�A�^�b�`
	/// </summary>
   static void Rettach_( CDepthStencilBuffer *pNode );

};

} } }

#define CDepthStencilBufferCreate_()	        sky::lib::graphic::CDepthStencilBuffer::Create()
#define CDepthStencilBufferCreateName_( name )	sky::lib::graphic::CDepthStencilBuffer::Create( name )

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

typedef SmartPointerCom< ID3D11DepthStencilView >    D3D11DepthStencilViewPtr;

struct CDepthStencilBufferMember
{
	/// <summary>
	/// �[�x�^�X�e���V���r���[
	/// </summary>
	D3D11DepthStencilViewPtr            m_spDepthStencilView;
};

} } }

#endif