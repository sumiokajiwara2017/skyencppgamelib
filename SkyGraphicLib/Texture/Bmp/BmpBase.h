#pragma once

namespace sky { namespace lib { namespace graphic {

class AbsBmpFile;
typedef SmartPointer< AbsBmpFile >	AbsBmpFilePtr;					//スマートポインタ版定義

/// <summary>
/// ビットマップベースクラス
/// </summary>
class AbsBmpFile : public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AbsBmpFile( fontbmp::eBmpKind eKind ) : m_pData( skyNull )
	{
		m_eKind = eKind;
		Reset();
	};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AbsBmpFile()
	{
		Reset();
	};

	/// <summary>
	/// リセット
	/// </summary>
	void Reset()
	{
		SAFE_DELETE( m_pData );

		m_pHed = skyNull;
		m_pBmpInfo = skyNull;
		m_pPalletAdr = skyNull;
		m_pDataAdr = skyNull;
		m_pData = skyNull;
		m_nDataSize = 0;
	}

	/// <summary>
	/// 色階調毎にカラーパレットを作成する(インデックスカラーのビットマップだけ実装する）
	/// </summary>
	virtual void vMakeColorPallet( const CFontBmpInfo &info ){ SKY_UNUSED_ARG( info ); };

	/// <summary>
	/// 文字情報を書き込む処理の初期処理
	/// </summary>
    virtual void vWriteFontInit( const CFontBmpInfo &info ) = 0;

	/// <summary>
	/// １文字情報を書き込む
	/// </summary>
	virtual void vWriteFont( const math::CBasicVector2 &pos , const CFontBmpInfo &info ) = 0;

	/// <summary>
	/// 指定のサイズのBmpをメモリ上に作成する
	/// </summary>
	virtual void vCreate( u32 width, u32 height, u32 color ) = 0;

	/// <summary>
	/// セーブ
	/// </summary>
	virtual void vSave( stream::IStreamPtr &dest ) = 0;

	/// <summary>
	/// ロード
	/// </summary>
	virtual void vLoad( stream::IStreamPtr &src ) = 0;

	/// <summary>
	/// 矩形情報の取得
	/// </summary>
	math::CBasicRect&  GetRect(){ return m_Rect; };

	/// <summary>
	/// 矩形情報の取得
	/// </summary>
	fontbmp::eBmpKind  GetKind(){ return m_eKind; }

protected:

	/// <summary>
	/// 種別
	/// </summary>
	fontbmp::eBmpKind   m_eKind;

	/// <summary>
	/// Bitmapヘッダー
	/// </summary>
	BITMAPFILEHEADER*	m_pHed;

	/// <summary>
	/// Bitmapヘッダー情報
	/// </summary>
	BITMAPINFO*			m_pBmpInfo;

	/// <summary>
	/// パレット部先頭アドレス
	/// </summary>
	RGBQUAD*			m_pPalletAdr;

	/// <summary>
	/// データ部先頭アドレス
	/// </summary>
	u8*					m_pDataAdr;

	/// <summary>
	/// 矩形情報
	/// </summary>
	math::CBasicRect	m_Rect;

	/// <summary>
	/// アライメント考慮済みの１ラインの幅
	/// ビットマップは２の倍数でないサイズのばあい、１ラインあたりのサイズが４バイトアライメントがとられる。
	/// </summary>
	u32					m_LineWidth;

	/// <summary>
	/// データ格納ポインタ
	/// </summary>
	u8					*m_pData;

	/// <summary>
	/// データサイズ
	/// </summary>
	u32					m_nDataSize;
};

} } }