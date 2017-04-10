#pragma once

namespace sky { namespace lib { namespace graphic {

class   CEffectObject;
typedef SmartPointer< CEffectObject >		CEffectObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// �G�t�F�N�g�I�u�W�F�N�g�N���X
/// </summary>
class CEffectObject : public CSceneGraphNode
{

public:

    SkyTypeDefRTTI;

	/// <summary>
	/// �p�X���̎擾
	/// </summary>
	inline u32 GetPassNum(){ return m_PassNum; };

	/// <summary>
	/// �����_�����O�p�X�ԍ��ݒ�
	/// </summary>
	inline void       SetRenderingPass( u32 passNo ){ m_RenderingPassNo = passNo; }

	/// <summary>
	/// ���_���W�ϊ��s���ݒ肷��i�ǂ̃V�F�[�_�[�ɂ����_���W�ϊ��͕K����������j
	/// </summary>
	inline void       SetWorldMatrix ( const math::CMatrix4x3 &w )  { m_WorldMatrix = w; };

	/// <summary>
	/// �J��������ݒ�
	/// </summary>
	inline void       SetCamera      ( const CameraPtr &spCamera ) { m_spCamera = spCamera;}

	/// <summary>
	/// ���b�V����ݒ�
	/// </summary>
	inline void       SetExtraData   ( const CExtraDataPtr   &spExtra )   { m_spExtraData = spExtra; }

	/// <summary>
	/// ���C�g��ݒ�
	/// </summary>
	inline void       SetLight   ( const CLightPtr   &spLight )   { m_spLight = spLight; }

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	virtual void vRendering	 ( CRenderingParam &param );

	/// <summary>
	/// �`��
	/// </summary>
	virtual void vDraw() = 0;

protected:

	/// <summary>
	/// �p�X��
	/// </summary>
	u32                       m_PassNum;

	/// <summary>
	/// �p�X��
	/// </summary>
	u32                       m_RenderingPassNo;

	/// <summary>
	/// ���[���h�}�g���b�N�X
	/// </summary>
	math::CMatrix4x3		  m_WorldMatrix;

	/// <summary>
	/// �J�������
	/// </summary>
    CameraPtr                 m_spCamera;

	/// <summary>
	/// �G�L�X�g���f�[�^���
	/// </summary>
    CExtraDataPtr             m_spExtraData;

	/// <summary>
	/// ���C�g���
	/// </summary>
    CLightPtr                 m_spLight;

    /// <summary>
	///  ���ݎg�p���Ă���V�F�[�_�[�������h�c
    /// </summary>
	u32                       m_ShaderId;

    /// <summary>
	/// �V�F�[�_�[���
    /// </summary>
	CShaderInfoPtr            m_spShaderInfo;
};

} } }
