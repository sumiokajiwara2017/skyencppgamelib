#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// �l�b�g���[�N�C���^�[�t�F�[�X���
/// </summary>
struct CNetWorkInterfaceInfo
{
	u32             index;                    //�l�b�g���[�N�@��Ɋ���U���Ă���C���f�b�N�X�ԍ�
	string::CString m_Name;                   //���ʖ�

	//�A�h���X���iIP�e�[�u�����j
	string::CString m_IPAddressString;        //IP�A�h���X
	u32             m_IPAddress;              //IP�A�h���X�i�o�C�i���j�����ł͂��������g����͂�
	string::CString m_NetMaskString;          //�l�b�g�}�X�N
	u32             m_NetMask;                //�l�b�g�}�X�N�i�o�C�i���j�����ł͂����炪�g����͂�
	string::CString m_BrodCastAddressString;  //�u���[�h�L���X�g�A�h���X
	u32             m_BrodCastAddress;        //�u���[�h�L���X�g�A�h���X�i�o�C�i���j�����ł͂����炪�g����͂�

	void Print()
	{
		SKY_PRINTF( _T ( " CNetWorkInterfaceInfo : index = [ %d ] name = [ %s ] IPAddress= [ %s ] NetMask= [ %s ] BrodCastAddress= [ %s ] PhysAddr= [ %s ] \n" ) , 
			index , 
			( const skyString * )m_Name , 
			( const skyString * )m_IPAddressString , 
			( const skyString * )m_NetMaskString , 
			( const skyString * )m_BrodCastAddressString );
	}
};
typedef CList< u32 , CNetWorkInterfaceInfo > CNetWorkInterfaceInfoList;

/// <summary>
/// �c�m�r���
/// </summary>
struct CDNSInfo
{
	string::CString m_IPAddressString;             //IP�A�h���X
	u32             m_IPAddress;                   //IP�A�h���X�i�o�C�i���j
	string::CString m_IPMaskString;                //IP�}�X�N
	u32             m_IPMask;                      //IP�}�X�N�i�o�C�i���j

	void Print()
	{
		SKY_PRINTF( _T( "CDNSInfo : IPAddress = [ %s ] IPMask = [ %s ] \n" ) , ( const skyString * )m_IPAddressString , ( const skyString * )m_IPMaskString );
	}
};
typedef CList< u32 , CDNSInfo > CDNSInfoList;

/// <summary>
/// �z�X�g�Ɋ֌W����c�m�r���
/// </summary>
struct CHostDNSInfo
{
	string::CString m_HostName;                  //�z�X�g��
	string::CString m_DomainName;                //�z�X�g����������h���C����
	CDNSInfoList    m_DNSServerList;             //DNS�T�[�o�[���X�g

	void Print()
	{
		SKY_PRINTF( _T( "CHostDNSInfo : HostName = [ %s ] DomainName = [ %s ] \n" ) , ( const skyString * )m_HostName , ( const skyString * )m_DomainName );

		FOREACH( CDNSInfoList , it , m_DNSServerList )
		{
			( *it ).Print();
		}
	}
};

/// <summary>
/// �l�b�g���[�N���I�u�W�F�N�g
/// </summary>
namespace NetWorkInfoUtilt
{
	/// <summary>
	/// �l�b�g���[�N�C���^�[�t�F�[�X�����擾����
	/// </summary>
	void GetNetWorkInterfaceInfo( CNetWorkInterfaceInfoList &dest );

	/// <summary>
	/// DNS�����擾����
	/// </summary>
	void GetDNSInfo( CHostDNSInfo &dest );
}

} } }