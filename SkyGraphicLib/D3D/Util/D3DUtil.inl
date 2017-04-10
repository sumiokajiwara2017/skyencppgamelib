#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

namespace D3DUtil
{

	inline void Convert4x3Matrix( const sky::lib::math::CMatrix4x3 &src , XMMATRIX &dest )
	{
		//DirectXのワールドマトリックスに設定
		dest._11 = ( f32 )src.M11;
		dest._12 = ( f32 )src.M12;
		dest._13 = ( f32 )src.M13;
		dest._14 = ( f32 )src.M14;
			
		dest._21 = ( f32 )src.M21;
		dest._22 = ( f32 )src.M22;
		dest._23 = ( f32 )src.M23;
		dest._24 = ( f32 )src.M24;
			
		dest._31 = ( f32 )src.M31;
		dest._32 = ( f32 )src.M32;
		dest._33 = ( f32 )src.M33;
		dest._34 = ( f32 )src.M34;
			
		dest._41 = ( f32 )src.Tx;
		dest._42 = ( f32 )src.Ty;
		dest._43 = ( f32 )src.Tz;
		dest._44 = ( f32 )src.Tw;
	}

	inline void Convert4x3Matrix( const sky::lib::math::CMatrix4x3 &src , XMFLOAT4X4 &dest )
	{
		//DirectXのワールドマトリックスに設定
		dest._11 = ( f32 )src.M11;
		dest._12 = ( f32 )src.M12;
		dest._13 = ( f32 )src.M13;
		dest._14 = ( f32 )src.M14;
			
		dest._21 = ( f32 )src.M21;
		dest._22 = ( f32 )src.M22;
		dest._23 = ( f32 )src.M23;
		dest._24 = ( f32 )src.M24;
			
		dest._31 = ( f32 )src.M31;
		dest._32 = ( f32 )src.M32;
		dest._33 = ( f32 )src.M33;
		dest._34 = ( f32 )src.M34;
			
		dest._41 = ( f32 )src.Tx;
		dest._42 = ( f32 )src.Ty;
		dest._43 = ( f32 )src.Tz;
		dest._44 = ( f32 )src.Tw;
	}


	inline void Convert4x3MatrixRv( const XMMATRIX& src , math::CMatrix4x3& dest )
	{
		//DirectXのワールドマトリックスに設定
		dest.M11 = ( f32 )src._11;
		dest.M12 = ( f32 )src._12;
		dest.M13 = ( f32 )src._13;
		dest.M14 = ( f32 )src._14;
			
		dest.M21 = ( f32 )src._21;
		dest.M22 = ( f32 )src._22;
		dest.M23 = ( f32 )src._23;
		dest.M24 = ( f32 )src._24;
			
		dest.M31 = ( f32 )src._31;
		dest.M32 = ( f32 )src._32;
		dest.M33 = ( f32 )src._33;
		dest.M34 = ( f32 )src._34;
			
		dest.Tx = ( f32 )src._41;
		dest.Ty = ( f32 )src._42;
		dest.Tz = ( f32 )src._43;
		dest.Tw = ( f32 )src._44;
	}

	inline void ConvertRotationMatrixRv( const XMMATRIX& src , math::CRotationMatrix& dest )
	{
		dest.M11 = ( f32 )src._11;
		dest.M12 = ( f32 )src._12;
		dest.M13 = ( f32 )src._13;
			
		dest.M21 = ( f32 )src._21;
		dest.M22 = ( f32 )src._22;
		dest.M23 = ( f32 )src._23;
			
		dest.M31 = ( f32 )src._31;
		dest.M32 = ( f32 )src._32;
		dest.M33 = ( f32 )src._33;
	}

