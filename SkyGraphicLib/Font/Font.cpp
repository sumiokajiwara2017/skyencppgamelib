#include "StdAfx.h"
#include "SkyGraphicLib/Font.h"
#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CFont       , sky::lib::graphic::CSceneGraphNode );
SkyImplementRTTI( sky::lib::graphic::CFontSimple , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

static vrtxtype s_FontIndexBufferTbl[] = { 0 , 1 , 2 , 2 , 1 , 3 };
static vrtxtype s_FontIndexBufferTblNum = array_sizeof( s_FontIndexBufferTbl );

CFont::CFont( font::eFontStyleType eStyle , dectype prio  ) : m_printFontNum( 0 )
{ 
	m_SentenceData.Color         = CColor_White;
	m_SentenceData.Sc            = 1.0f;
	m_SentenceData.Sentence[ 0 ] = NULL;
	m_SentenceData.Pos           = math::CBasicVector2( 0.0f , 0.0f );
	m_SentenceData.OutIntvl      = 0;
    m_Prio = CONVERT_PRIO_TO_Z( prio );

	//�����X�^�C��
	m_Style = eStyle;

	//���ƍ���
    m_SentenceSize.W = 0;
    m_SentenceSize.H = 0;
	m_FontSize.W = 0;
	m_FontSize.H = 0;

	m_IsReWrite = skyTrue;
}

void CFont::SetUp()
{
	if ( m_IsSetUp == skyFalse )
	{
		//�V�F�[�_�[����
		AttachBranch( CTextureMappingShaderObject::Create( CHS( _T( "font_default_shader" ) ) ) );

		m_IsSetUp = skyTrue;
	}
}

CTextureMappingShaderObjectPtr CFont::GetShader( skyBool isRequired )
{
	CTextureMappingShaderObjectPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CTextureMappingShaderObject , CTextureMappingShaderObjectPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "TextureMappingShaderObject is not found." ) );
	}
#endif
	return spResult;
}

void CFont::PrintfSentence( dectype posX , dectype posY , const CColor &color , dectype sc , u32 outIntvl , const skyString *format , ... )
{
	SKY_ASSERT_MSG( m_IsSetUp == skyTrue ,  _T( "It has not setup yet." ) );

	m_SentenceData.Pos.X = posX;
	m_SentenceData.Pos.Y = posY;
	m_SentenceData.Color = color;
	m_SentenceData.Sc    = sc;
	m_SentenceData.OutIntvl = outIntvl;

	VA_LIST( m_SentenceData.Sentence , font::ONE_LINE_FONT_NUM_MAX , format );

	//���������J�E���g�i���ׂ������j
	m_printFontNum = ( vrtxtype )CStringUtil_::Len( m_SentenceData.Sentence );

	//���ƍ������v�Z�i���ׂ������j
	dectype wX = 0 , wY = 0 , minWidth = 0 , maxWidth = 0 , minHeight = 0 , maxHeight = 0;

	//�v�Z�p���[�N
	math::CBasicVector2 vec2( 0 , 0 );

	vrtxtype i = 0;
	dectype maxHeightNum = 0; //�o�͕����̍ő�l
	dectype space = 0;
	u32 ecNum = 1; //���s��
	while( m_SentenceData.Sentence[ i ] != EOT )
	{
		//���s�R�[�h�̏ꍇ���W�����ω������Ď���
		if ( m_SentenceData.Sentence[ i ] == EC )
		{
			//���W�͒��S���W�ɂ��Ċi�[
			vec2.Y = vec2.Y + maxHeightNum * -1;
			vec2.X = m_SentenceData.Pos.X;
			ecNum++;
		}
		else if ( m_SentenceData.Sentence[ i ] == _T( ' ' ) )
		{
			vec2.X += space;
		}
		else 
		{
			//�����R�[�h�ɑΉ������`������擾����
			COneFontData &data = CFontManager_::GetFont( m_SentenceData.Sentence[ i ] , m_Style );

			//���_���̎擾
			for ( vrtxtype j = 0 ; j < 4 ; j++ )
			{
				//���W��ς���i ( ���Ƃ��Ƃ̋�`���W * �g��k�� ) �{ ���[���h���W �j
				wX = ( f32 )( vec2.X + ( data.pos[ j ].X * m_SentenceData.Sc ) );
				wY = ( f32 )( vec2.Y + ( data.pos[ j ].Y * m_SentenceData.Sc ) );

				if( minWidth > wX )
				{
					minWidth = wX;
				} 
				else if ( maxWidth < wX )
				{
					maxWidth = wX;
				}
				else if ( minHeight > wY )
				{
					minHeight = wY;
				}
				else if ( maxHeight < wY )
				{
					maxHeight = wY;
				}
			}

			//�`��ʒu�����炷�B
			space = ( dectype )( m_SentenceData.Sc * data.size.W ); //�X�y�[�X�Ɏg��
			vec2.X += space;
			dectype height = ( dectype )( m_SentenceData.Sc * data.size.H );
			if ( maxHeightNum < height ) maxHeightNum = height;
		}
		i++;
	}

	//������̒���
    m_SentenceSize.W = maxWidth - minWidth;
    m_SentenceSize.H = maxHeight - minHeight;

	//1�����̏c����
	m_FontSize.W = m_SentenceSize.W / i - 1;
	m_FontSize.H = m_SentenceSize.H / ecNum;

	m_IsReWrite = skyTrue;
}

