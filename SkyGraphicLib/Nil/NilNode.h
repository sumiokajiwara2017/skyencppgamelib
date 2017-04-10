#pragma once

namespace sky { namespace lib { namespace graphic {

//�e���v���[�gTypedef�S
class CNilNode;
typedef SmartPointer< CNilNode >	CNilNodePtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CNilNodePtr >	CNilNodePtrList;	//���X�g��`
typedef CHash< CNilNodePtr >		CNilNodePtrHash;	//Hash��`
typedef CHashMT< CNilNodePtr >		CNilNodePtrHashMT;	//Hash��`

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// ���m�[�h�B�������g��`�悵�Ȃ��BJoint�������Č����Ȃ��֐߂Ƃ��ē��삳����Ƃ��ł���
/// </summary>
class CNilNode : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
	virtual ~CNilNode(){};

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
	serialize::CSerializeInterfacePtr Clone();
	serialize::CSerializeInterfacePtr CloneSharing();

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
	static CNilNodePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CNilNodePtr spNil( NEW__( CNilNode , CNilNode() ) );
		if ( name.IsEmpty() == skyFalse ) spNil->Name.SetName( name );
		return spNil;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CNilNode();
};

} } } 


#define CNilNodeCreate_()			sky::lib::graphic::CNilNode::Create()
#define CNilNodeCreateName_( name )	sky::lib::graphic::CNilNode::Create( name )