	inline void PrintD3DMatrix( XMMATRIX& src )
	{
		SKY_UNUSED_ARG( src );
		SKY_PRINTF( _T( "[ 11[ %+-08.3f ] , 12[ %+-08.3f ] , 13[ %+-08.3f ] , 14[ %+-08.3f ] ] \n" ) , src._11 , src._12 , src._13 , src._14 );
		SKY_PRINTF( _T( "[ 21[ %+-08.3f ] , 22[ %+-08.3f ] , 23[ %+-08.3f ] , 24[ %+-08.3f ] ] \n" ) , src._21 , src._22 , src._23 , src._24 );
		SKY_PRINTF( _T( "[ 31[ %+-08.4f ] , 32[ %+-08.4f ] , 33[ %+-08.4f ] , 34[ %+-08.4f ] ] \n" ) , src._31 , src._32 , src._33 , src._34 );
		SKY_PRINTF( _T( "[ 41[ %+-08.3f ] , 42[ %+-08.3f ] , 43[ %+-08.3f ] , 44[ %+-08.3f ] ] \n" ) , src._41 , src._42 , src._43 , src._44 );
	}

	inline void ConvertVector3ToVector3( const math::CBasicVector3 &src , XMFLOAT3 &dest )
	{
		dest.x = ( FLOAT )src.X;
		dest.y = ( FLOAT )src.Y;
		dest.z = ( FLOAT )src.Z;
	}

	inline void ConvertVector3Vector4( const math::CBasicVector3 &src , XMFLOAT4 &dest )
	{
		dest.x = ( FLOAT )src.X;
		dest.y = ( FLOAT )src.Y;
		dest.z = ( FLOAT )src.Z;
		dest.w = 0;
	}

	inline void ConvertVector4ToVector4( const math::CBasicVector4 &src , XMFLOAT4 &dest )
	{
		dest.x = ( FLOAT )src.X;
		dest.y = ( FLOAT )src.Y;
		dest.z = ( FLOAT )src.Z;
		dest.w = ( FLOAT )src.W;
	}

	inline void ConvertColorVector3( const CColor &src , XMFLOAT3 &dest )
	{
		dest.x = src.r;
		dest.y = src.g;
		dest.z = src.b;
	}

	inline void ConvertColorVector4( const CColor &src , XMFLOAT4 &dest )
	{
		dest.x = src.r;
		dest.y = src.g;
		dest.z = src.b;
		dest.w = src.a;
	}

	inline void PrintD3D11Result( HRESULT hr )
	{
		switch( hr )
		{
		case D3D11_ERROR_FILE_NOT_FOUND:
			SKY_PRINTF( _T( "ファイルが見つかりませんでした。 \n" ) );
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			SKY_PRINTF( _T( "特定の種類のステート オブジェクトの一意のインスタンスが多すぎます。 \n" ) );
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			SKY_PRINTF( _T( "特定の種類のビュー オブジェクトの一意のインスタンスが多すぎます。 \n" ) );
			break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			SKY_PRINTF( _T( " \n" ) );
			break;
		case D3DERR_INVALIDCALL:
			SKY_PRINTF( _T( "メソッドの呼び出しが無効です。たとえば、メソッドのパラメーターが有効なポインターでない可能性があります。 \n" ) );
			break;
		case D3DERR_WASSTILLDRAWING:
			SKY_PRINTF( _T( "このサーフェスとの間で情報を転送している以前のビット演算が不完全です。 \n" ) );
			break;
		case E_FAIL:
			SKY_PRINTF( _T( "デバッグ レイヤーを有効にしてデバイスを作成しようとしましたが、該当するレイヤーがインストールされていません。\n" ) );
			break;
		case E_INVALIDARG:
			SKY_PRINTF( _T( "An invalid parameter to the return function was passed." ) ); //戻り関数に無効なパラメーターが渡されました。
			break;
		case E_OUTOFMEMORY:
			SKY_PRINTF( _T( "Direct3D が呼び出しを完了するうえで十分なメモリーを割り当てることができませんでした。\n" ) );
			break;
		case S_FALSE:
			SKY_PRINTF( _T( "正常に処理されたものの、非標準の完了を示す代替成功値です (正確な意味はコンテキストによって異なります)。\n" ) );
			break;
		case S_OK:
			SKY_PRINTF( _T( "エラーは発生していません。\n" ) );
			break;
		}
	}
}

} } }

#endif