void CFont::vRendering( CRenderingParam &param )
{
	if ( m_SentenceData.Sentence[ 0 ] == NULL ) return;

	if ( m_IsReWrite == skyTrue )
	{//�����̓��e���ω������烁�b�V�������蒼���B���t���[�����蒼���̂͌����I�ł͂Ȃ��B

		//�v�Z�p���[�N
		math::CBasicVector2 vec2;

		//�ő啶���o�͐��~�S�̒��_���Œ��_�o�b�t�@������������B
		//�P�����S���_�B
		//�P�����U�C���f�b�N�X
		//���񓯕`��̂��ߖ��t���[���m�ۂ��Ă���B���_�o�b�t�@�̓��e�����t���[���ω�������̂͒��_�o�b�t�@�̊m�ۂ𖈃t���[���s���K�v������j
		//�񓯊�
		m_MeshContena = CMeshContenaCreate_();
		m_MeshContena->AttachMesh( CMeshCreate_() );
		m_MeshContena->GetMesh()->AttachMaterialContena( CMaterialContenaCreate_() );
		m_spMaterial = CMaterialCreate_();
		m_spMaterial->AttachDiffuseMap( CFontManager_::GetFontStyleTex( m_Style ) );

		CIndexBufferPtr spIndexBuffer = CIndexBufferCreate_();
		CVertexBufferPtr spVertexBuffer = CVertexUtil::CreateVertexBuffer( m_printFontNum * 4 , m_printFontNum * 6 , vertex::ePrimitiveType_Trianglelist , spIndexBuffer );
		vrtxtype spriteIndexNum = 6;
		m_MeshContena->GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , spriteIndexNum );
		spVertexBuffer->CreateColorStream( spVertexBuffer->GetVertexNum() );
		spVertexBuffer->CreateUvStream( spVertexBuffer->GetVertexNum() );

		vrtxtype vertexCoursor = 0;
		vrtxtype indexCoursor = 0;

		vec2.X = 0;
		vec2.Y = 0;

		//�������㌴�_�������ꍇ�́A�n���ꂽ���W�����㌴�_���W���Ɖ��肵��Y�ړ������𔽓]����
		CameraPtr spCamera = param.CameraStack.GetTop();
		if ( spCamera->IsLeftTopCenter() )
		{
			vec2.Y *= -1;
		}

		u32 i = 0;
		dectype maxHeight = 0; //�o�͕����̍ő�l
		dectype space = 0;

		while( m_SentenceData.Sentence[ i ] != EOT )
		{
			//���s�R�[�h�̏ꍇ���W�����ω������Ď���
			if ( m_SentenceData.Sentence[ i ] == EC )
			{
				//���W�͒��S���W�ɂ��Ċi�[
				vec2.Y = vec2.Y + maxHeight * -1;
				vec2.X = m_SentenceData.Pos.X;
			}
			else if ( m_SentenceData.Sentence[ i ] == _T( ' ' ) )
			{
				vec2.X += space;
			}
			else 
			{
				//�����R�[�h�ɑΉ������`������擾����
				COneFontData &data = CFontManager_::GetFont( m_SentenceData.Sentence[ i ] , m_Style );

				//���_���̎擾
				for ( vrtxtype j = 0 ; j < 4 ; j++ )
				{
					//UV��ݒ肷��
					spVertexBuffer->GetUvStream()->GetUv( j + vertexCoursor )->tu = ( dectype )data.uv[ j ].X;
					spVertexBuffer->GetUvStream()->GetUv( j + vertexCoursor )->tv = ( dectype )data.uv[ j ].Y;

					//�F��ς���
					*spVertexBuffer->GetColorStream()->GetColor( j + vertexCoursor ) = m_SentenceData.Color;

					//���W��ς���i ( ���Ƃ��Ƃ̋�`���W * �g��k�� ) �{ ���[���h���W �j
					spVertexBuffer->GetPositionStream()->GetPosition( j + vertexCoursor )->x = ( f32 )( vec2.X + ( data.pos[ j ].X * m_SentenceData.Sc ) );
					spVertexBuffer->GetPositionStream()->GetPosition( j + vertexCoursor )->y = ( f32 )( vec2.Y + ( data.pos[ j ].Y * m_SentenceData.Sc ) );
					spVertexBuffer->GetPositionStream()->GetPosition( j + vertexCoursor )->z = ( f32 )( 0 ); //�O��֌W�͕��s�ړ��Œ�������
				}

				//�C���f�b�N�X���̎擾
				for ( vrtxtype j = 0 ; j < s_FontIndexBufferTblNum; j++ )
				{
					spIndexBuffer->m_pCpuMemoryIndexBuffer[ j + indexCoursor ] = s_FontIndexBufferTbl[ j ] + vertexCoursor;
				}

				//�J�[�\���𓮂���
				vertexCoursor += 4;
				indexCoursor += s_FontIndexBufferTblNum;

				//�`��ʒu�����炷�B
				space = ( dectype )( m_SentenceData.Sc * data.size.W ); //�X�y�[�X�Ɏg��
				vec2.X += space;
				dectype height = ( dectype )( m_SentenceData.Sc * data.size.H );
				if ( maxHeight < height ) maxHeight = height;
			}
			i++;
		}
		//�g�p���_���A�C���f�b�N�X���̍X�V�i����͂��Ȃ����ł����ʂ���Ȃ��Ƃ͂��܂���j
		spVertexBuffer->UpdateUseVertexNum( vertexCoursor );
		spIndexBuffer->UpdateUseIndexNum( indexCoursor );

		//�O���[�v�̃C���f�b�N�X����ݒ肷��
		m_MeshContena->GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , indexCoursor );

		//���b�V���ɒ��_�o�b�t�@��ݒ�
		m_MeshContena->GetMesh()->AttachVertexBuffer( spVertexBuffer );
		m_MeshContena->GetMesh()->AttachIndexBuffer( spIndexBuffer );

		//GPU�������ɏ�������
		CRenderCommand::CreateMeshGpuMemory( m_MeshContena );
		CRenderCommand::WirteMeshGpuMemory( m_MeshContena );

		//CPU���̃������͏���
		CRenderCommand::DeleteMeshCpuMemory( m_MeshContena );

		m_IsReWrite = skyFalse;
	}

	//���W�̐ݒ�(�e�̍��W�n�Ɉړ�����j
	//�e�̍��W�n�̂Q�c�ړ����݂̂𔽉f����B
	math::CMatrix4x3 transMat;
	transMat.Identity();
	transMat.SetTranslation( math::CBasicVector3( m_SentenceData.Pos.X + param.MatrixStack.GetTop().Tx , -m_SentenceData.Pos.Y + param.MatrixStack.GetTop().Ty , CONVERT_PRIO_TO_Z( m_Prio ) ) );

	//��Δ������Ȃ̂ł��Ƃ��炢�����Ƀ����_�����O����
	CShaderObjectPtr shader = GetShader();
	CExtraDataPtr spExtraData;
	if ( param.ExtraDataStack.IsEmpty() == skyFalse )
	{
		spExtraData = param.ExtraDataStack.GetTop();
	}

	for ( u32 i = 0 ; i < m_MeshContena->GetMeshNum() ; i++ )
	{
		param.AlphaDataList.AddTail( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingMesh , 
			CRenderCommand::RenderingMeshParam( 0 , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , shader , m_MeshContena->GetMesh( i ) , transMat , spExtraData , param.LightStack.GetTop() ) , IS_COMMAND ) ) );
	}
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_IsSetUp , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Color.r , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Color.g , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Color.b , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Color.a , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Sc , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&sentencesize , sizeof( u32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )sentence , sentencesize , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.OutIntvl , sizeof( u32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Pos.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Pos.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Style , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_printFontNum , sizeof( u16 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceSize.W , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceSize.H , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_FontSize.W , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_FontSize.H , sizeof( d64 ) , isWrite ); \

skyBool CFont::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "font bin save. \n" ) );

	string::CString  sentencestr  =  ( m_SentenceData.Sentence == skyNull )? _T( "" ) : m_SentenceData.Sentence;
	u32              sentencesize = sentencestr.GetSize();
	const skyString* sentence     = sentencestr.Get();

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CFont::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "font bin load. \n" ) );

	u32 sentencesize;
	skyString sentence[ PATH_LEN_MAX ] = _T( "none" );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	//���O�擾
	CStringUtil_::Copy( m_SentenceData.Sentence , font::ONE_LINE_FONT_NUM_MAX , sentence );

	return result;
}
const serialize::CSerializeInterfacePtr& CFont::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CFontPtr )spContainer )->m_IsSetUp = m_Style;
	( ( CFontPtr )spContainer )->m_SentenceData = m_SentenceData;
	( ( CFontPtr )spContainer )->m_Style = m_Style;
	( ( CFontPtr )spContainer )->m_printFontNum = m_printFontNum;
	( ( CFontPtr )spContainer )->m_SentenceSize = m_SentenceSize;
	( ( CFontPtr )spContainer )->m_FontSize = m_FontSize;
	return spContainer;
}
const serialize::CSerializeInterfacePtr& CFont::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CFont::Clone()
{
	return Copy( Create( hash::CHashStringPtr() , font::eFontStyleType_Gosic13 , RENDERING_2D_PRIO_FAST ) );
}
serialize::CSerializeInterfacePtr CFont::CloneSharing()
{
	return Clone();
}

