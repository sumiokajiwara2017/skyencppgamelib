#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

SkyImplementRTTI( sky::lib::graphic::CShaderObject       , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

void CShaderObject::Initialize()
{

#if  defined( SKYLIB_RENDER_OPENGL4 ) && defined( SW_SKYGRAPHICLIB_USE_CG_ON )
	CCg::Initialize();
#endif

}

void CShaderObject::Dispose()
{
#if  defined( SKYLIB_RENDER_OPENGL4 ) && defined( SW_SKYGRAPHICLIB_USE_CG_ON )
	CCg::Dispose();
#endif

}

void CShaderObject::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "It failed in making the device context. \n" ) );

	//�������X�^�b�N�ɐς�
	param.ShaderStack.Push( thisPtr( CShaderObjectPtr ) ); //���Ƃ��Ƃ���this�����̃X�}�[�g�|�C���^�ɊǗ�����Ă����ꍇ�iRefCount��0�ȏ�̏ꍇ)���̎󂯓n�������Ă���肪�������Ȃ��B
}

void CShaderObject::vPostRendering( CRenderingParam &param )
{
	//�X�^�b�N������
	param.ShaderStack.Pop();
}

CShaderObject::CShaderObject() : m_PassNum( 0 )
{
	
}

} } }
