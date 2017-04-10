#pragma once

namespace sky { namespace lib { namespace graphic {

//�e���v���[�gTypedef�S
struct CMesh;
typedef SmartPointer< CMesh >	CMeshPtr;				//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CMeshPtr >	CMeshPtrList;			//���X�g��`

/// <summary>
/// �O�p�`�Q
/// </summary>
struct CMesh : public CSceneGraphNode
{
    SkyTypeDefRTTI;

//---Branch

	/// <summary>
	/// ���_�o�b�t�@
	/// </summary>
	CVertexBufferPtr  GetVertexBuffer( skyBool isRequired = skyTrue );
	void              AttachVertexBuffer( const CVertexBufferPtr &spVertex );

//---Branch

//---Leaf

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�擾�^�ݒ�
	/// </summary>
	CIndexBufferPtr  m_IndexBufferCash;
	CIndexBufferPtr  GetIndexBuffer( skyBool isRequired = skyTrue );
	void             AttachIndexBuffer( const CIndexBufferPtr &spIndexBuffer );

	/// <summary>
	/// �}�e���A���擾�^�ݒ�
	/// </summary>
	CMaterialContenaPtr m_MaterialContenaCash;
	CMaterialContenaPtr GetMaterialContena( skyBool isRequired = skyTrue );
	void                AttachMaterialContena( const CMaterialContenaPtr &spMaterialContena );

//---Leaf

	/// <summary>
	/// �X�P���g���擾�^�ݒ�
	/// </summary>
	math::CSkeletonPtr   GetSkeleton();
	void                 AttachSkeleton( const math::CSkeletonPtr &spSkeleton );

	/// <summary>
	/// �w���ID�Œ��_�X�g���[�����֘A�t����������
	/// </summary>
	void AttachStream( eVertexBufferStreamType eType , const CVertexBufferStreamPtr& stream );

	/// <summary>
	/// �w���ID�Œ��_�X�g���[�����֘A�t����������
	/// </summary>
	void DetachStream( eVertexBufferStreamType eType );

	/// <summary>
	/// �w���ID�Œ��_�X�g���[�����擾����
	/// </summary>
	CVertexBufferStreamPtr GetStream( eVertexBufferStreamType eType , skyBool isRequired = skyTrue );

	/// <summary>
	/// CPU���̃�������ɒ��_�o�b�t�@���폜����
	/// </summary>
	void DeleteCpuMemory();

	/// <summary>
	/// GPU���̃�������ɒ��_�o�b�t�@���쐬����
	/// </summary>
	void CreateGpuMemory();

	/// <summary>
	/// GPU���̃�������ɒ��_�o�b�t�@���폜����
	/// </summary>
	void DeleteGpuMemory();

	/// <summary>
	/// GPU���̃�������̒��_�o�b�t�@�ɏ�������
	/// </summary>
	void WirteGpuMemory();

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
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

//---Static

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static CMeshPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CMeshPtr spMesh( NEW__( CMesh , CMesh() ) );
		if ( name.IsEmpty() == skyFalse ) spMesh->Name.SetName( name );
		return spMesh;
	}

private:

	/// <summary>
	/// �_�~�[�l
	/// </summary>
	u32 m_Dummy;

	/// <summary>
	/// �W���C���g�p���b�g
	/// </summary>
	math::CSkeletonPtr m_spJoinbPallet;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CMesh(){}
};

#define CMeshCreate_()				sky::lib::graphic::CMesh::Create()
#define CMeshCreateName_( name )	sky::lib::graphic::CMesh::Create( name )

} } }