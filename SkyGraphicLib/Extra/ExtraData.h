#pragma once

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< ���� >---------------------------------------------//

//�e���v���[�gTypedef�S
class CExtraData;
typedef SmartPointer< CExtraData >			CExtraDataPtr;
typedef CList< u32 , CExtraData >			CExtraDataPtrList;

class CExtraData : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CExtraData(){};

//---Member

	/// <summary>
	/// double
	/// </summary>
	d64 m_DoubleValue;

	/// <summary>
	/// int
	/// </summary>
	s32 m_IntValue;

//---Member

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	void vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���㏈��
	/// </summary>
	void vPostRendering   ( CRenderingParam &param );

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

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CExtraDataPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CExtraDataPtr spExtraData( NEW__( CExtraData , CExtraData() ) );
		if ( name.IsEmpty() == skyFalse ) spExtraData->Name.SetName( name );
		return spExtraData;
	}

protected:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CExtraData(){};
};

} } }

#define CExtraDataCreate_()	          sky::lib::graphic::CExtraData::Create()
#define CExtraDataCreateName_( name ) sky::lib::graphic::CExtraData::Create( name )
