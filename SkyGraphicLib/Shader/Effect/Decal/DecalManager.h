#pragma once

namespace sky { namespace lib { namespace graphic {

class   CDecalManager;
typedef SmartPointer< CDecalManager >		CDecalManagerPtr;			//�X�}�[�g�|�C���^�^��`

struct CDecalManagerMember;

/// <summary>
/// �f�J�[���G�t�F�N�g�Ǘ�
/// </summary>
class CDecalManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CDecalManager();

	/// <summary>
	/// �f�J�[����\�����Ώۂ̃��f����o�^
	/// </summary>
	void AttachModel( const CModelPtr &spModel );

	/// <summary>
	/// �f�J�[����\�����Ώۂ̃��f����o�^�j��
	/// </summary>
	void DetachModel( const CModelPtr &spModel );

	/// <summary>
	/// �f�J�[���쐬
	/// tang�͐ڃx�N�g���Bnorm�������Ȃ�tang�͎��������ɑ΂��Đ^���������x�N�g����ݒ肵�Ă�������
	/// </summary>
	void CreateDecal( const CTexturePtr &spTexture , const math::CBasicVector3 &pos , const math::CBasicVector3 &norm , math::CBasicVector3 &tang , f32 distance , f32 width , f32 height , f32 depth , updateTime destoryTime );

	/// <summary>
	/// �f�J�[���j��
	/// </summary>
	void DestoryDecal( const CDecalPtr &spDecal );

	/// <summary>
	/// �f�J�[���S�j��
	/// </summary>
	void DestoryAllDecal();

	/// <summary>
	/// �\�������
	/// </summary>
	void DrawDecal( CRenderingParam &param );

	/// <summary>
	/// �쐬
	/// </summary>
	static CDecalManagerPtr   Create()
	{
		CDecalManagerPtr spDecalt( NEW__( CDecalManager , CDecalManager() ) );
		return spDecalt;
	}

private:

	/// <summary>
	///�����o�ϐ�
	/// </summary>
	CDecalManagerMember *m_pMember;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CDecalManager();
};

} } }