void CFont::Activate()
{
	ACTIVATE_COMMON();

	//�������o�͂���̂ŕ����Ǘ���������
	CFontManagerInitialize_();
}

#ifdef SW_SKYLIB_DEBUG_ON

void CFont::NodeTreePrint( u32 indent , eRelationKind eKind )
{
	SKY_UNUSED_ARG( eKind );

	SKY_PRINT_INDENT( indent * 2 );
	SKY_PRINTF( "%s " , GetRTTI()->GetName() );
	SKY_PRINTF( _T( "%s pos[ %4.3f , %4.3f ] prio[ %4.3f ] offsetPrio[ %4.3f ] \n" ) , m_SentenceData.Sentence , m_SentenceData.Pos.X , m_SentenceData.Pos.Y , m_Prio );
}

#endif

//----------------------------------------------------------------------------

CFontSimple::CFontSimple( font::eFontStyleType eStyle , u32 lineNum , dectype prio ) : 
m_SentencePool( lineNum ) ,
m_IsStanderdPositionDraw( skyFalse ) ,
m_IsStanderdPosition( 0 , 0 )
{
	//�����X�^�C��
	m_Style = eStyle;

	//�D�揇�ʂ̐ݒ�
	m_Prio = CONVERT_PRIO_TO_Z( prio );
}

