#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// RGBA�F��ԃN���X(4byte��)
/// RGB�̐F��Ԃɉ����āA�A���t�@�`�����l�����F����ɍl��������B����́A���߁i�����x�j��\��������̂ł���i�����ɂ͂���͐F��Ԃł͂Ȃ��Ƃ��l������j�B
/// �e�v�f��1byte���A�n4byte�ŐF��\������B
/// </summary>
template< typename T = colortype >
class CRGBA_4byte : public base::SkyObject
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    CRGBA_4byte (u8 lr = 0, u8 lg = 0, u8 lb = 0, u8 la = 0);

	/// <summary>
	/// rgba�e�v�f�ւ̎Q�ƃA�N�Z�X�Q�i�ʏ�A�N�Z�X��const�A�N�Z�X�ł���j
	/// </summary>
    u8& r();
    const u8& r() const;
    u8& g();
    const u8& g() const;
    u8& b();
    const u8& b() const;
    u8& a();
    const u8& a() const;

	/// <summary>
	/// rgba�̏���32bit�Ƀp�b�N
	/// </summary>
	u32 rgba() const;

	/// <summary>
	/// u32�l�Œl���擾
	/// </summary>
    void GetAs( u32& dest ) const;
    u32  GetAs() const;
    void GetAsBGRA( u32& dest ) const;
    u32  GetAsBGRA() const;
    void GetAsARGB( u32& dest ) const;
    u32  GetAsARGB() const;

	/// <summary>
	/// �F�N���X�Ŏ擾
	/// </summary>
    void GetAs( CRGB< T >& rgb ) const;
    void GetAs( CRGBA< T >& rgba ) const;

	/// <summary>
	/// ������Z�q�̃I�[�o�[���C�h
	/// </summary>
    CRGBA_4byte& operator= (u8 value);

	/// <summary>
	/// ��r���Z�q�̃I�[�o�[���C�h
	/// </summary>
    bool operator== (const CRGBA_4byte< T >& rgba) const;
    bool operator!= (const CRGBA_4byte< T >& rgba) const;

	/// <summary>
	/// �Z�p���Z�q�̃I�[�o�[���C�h
	/// </summary>
    CRGBA_4byte operator+ (const CRGBA_4byte< T >& rgba) const;
    CRGBA_4byte operator- (const CRGBA_4byte< T >& rgba) const;
    CRGBA_4byte operator* (T value) const;
    CRGBA_4byte operator* (const CRGBA_4byte< T >& rgba) const;
    CRGBA_4byte operator/ (T value) const;
    CRGBA_4byte operator/ (const CRGBA_4byte< T >& rgba) const;
    CRGBA_4byte operator- () const;

    CRGBA_4byte& operator+= (const CRGBA_4byte< T >& rgba);
    CRGBA_4byte& operator-= (const CRGBA_4byte< T >& rgba);
    CRGBA_4byte& operator*= (T value);
    CRGBA_4byte& operator*= (const CRGBA_4byte< T >& rgba);
    CRGBA_4byte& operator/= (T value);
    CRGBA_4byte& operator/= (const CRGBA_4byte< T >& rgba);

private:

	/// <summary>
	/// �l�^
	/// </summary>
    struct rgba_
    {
        u8 r, g, b, a;
    };

	/// <summary>
	/// �l
	/// </summary>
    union 
    {
        rgba_	m_rgba;
        u32		m_rgba_u32;
    }; 
};

/// <summary>
/// ��{RGBA�^
/// </summary>
typedef CRGBA_4byte< colortype >	CBasicRGBA_4byte;

/// <summary>
/// �F�萔
/// </summary>
static const CBasicRGBA_4byte CBasicRGBA_4byte_White      ( 255 , 255 , 255 , 255 );
static const CBasicRGBA_4byte CBasicRGBA_4byte_Black      ( 0   , 0   , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Red        ( 255 , 0   , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Green      ( 0   , 255 , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Blue       ( 0   , 0   , 255 , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Tomato     ( 255 , 99  , 71  , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Orangered  ( 255 , 69  , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Sandybrown ( 244 , 164 , 96  , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Yellow     ( 255 , 255 , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Violetw    ( 238 , 130 , 238 , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Silver     ( 192 , 192 , 192 , 255 ); 
} } }