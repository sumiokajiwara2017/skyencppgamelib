#pragma once

namespace sky { namespace lib {

/// <summary>
/// RTTIを実現するためのクラス
/// </summary>
/*
使い方

-------HogeBase.h----------------

//最上位の親クラス
class HogeBase
{
    SkyTypeDefRootRTTI( HogeBase ); ←こう宣言する★
};

-------HogeBase.cpp--------------

SkyImplementRootRTTI( HogeBase ); ←こう宣言する★


-------Hoge.h--------------------

//子クラス
class Hoge : public HogeBase
{

SkyTypeDefRTTI; ←こう宣言する★

};

-------Hoge.cpp------------------

SkyImplementRTTI( Hoge , HogeBase ); ←こう宣言する★

*/
class CRTTI
{

public:

        /// <summary>
        /// コンストラクタ
        /// </summary>
    CRTTI ( const skyMString* pName, const CRTTI* pBaseRTTI );

        /// <summary>
        /// 名前取得
        /// </summary>
    inline const skyMString* GetName() const { return m_pName; }

        /// <summary>
        /// 親クラス名取得
        /// </summary>
    inline const CRTTI* GetBaseRTTI() const { return m_pBaseRTTI; }

        /// <summary>
        /// 名前コピー
        /// </summary>
    skyBool CopyName( skyMString* pNameBuffer , u32 maxSize ) const;

protected:

    const skyMString* m_pName;

    const CRTTI* m_pBaseRTTI;
};

} }

/// <summary>
/// 最上位クラスにRTTI宣言をするマクロ
/// </summary>
#define SkyTypeDefRootRTTI( classname )                                           \
    public:                                                                       \
        static const sky::lib::CRTTI ms_RTTI;                                   \
        virtual const sky::lib::CRTTI* GetRTTI() const                      \
       {                                                                          \
                   return &ms_RTTI;                                                       \
           }                                                                          \
       static skyBool IsExactKindOf( const sky::lib::CRTTI* pRTTI ,         \
           const classname* pObject )                                             \
       {                                                                          \
           if ( !pObject )                                                        \
           {                                                                      \
               return skyFalse;                                                   \
           }                                                                      \
           return pObject->IsExactKindOf( pRTTI );                                \
        }                                                                         \
        skyBool IsExactKindOf( const sky::lib::CRTTI* pRTTI ) const         \
        {                                                                         \
            return ( GetRTTI() == pRTTI );                                        \
        }                                                                         \
        static skyBool IsKindOf ( const sky::lib::CRTTI* pRTTI ,            \
            const classname* pObject )                                            \
        {                                                                         \
            if ( !pObject )                                                       \
            {                                                                     \
                return skyFalse;                                                  \
            }                                                                     \
            return pObject->IsKindOf( pRTTI );                                    \
        }                                                                         \
        skyBool IsKindOf( const sky::lib::CRTTI* pRTTI ) const              \
        {                                                                         \
            const sky::lib::CRTTI* pTmp = GetRTTI();                        \
            while ( pTmp )                                                        \
            {                                                                     \
                if ( pTmp == pRTTI )                                              \
                {                                                                 \
                    return skyTrue;                                               \
                }                                                                 \
                pTmp = pTmp->GetBaseRTTI();                                       \
            }                                                                     \
            return skyFalse;                                                      \
        }                                                                         \
        static classname* DynamicCast( const sky::lib::CRTTI* pRTTI ,       \
            const classname* pObject )                                            \
        {                                                                         \
            if ( !pObject )                                                       \
            {                                                                     \
                return skyNull;                                                   \
            }                                                                     \
            return pObject->DynamicCast( pRTTI );                                 \
        }                                                                         \
        classname* DynamicCast( const sky::lib::CRTTI* pRTTI ) const        \
        {                                                                         \
            return ( IsKindOf( pRTTI ) ? ( classname* ) this : 0 );               \
        }

/// <summary>
/// 子クラスにRTTI宣言をするマクロ
/// </summary>
#define SkyTypeDefRTTI                                                            \
    public:                                                                       \
        static const sky::lib::CRTTI ms_RTTI;                               \
        virtual const sky::lib::CRTTI* GetRTTI() const { return &ms_RTTI; }

/// <summary>
/// 最上位クラスにRTTI実装をするマクロ
/// </summary>
#define SkyImplementRootRTTI( rootclassname )                                     \
    const sky::lib::CRTTI rootclassname::ms_RTTI( #rootclassname , 0 )

/// <summary>
/// 子クラスにRTTI実装をするマクロ
/// </summary>
#define SkyImplementRTTI( classname , baseclassname )                             \
    const sky::lib::CRTTI classname::ms_RTTI( #classname , &baseclassname::ms_RTTI )

/// <summary>
/// 実行時型チェックマクロ
/// </summary>
#define SkyIsExactKindOf( classname , pObject )                                  \
    classname::IsExactKindOf( &classname::ms_RTTI , pObject )

/// <summary>
/// 実行時型チェックマクロ
/// </summary>
#define SkyIsKindOf( classname , pObject )                                       \
    classname::IsKindOf( &classname::ms_RTTI , pObject )

/// <summary>
/// コンパイル時型キャストマクロ
/// </summary>
#define SkyStaticCast( classname , pObject )                                     \
    ( ( classname * ) pObject )

/// <summary>
/// コンパイル時型キャストマクロ（デバッグ実行時アサート）
/// </summary>
#ifdef SW_SKYLIB_DEBUG_ON
    #define SkyVerifyStaticCast( classname , pObject )                           \
        ( ( classname * ) classname::VerifyStaticCastDebug(                      \
            &classname::ms_RTTI , pObject ) )
#else
    #define SkyVerifyStaticCast( classname, pObject) ( ( classname * ) ( pObject ) )
#endif

/// <summary>
/// 実行時型キャストマクロ（失敗の場合skyNullを返す）
/// </summary>
#define SkyDynamicCast( classname , pObject )                                    \
    ( ( classname * ) classname::DynamicCast( &classname::ms_RTTI , pObject ) )
