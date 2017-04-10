#pragma once

namespace sky { namespace lib { namespace graphic {

class CMaterialDrawData;
typedef SmartPointer< CMaterialDrawData >		CMaterialDrawDataPtr;	  //�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �`��p�}�e���A���f�[�^
/// </summary>
struct CMaterialDrawData : public CSceneGraphNode
{

public:

//--Branch

	/// <summary>
	/// �}�e���A���̎擾
	/// </summary>
	CMaterialPtr GetMaterial( skyBool isRequired = skyTrue );

//--Branch

//---Serialize

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static CMaterialDrawDataPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CMaterialDrawDataPtr spMaterialDrawData( NEW__( CMaterialDrawData , CMaterialDrawData() ) );
		if ( name.IsEmpty() == skyFalse ) spMaterialDrawData->Name.SetName( name );
		return spMaterialDrawData;
	}

	/// <summary>
	/// �`��J�n���_�o�b�t�@�C���f�b�N�X
	/// </summary>
	vrtxtype     m_DrawStartIndex;

	/// <summary>
	/// �`��I�����_�o�b�t�@�C���f�b�N�X
	/// </summary>
	vrtxtype     m_DrawEndIndex;

};

#define CMaterialDrawDataCreate_()				                              sky::lib::graphic::CMaterialDrawData::Create()
#define CMaterialDrawDataCreateName_( name )	                              sky::lib::graphic::CMaterialDrawData::Create( name )

class CMaterialContena;
typedef SmartPointer< CMaterialContena >		CMaterialContenaPtr;	  //�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �}�e���A���R���e�i�N���X
/// </summary>
class CMaterialContena : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

//----Leaf

	/// <summary>
	/// �}�e���A���ǉ�
	/// </summary>
	void AddMaterial( const CMaterialPtr &spMaterial , vrtxtype startIndex , vrtxtype endIndex );

	/// <summary>
	/// �}�e���A���̎擾
	/// </summary>
	CMaterialPtr GetMaterial( u32 index = 0 );

	/// <summary>
	/// �}�e���A�����̎擾
	/// </summary>
	CMaterialDrawDataPtr GetMaterialData( u32 index = 0 );

	/// <summary>
	/// �`��C���f�b�N�X�͈͂̐ݒ�
	/// </summary>
	void SetDrawIndex(  u32 index , vrtxtype drawStartIndex , vrtxtype drawEndIndex );

	/// <summary>
	/// �}�e���A�����X�g�̎Q�Ǝ擾�i����Ŏ�����FOREACH���Ăˁj
	/// </summary>
	CSceneGraphNodePtrList &GetMaterialDataArray();

//----Leaf

//---Serialize

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

//---Static

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static CMaterialContenaPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CMaterialContenaPtr spMaterialContena( NEW__( CMaterialContena , CMaterialContena() ) );
		if ( name.IsEmpty() == skyFalse ) spMaterialContena->Name.SetName( name );
		return spMaterialContena;
	}
	static CMaterialContenaPtr Create( const CMaterialPtr &spMaterial , vrtxtype endIndex )
	{
		CMaterialContenaPtr spMaterialContena( NEW__( CMaterialContena , CMaterialContena() ) );
		spMaterialContena->AddMaterial( spMaterial , 0 , endIndex );

		return spMaterialContena;
	}
};

} } }

#define CMaterialContenaCreate_()				                              sky::lib::graphic::CMaterialContena::Create()
#define CMaterialContenaCreateName_( name )	                                  sky::lib::graphic::CMaterialContena::Create( name )
#define CMaterialContenaCreateSetMaterial_( spMaterial , endIndex )           sky::lib::graphic::CMaterialContena::Create( spMaterial , endIndex )
