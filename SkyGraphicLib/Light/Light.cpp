#include "StdAfx.h"
#include "SkyGraphicLib/Light.h"

SkyImplementRTTI( sky::lib::graphic::CLight , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

void CLight::vRendering( CRenderingParam &param )
{
    //�������X�^�b�N�ɐς�
    param.LightStack.Push( thisPtr( CLightPtr ) ); //���Ƃ��Ƃ���this�����̃X�}�[�g�|�C���^�ɊǗ�����Ă����ꍇ�iRefCount��0�ȏ�̏ꍇ)���̎󂯓n�������Ă���肪�������Ȃ��B
}

void CLight::vPostRendering( CRenderingParam &param )
{
    //�������X�^�b�N�������
    param.LightStack.Pop();
}

} } }