void CFontSimple::SetUp()
{
	if ( m_IsSetUp == skyFalse )
	{
		//�V�F�[�_�[����
		AttachBranch( CTextureMappingShaderObject::Create( CHS( _T( "font_default_shader" ) ) ) );

		//���_�o�b�t�@�̍쐬
		vrtxtype printFontNum = ( vrtxtype )( font::ONE_LINE_NUM_MAX * font::ONE_LINE_FONT_NUM_MAX );
		CIndexBufferPtr  spIndexBuffer = CIndexBufferCreate_();
		CVertexBufferPtr spVertexBuffer = CVertexUtil::CreateVertexBuffer( printFontNum * 4 , printFontNum * 6 , vertex::ePrimitiveType_Trianglelist , spIndexBuffer );
		spVertexBuffer->CreateColorStream( spVertexBuffer->GetVertexNum() );
		spVertexBuffer->CreateUvStream( spVertexBuffer->GetVertexNum() ); 
		AttachBranch( spVertexBuffer );

		//�}�e���A���̍쐬
		CMeshPtr spMesh = CMeshCreateName_( CHS( _T( "font_default_material" ) ) );
		spMesh->AttachMaterialContena( CMaterialContenaCreate_() );
		CMaterialPtr  spMaterial = CMaterialCreateName_( CHS( _T( "font_default_material" ) ) );
		spMaterial->AttachDiffuseMap( CFontManager_::GetFontStyleTex( m_Style ) );
		spMesh->GetMaterialContena()->AddMaterial( spMaterial , 0 , 0 );
		spMesh->AttachVertexBuffer( spVertexBuffer );
		spMesh->AttachIndexBuffer( spIndexBuffer );
		AttachBranch( spMesh );

		//GPU���������쐬����
		spMesh->CreateGpuMemory();

		m_IsSetUp = skyTrue;
	}
}

