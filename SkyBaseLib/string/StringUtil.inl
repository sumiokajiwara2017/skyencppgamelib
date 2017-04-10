namespace sky { namespace lib { namespace string {

/// <summary>
/// 指定のフォーマットの文字を読み込む
/// scanfはうまくラッパー関数が書けないので#defineした。
/// </summary>
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
#define SkyScanf( src , format, ... )   _stscanf_s( src , format, __VA_ARGS__ )
#else
#define SkyScanf( src , format, ... )   _stscanf( src , format, __VA_ARGS__ )
#endif

} } }