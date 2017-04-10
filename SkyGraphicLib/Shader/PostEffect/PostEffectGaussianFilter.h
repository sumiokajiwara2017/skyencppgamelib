#pragma once
/*

�y �T�v �z
��ʑS�̂��ڂ����܂��B

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL ��������
OpenGL    PC Cg   ��������

*/

namespace sky { namespace lib { namespace graphic {

class   CPostEffectGaussianFilter;
typedef SmartPointer< CPostEffectGaussianFilter >		CPostEffectGaussianFilterPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
///  spRenderTargetTexture�̓��e���ڂ����ă����_�����O����
///  CSamplerData::m_Sampler0(�g�p���Ȃ��j
///  CSamplerData::m_Sampler1(�g�p���Ȃ��j
///  CSamplerData::m_Sampler2(�g�p���Ȃ��j
/// </summary>
class CPostEffectGaussianFilter : public CPostEffectObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CPostEffectGaussianFilter(){};

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void vRendering( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix );

	/// <summary>
	/// �ڂ����x
	/// </summary>
	void SetDispersion( f32 value = 5.0f );

//---static

	/// <summary>
	/// �쐬
	/// </summary>
	static CPostEffectGaussianFilterPtr   Create()
	{
		return CPostEffectGaussianFilterPtr( NEW__( CPostEffectGaussianFilter , CPostEffectGaussianFilter() ) );
	}

private:

	/// <summary>
	/// �E�F�C�g
	/// </summary>
	f32 m_Weights[ 8 ];

	/// <summary>
	/// �ڂ����x
	/// </summary>
	f32 m_Dispersion_sq;

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix );

	/// <summary>
	/// �p�X�Q�`��i�����ƂɎ����j
	/// </summary>
	void Pass2Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix );

	/// <summary>
	/// �E�F�C�g���v�Z
	/// </summary>
	void CPostEffectGaussianFilter::ComputeGaussWeights( f32 dispersion );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CPostEffectGaussianFilter();
};

} } }

#define CPostEffectGaussianFilterCreate_()	          sky::lib::graphic::CPostEffectGaussianFilter::Create()