void CFontSimple::PrintfSentence( dectype posX , dectype posY , const CColor &color , dectype sc , const skyString *format , ... )
{
	SKY_ASSERT_MSG( m_IsSetUp == skyTrue ,  _T( "It has not setup yet." ) );

	CSentenceData_* pItem = m_SentencePool.Create();
	VA_LIST( pItem->Sentence , font::ONE_LINE_FONT_NUM_MAX , format );
	pItem->Color = color;
	pItem->Sc = sc;
	pItem->Pos.X = posX;
	pItem->Pos.Y = posY;

	m_SentenceList.AddTail( pItem );
}

void CFontSimple::vRendering( CRenderingParam &param )
{
	if ( m_SentenceList.GetNum() == 0 ) return;
	if ( m_SentenceList.GetNum() > font::ONE_LINE_NUM_MAX )
	{
		//���X�g���폜
		m_SentenceList.DeleteAll();
		return; //���E���C�����𒴂�����I���
	}
	if ( IS_COMMAND == skyTrue ) return; //�񓯊����s�ɂ͑Ή����Ă܂���B

	//�v�Z�p���[�N
	math::CBasicVector2 vec2;

	//�ő啶���o�͐��~�S�̒��_���Œ��_�o�b�t�@������������B
	//�P�����S���_�B
	//�P�����U�C���f�b�N�X
	//���񓯕`��̂��ߖ��t���[���m�ۂ��Ă���B���_�o�b�t�@�̓��e�����t���[���ω�������̂͒��_�o�b�t�@�̊m�ۂ𖈃t���[���s���K�v������j
	//�񓯊�
	CVertexBufferPtr spVertexBuffer = GetVertexBuffer();
	CIndexBufferPtr spIndexBuffer = GetMesh()->GetIndexBuffer();
	CMeshContenaPtr meshContena = CMeshContenaCreate_();
	vrtxtype spriteIndexNum = 6;
	meshContena->AttachMesh( GetMesh() );
	GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , spriteIndexNum );

	vrtxtype vertexCoursor = 0;
	vrtxtype indexCoursor = 0;
	dectype maxHeight = 0; //�o�͕����̍ő�l
	math::CBasicVector2 offsetPos( m_IsStanderdPosition.X , m_IsStanderdPosition.Y );
	FOREACH( CSentenceDataPtrList_ , it , m_SentenceList )
	{
		//�P�����������_�����O���Ă���
		CSentenceData_ *snt = *it;

		if( m_IsStanderdPositionDraw == skyTrue )
		{
			offsetPos.Y +=  maxHeight;
		}

		//���W�̐ݒ�(�e�̍��W�n�Ɉړ�����j
		vec2.X = snt->Pos.X + offsetPos.X;
		vec2.Y = snt->Pos.Y + offsetPos.Y;

		//�������㌴�_�������ꍇ�́A�n���ꂽ���W�����㌴�_���W���Ɖ��肵��Y�ړ������𔽓]����
		CameraPtr spCamera = param.CameraStack.GetTop();
		if ( spCamera->IsLeftTopCenter() )
		{
			vec2.Y *= -1;
		}
		vertex::CVertexFmt_UV *pToUv = spVertexBuffer->GetUvStream()->GetUv( 0 );
		vertex::CVertexFmt_Color *pToColor = spVertexBuffer->GetColorStream()->GetColor( 0 );
		vertex::CVertexFmt_Pos *pToPos = spVertexBuffer->GetPositionStream()->GetPosition( 0 );

		u32 i = 0;
		maxHeight = 0; //�o�͕����̍ő�l
		dectype space = 0;
		while( snt->Sentence[ i ] != EOT )
		{
			//���s�R�[�h�̏ꍇ���W�����ω������Ď���
			if ( snt->Sentence[ i ] == EC )
			{
				//���W�͒��S���W�ɂ��Ċi�[
				vec2.Y = vec2.Y + maxHeight * -1;
				vec2.X = snt->Pos.X;
			}
			else if ( snt->Sentence[ i ] == _T( ' ' ) )
			{
				vec2.X += space;
			}
			else 
			{
				//�����R�[�h�ɑΉ������`������擾����
				COneFontData &data = CFontManager_::GetFont( snt->Sentence[ i ] , m_Style );

				//���_���̎擾
				for ( vrtxtype j = 0 ; j < 4 ; j++ )
				{
					//UV��ݒ肷��
					pToUv[ j + vertexCoursor ].tu = ( dectype )data.uv[ j ].X;
					pToUv[ j + vertexCoursor ].tv = ( dectype )data.uv[ j ].Y;

#ifdef SKYLIB_RENDER_OPENGL4 //OpenGL�̂Ƃ��͏���UV��␳����
					pToUv[ j + vertexCoursor ].tv += 0.002f;
#endif
					//�F��ς���
					pToColor[ j + vertexCoursor ] = snt->Color;

					//���W��ς���i ( ���Ƃ��Ƃ̋�`���W * �g��k�� ) �{ ���[���h���W �j
					pToPos[ j + vertexCoursor ].x = ( f32 )( vec2.X + ( data.pos[ j ].X * snt->Sc ) );
					pToPos[ j + vertexCoursor ].y = ( f32 )( vec2.Y + ( data.pos[ j ].Y * snt->Sc ) );
					pToPos[ j + vertexCoursor ].z = ( f32 )( CONVERT_PRIO_TO_Z( m_Prio ) );
				}

				//�C���f�b�N�X���̎擾
				for ( vrtxtype j = 0 ; j < s_FontIndexBufferTblNum; j++ )
				{
					spIndexBuffer->m_pCpuMemoryIndexBuffer[ j + indexCoursor ] = s_FontIndexBufferTbl[ j ] + vertexCoursor;
				}

				//�J�[�\���𓮂���
				vertexCoursor += 4;
				indexCoursor += s_FontIndexBufferTblNum;

				//�`��ʒu�����炷�B
				space = ( dectype )( snt->Sc * data.size.W ); //�X�y�[�X�Ɏg��
				vec2.X += space;
				dectype height = ( dectype )( snt->Sc * data.size.H );
				if ( maxHeight < height ) maxHeight = height;
			}
			i++;
		}

		//�`��f�[�^�ɏW�߂�������Ă��܂��B
		m_SentencePool.Delete( *it );
	}

	//�g�p���_���A�C���f�b�N�X���̍X�V�i����͂��Ȃ����ł����ʂ���Ȃ��Ƃ͂��܂���j
	spVertexBuffer->UpdateUseVertexNum( vertexCoursor );
	spIndexBuffer->UpdateUseIndexNum( indexCoursor );

	//�O���[�v�̃C���f�b�N�X����ݒ肷��
	GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , indexCoursor );

	//���b�V���ɒ��_�o�b�t�@��ݒ�
	meshContena->GetMesh()->AttachVertexBuffer( spVertexBuffer );

	//GPU�������ɏ�������
	CRenderCommand::WirteMeshGpuMemory( meshContena );

	//��Δ������Ȃ̂ł��Ƃ��炢�����Ƀ����_�����O����
	CShaderObjectPtr shader = GetShader();
	CExtraDataPtr spExtraData;
	if ( param.ExtraDataStack.IsEmpty() == skyFalse )
	{
		spExtraData = param.ExtraDataStack.GetTop();
	}

	//�e�s�񂩂�2D���s�ړ���񂾂������p�����s����쐬����B
	math::CMatrix4x3 mat;
	mat.Identity();
	math::CMatrix4x3 &parentmat = param.MatrixStack.GetTop();
	mat.Tx = parentmat.Tx;
	mat.Ty = parentmat.Ty;
	mat.Tz = CONVERT_PRIO_TO_Z( m_Prio );

	for ( u32 i = 0 ; i < meshContena->GetMeshNum() ; i++ )
	{
		//�������Ƃ��ĕ`��f�[�^�L���[�Ɋi�[����
		param.AlphaDataList.AddTail( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingMesh , 
			CRenderCommand::RenderingMeshParam( 0 , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , shader , meshContena->GetMesh( i ) , mat , spExtraData , param.LightStack.GetTop() ) , IS_COMMAND ) ) );
	}

	//���X�g���폜
	m_SentenceList.DeleteAll();

	//�`��I��
	m_RenderingEnd();
}

