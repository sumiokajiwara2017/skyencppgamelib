#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_KFM

#include "SkyBaseLib/Kfm.h"

using namespace sky::lib;

static void ColValueEventDelegate_f32( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	f32 *time   = ( f32 * )args.Getu32( 0 );
	f32 *value  = ( f32 * )args.Getu32( 1 );
	f32 *weight = ( f32 * )args.Getu32( 2 );

	SKY_PRINTF( _T( "time=%f f32=[ %f ]( %f ) \n" ) , *time , *value , *weight );
}

static void ColValueEventDelegate_u32( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	f32 *time  = ( f32 * )args.Getu32( 0 );
	u32 *value = ( u32 * )args.Getu32( 1 );
	f32 *weight = ( f32 * )args.Getu32( 2 );

	SKY_PRINTF( _T( "time=%f u32=[ %d ]( %f )  \n" ) , *time , *value , *weight );
}

#if ON_

TEST( Kfm , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//任意の型でファンクションカーブを作成できる。
	kfm::CFCurve< f32 > fcf32( 0 );
	kfm::CFCurve< f32 > fcf32_next( 0 );
	kfm::CFCurve< u32 > fcu32( 0 );

	//指定の時間、値、補間タイプを指定してキー値を加えることができる。
	fcf32.AddKey( 100 , 1.0f , eInterpolateType_Linear ); 
	fcf32.AddKey( 100 , 2.0f , eInterpolateType_Linear ); 
	fcf32.AddKey( 100 , 3.0f , eInterpolateType_Linear ); 
	fcf32.AddKey( 100 , 4.0f , eInterpolateType_Linear );

	fcf32_next.AddKey( 100 , 1.0f , eInterpolateType_Linear ); 
	fcf32_next.AddKey( 100 , 2.0f , eInterpolateType_Linear ); 
	fcf32_next.AddKey( 100 , 3.0f , eInterpolateType_Linear ); 
	fcf32_next.AddKey( 100 , 4.0f , eInterpolateType_Linear ); 

	fcu32.AddKey( 100 , 100 , eInterpolateType_Linear ); 
	fcu32.AddKey( 100 , 200 , eInterpolateType_Linear ); 
	fcu32.AddKey( 100 , 300 , eInterpolateType_Linear ); 
	fcu32.AddKey( 100 , 400 , eInterpolateType_Linear ); 
	fcu32.AddKey( 100 , 500 , eInterpolateType_Linear ); 


	kfm::CFCurvePlayer< f32 > fcurvePlayer_f32;
	fcurvePlayer_f32.GetColValueEventHndl() += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ColValueEventDelegate_f32 ) ); 
	fcurvePlayer_f32.PlayFCurveRequest( &fcf32 );
	fcurvePlayer_f32.PlayFCurveRequest( &fcf32_next , 100.f );

	kfm::CFCurvePlayer< u32 > fcurvePlayer_u32;
	fcurvePlayer_u32.GetColValueEventHndl() += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ColValueEventDelegate_u32 ) ); 
	fcurvePlayer_u32.PlayFCurveRequest( &fcu32 , 100.0f );

	//仮想ゲームループ
	for( f32 i = 0.0f ; i <= 2400 ; i++ )
	{
		SKY_PRINTF( _T( "=== %f \n" ) , i );
		fcurvePlayer_f32.Play( 1.0f );
		fcurvePlayer_u32.Play( 1.0f );
	}

	CheckMemBreak();

	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

TEST( Kfm , 2 ) 
{
	SKY_BASE_LIB_INIT();

	//任意の型でファンクションカーブを作成できる。
	kfm::CFCurve< f32 > fcf32;
	kfm::CFCurve< u32 > fcu32;

	//指定の時間、値、補間タイプを指定してキー値を加えることができる。
	fcf32.AddKey( 100 , 0.0f , kfm::eInterpolateType_Linear ); 
	fcf32.AddKey( 100 , 1.0f , kfm::eInterpolateType_Linear ); 
	fcf32.AddKey( 100 , 2.0f , kfm::eInterpolateType_Linear ); 
	fcf32.AddKey( 100 , 3.0f , kfm::eInterpolateType_Linear ); 
//	fcf32.ClearKey();

	fcu32.AddKey( 100 , 0   , kfm::eInterpolateType_Linear ); 
	fcu32.AddKey( 100 , 100 , kfm::eInterpolateType_Linear ); 
	fcu32.AddKey( 100 , 200 , kfm::eInterpolateType_Linear ); 
	fcu32.AddKey( 100 , 300 , kfm::eInterpolateType_Linear ); 
	fcu32.AddKey( 100 , 400 , kfm::eInterpolateType_Linear ); 

	//Execが呼ばれるたびにデリゲートをコールしてもらい値を受け取ることが可能
	fcf32.GetColValueEventHndl() += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ColValueEventDelegate_f32 ) );
	fcu32.GetColValueEventHndl() += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ColValueEventDelegate_u32 ) );

	//シーケンスに登録する
	kfm::CKfmSequence sequence;
	sequence.AddFCurve( 0.0f , &fcf32 );
	sequence.AddFCurve( 0.0f , &fcu32 );

	//シーケンスプレイヤー作成
	kfm::CKfmSequencePlayer player;

	//再生開始
	player.PlayReverseSequenceRequest( &sequence );
	player.PlayReverseSequenceRequest( &sequence , 100.0f );
	player.PlayReverseSequenceRequest( &sequence , 100.0f );
//	player.PlaySequence( &sequence );

	//仮想ゲームループ
	for( f32 i = 0.0f ; i <= 2400 ; i++ )
	{
		SKY_PRINTF( _T( "=== %f \n" ) , i );
		player.Play( 1.0f );

		if ( 800 == i )
		{
			player.NextCurrentSequence();
		}
	}

	CheckMemBreak();

	SKY_BASE_LIB_TERM();
}

#endif

#endif