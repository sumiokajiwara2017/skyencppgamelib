#pragma once

namespace sky { namespace lib { namespace cons {

/// <summary>
/// �v���O�����萔�̑���
/// �v���O�����̒萔��Excel�ŋL�q���A�o�C�i�������Ă��̃v���O����
/// �ǂݍ��݂����Ă��܂��B\lib\skycpplib\media\const�ȉ���Excel�����݂��܂��B
/// Excel���g�p�����Windows�ˑ��Ȃ̂ŁA������ʂ̃t�@�C���`���ɂ��邩������܂���B
/// </summary>
namespace PgConst
{
	extern u8 *g_pPgConstDataAdr;

	/// <summary>
	/// �v���O�����萔�̓Ǎ�
	/// </summary>
	void Load			();

	/// <summary>
	/// �v���O�����萔�̔j��
	/// </summary>
	void Destroy		();

	/// <summary>
	/// �v���O�����萔�̍ēǍ�
	/// </summary>
	void ReLoad			();

	/// <summary>
	/// �v���O�����萔�̎擾
	/// </summary>
	inline u8 *Get_u8	( pgconstdata id )
	{
		return g_pPgConstDataAdr + id;
	}
	inline s8 Get_s8	( pgconstdata id )
	{
		return *( s8 * )( g_pPgConstDataAdr + id );
	}
	inline u16 Get_u16	( pgconstdata id )
	{
		return *( u16 * )( g_pPgConstDataAdr + id );
	}
	inline s16 Get_s16	( pgconstdata id )
	{
		return *( s16 * )( g_pPgConstDataAdr + id );
	}
	inline u32 Get_u32	( pgconstdata id )
	{
		return *( u32 * )( g_pPgConstDataAdr + id );
	}
	inline s32 Get_s32	( pgconstdata id )
	{
		return *( s32 * )( g_pPgConstDataAdr + id );
	}
	inline const char *Get_Char	( pgconstdata id )
	{
		return ( const char * )( g_pPgConstDataAdr + id );
	}
	inline const skyString *Get_String	( pgconstdata id )
	{
		return ( skyString * )( g_pPgConstDataAdr + id );
	}
}

} } }

//LC�̓��C�u�����R���X�g�̗�
#define C_S32( key ) sky::lib::cons::PgConst::Get_s32( key )
#define C_ST( key )  sky::lib::cons::PgConst::Get_String( key )
#define C_CHAR( key )  sky::lib::cons::PgConst::Get_Char( key )
