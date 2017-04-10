#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// �t�o���o�𑗐M���ă��[�^�[�̃|�[�g�J���߂���B
/// get_StaticPortMappingCollection�Ŏ��s����ꍇ�� http://www.akakagemaru.info/port/windows7upnp.html �Ƃ�����Č���
/// </summary>
class CUPnPClient : public base::SkyObject
{

public:

	/// <summary>
	/// �|�[�g���������
	/// </summary>
	static void OpnePort( const skyString *name , const char *ip , u16 portNo );

	/// <summary>
	/// �|�[�g�����
	/// </summary>
	static void ClosePort( const char *ip , u16 portNo );
};

} } }
