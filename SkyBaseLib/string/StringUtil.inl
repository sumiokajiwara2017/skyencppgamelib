namespace sky { namespace lib { namespace string {

/// <summary>
/// �w��̃t�H�[�}�b�g�̕�����ǂݍ���
/// scanf�͂��܂����b�p�[�֐��������Ȃ��̂�#define�����B
/// </summary>
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
#define SkyScanf( src , format, ... )   _stscanf_s( src , format, __VA_ARGS__ )
#else
#define SkyScanf( src , format, ... )   _stscanf( src , format, __VA_ARGS__ )
#endif

} } }