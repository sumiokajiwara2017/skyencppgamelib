#pragma once
/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL ��������
OpenGL    PC Cg   ��������

*/

namespace sky { namespace lib { namespace graphic {

class   CPostEffectTextureMapping;
typedef SmartPointer< CPostEffectTextureMapping >		CPostEffectTextureMappingPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
///  spRenderTargetTexture�̓��e�������_�����O����
///  CSamplerData::m_Sampler0(�g�p���Ȃ��j
///  CSamplerData::m_Sampler1(�g�p���Ȃ��j
///  CSamplerData::m_Sampler2(�g�p���Ȃ��j
/// </summary>
class CPostEffectTextureMapping : public CPostEffectObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CPostEffectTextureMapping(){};

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void vRendering( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix );

//---static

	/// <summary>
	/// �쐬
	/// </summary>
	static CPostEffectTextureMappingPtr   Create()
	{
		return CPostEffectTextureMappingPtr( NEW__( CPostEffectTextureMapping , CPostEffectTextureMapping() ) );
	}
};

} } }

#define CPostEffectTextureMappingCreate_()	          sky::lib::graphic::CPostEffectTextureMapping::Create()
