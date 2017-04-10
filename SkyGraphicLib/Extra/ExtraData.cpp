#include "StdAfx.h"
#include "SkyGraphicLib/Extra.h"

SkyImplementRTTI( sky::lib::graphic::CExtraData , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< ���� >---------------------------------------------//

void CExtraData::vRendering( CRenderingParam &param )
{
	//�������X�^�b�N�ɐς�
	param.ExtraDataStack.Push( thisPtr( CExtraDataPtr ) ); //���Ƃ��Ƃ���this�����̃X�}�[�g�|�C���^�ɊǗ�����Ă����ꍇ�iRefCount��0�ȏ�̏ꍇ)���̎󂯓n�������Ă���肪�������Ȃ��B
}

void CExtraData::vPostRendering  ( CRenderingParam &param )
{
	//�X�^�b�N������
	param.ExtraDataStack.Pop();
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_DoubleValue , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IntValue , sizeof( s32 ) , isWrite ); \

skyBool CExtraData::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "extradata bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CExtraData::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "extradata bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CExtraData::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CExtraDataPtr )spContainer )->m_DoubleValue = m_DoubleValue;
	( ( CExtraDataPtr )spContainer )->m_IntValue = m_IntValue;
	return spContainer;
}
const serialize::CSerializeInterfacePtr& CExtraData::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CExtraData::Clone()
{
	return Copy( CExtraDataPtr( NEW__( CExtraData , CExtraData() ) ) );
}
serialize::CSerializeInterfacePtr CExtraData::CloneSharing()
{
	return Clone();
}

} } }