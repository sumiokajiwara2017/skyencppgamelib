#pragma once

#ifdef SW_SKYLIB_DEBUG_ON

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// ���񂵂傳��̂g�o�̃T���v�����f�������[�h����
/// </summary>
class CMNSSampleModelFileLoader : public base::SkyObject
{

public:

	/// <summary>
	/// �t�@�C�����烍�[�h
	/// </summary>
	static void LoadFile( const hash::CHashStringPtr &path );
};

} } }

#define CMNSSampleModelFileLoader_	sky::lib::graphic::CMNSSampleModelFileLoader

#endif