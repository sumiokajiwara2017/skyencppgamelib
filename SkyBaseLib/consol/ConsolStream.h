#pragma once

namespace sky { namespace lib { namespace consol {

struct CConsolStreamMember;

static const u32 CONSOL_IO_BUF_LEN = 256;

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
///  コンソールストリーム
/// </summary>
class CConsolStream  : public stream::AbsStream
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
	/// </summary>  
	skyBool     vOpen();
	skyBool     vClose();
	u32			vWriteStream( const skyString *format , ... );
	u32		    vReadStream( skyString  *pData , u32 len );

	/// <summary>
	///  コンストラクタ
	/// </summary>
	CConsolStream();

	/// <summary>
	///  デストラクタ
	/// </summary>
	virtual ~CConsolStream();

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CConsolStreamMember *m_pMember;
};

} } }