#define GETSET_BIN_2( isGet ) \
	stream->vWriteReadStream( ( u8 * )&m_IsSetUp , sizeof( s32 ) , isGet ); \
	stream->vWriteReadStream( ( u8 * )&m_Style , sizeof( s32 ) , isGet ); \
	stream->vWriteReadStream( ( u8 * )&m_IsStanderdPositionDraw , sizeof( s32 ) , isGet ); \
	stream->vWriteReadStream( ( u8 * )&m_IsStanderdPosition.X , sizeof( d64 ) , isGet ); \
	stream->vWriteReadStream( ( u8 * )&m_IsStanderdPosition.Y , sizeof( d64 ) , isGet ); \
	
skyBool CFontSimple::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "fontsimple bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN_2( skyTrue );

	return result;
}

skyBool CFontSimple::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "fontsimple bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN_2( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CFontSimple::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CFontSimplePtr )spContainer )->m_IsSetUp = m_IsSetUp;
	( ( CFontSimplePtr )spContainer )->m_Style = m_Style;
	( ( CFontSimplePtr )spContainer )->m_IsStanderdPositionDraw = m_IsStanderdPositionDraw;
	( ( CFontSimplePtr )spContainer )->m_IsStanderdPosition = m_IsStanderdPosition;
	return spContainer;
}

serialize::CSerializeInterfacePtr CFontSimple::Clone()
{
	return Copy( Create( hash::CHashStringPtr() ) );
}

void CFontSimple::Activate()
{
	ACTIVATE_COMMON();

	//�������o�͂���̂ŕ����Ǘ���������
	CFontManagerInitialize_();
}

CTextureMappingShaderObjectPtr CFontSimple::GetShader( skyBool isRequired )
{
	CTextureMappingShaderObjectPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CTextureMappingShaderObject , CTextureMappingShaderObjectPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "TextureMappingShaderObject is not found." ) );
	}
#endif
	return spResult;
}

CMeshPtr CFontSimple::GetMesh( skyBool isRequired )
{
	CMeshPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CMesh , CMeshPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "Mesh is not found." ) );
	}
#endif
	return spResult;
}

CVertexBufferPtr CFontSimple::GetVertexBuffer( skyBool isRequired )
{
	CVertexBufferPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CVertexBuffer , CVertexBufferPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "Material is not found." ) );
	}
#endif
	return spResult;
}

} } }