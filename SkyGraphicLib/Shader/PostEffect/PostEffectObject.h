#pragma once

namespace sky { namespace lib { namespace graphic {

class CPostEffectObject;
typedef SmartPointer< CPostEffectObject >   CPostEffectObjectPtr;		    //�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CPostEffectObjectPtr > CPostEffectObjectPtrList;		//�X�}�[�g�|�C���^���X�g�^
/// <summary>
/// �|�X�g�G�t�F�N�g��{�N���X
/// </summary>
class CPostEffectObject : public base::SkyRefObject
{
    SkyTypeDefRTTI;

friend class PostEffectManager;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CPostEffectObject(){};

	/// <summary>
	/// �p�X���̎擾
	/// </summary>
	u32 GetPassNum();

	/// <summary>
	/// �L���ݒ�i�L�����ƃ|�X�g�G�t�F�N�g��������B�f�t�H���g�L���j
	/// </summary>
	void SetEnable( skyBool isEnable );

	/// <summary>
	/// �`��
	/// </summary>
	virtual void vRendering( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix ) = 0;

	/// <summary>
	/// ���̎����͋�����ɂ��Ă���
	/// </summary>
	void vDraw(){};

protected:

	/// <summary>
	/// �p�X��
	/// </summary>
	u32 m_PassNum;

	/// <summary>
	/// �L���t���O
	/// </summary>
	skyBool m_IsEnable;

    /// <summary>
	///  ���ݎg�p���Ă���V�F�[�_�[�������h�c
    /// </summary>
	u32	m_ShaderId;

    /// <summary>
	/// �V�F�[�_�[���
    /// </summary>
	CShaderInfoPtr m_spShaderInfo;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CPostEffectObject();
};

} } }
