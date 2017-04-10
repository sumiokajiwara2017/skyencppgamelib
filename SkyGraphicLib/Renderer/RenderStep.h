#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �����_�����O�X�e�b�v�N���X
/// ���̃N���X���p������vRendering����������Ε`����J�X�^�}�C�Y�ł���B
/// </summary>
class CRenderStep : public base::SkyRefObject
{
	friend class CRenderStepManager;
public:

	/// <summary>
	/// ���O
	/// </summary>
    CName   Name;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CRenderStep(){}

    /// <summary>
	///  �����_�����O���s
    /// </summary>
	virtual void Rendering( const fps::CFpsController &fps );

	/// <summary>
	/// �E�B���h�E�̎Q��
	/// </summary>
	CWindowPtr             m_spWindow;

	/// <summary>
	/// �����_�����O�^�[�Q�b�g
	/// </summary>
	CRenderTargetPtr       m_spRenderTarget;

	/// <summary>
	/// �[�x�X�e���V���o�b�t�@
	/// </summary>
	CDepthStencilBufferPtr m_spDepthStencilBuffer;

	/// <summary>
	/// �����_�[�X�e�[�g
	/// </summary>
	CRenderStatePtr        m_spRenderState;

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CRenderStepPtr Create( const hash::CHashStringPtr &name )
	{
		
		CRenderStepPtr spStep( NEW__( CRenderStep , CRenderStep() ) );
		if ( name.IsEmpty() == skyFalse ) spStep->Name.SetName( name );
		return spStep;
	}

private:

	/// <summary>
	/// �Ǘ�
	/// </summary>
	CRenderStepManagerPtr m_RenderStepManager;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRenderStep(){}
};

#define CRenderStepCreate_( name )	sky::lib::graphic::CRenderStep::Create( name );

} } }