#pragma once

namespace sky { namespace lib { namespace graphic {

class CMeshContena;
typedef SmartPointer< CMeshContena >			CMeshContenaPtr;				//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CMeshContenaPtr >		CMeshContenaPtrList;			//���X�g��`
typedef CHash< CMeshContenaPtr >			    CMeshContenaPtrHash;			//�n�b�V����`

/// <summary>
/// ���b�V���N���X
/// </summary>
class CMeshContena : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CMeshContena(){}

    /// <summary>
	/// �����_�����O����
    /// </summary>
	virtual void vRendering ( CRenderingParam &param );

//---Leaf

	/// <summary>
	/// �􉽊w�`����̍쐬
	/// </summary>
	void     AttachMesh( const CMeshPtr &spMesh );
	CMeshPtr GetMesh( u32 meshNo = 0 );
	void     DeleteAllMesh();
	u32      GetMeshNum() const;

//---Leaf

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
	/// ���s�\��Ԃɂ���
	/// </summary>
	virtual void Activate();

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	virtual void Deactivate();

//---Static

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static CMeshContenaPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CMeshContenaPtr spMesh( NEW__( CMeshContena , CMeshContena() ) );
		if ( name.IsEmpty() == skyFalse ) spMesh->Name.SetName( name );
		return spMesh;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CMeshContena(){}

};

} } }

#define CMeshContenaCreate_()				sky::lib::graphic::CMeshContena::Create()
#define CMeshContenaCreateName_( name )	sky::lib::graphic::CMeshContena::Create( name )

