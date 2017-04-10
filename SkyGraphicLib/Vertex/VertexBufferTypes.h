#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// 頂点関連定義
/// </summary>
namespace vertex {

	/// <summary>
	/// プリミティブの種類（頂点のプリミティブタイプは以下のいずれか一つ）
	/// </summary>
	enum ePrimitiveType
	{
		ePrimitiveType_Pointlist = 0,		//座標を点で描画する
		ePrimitiveType_Linelist,			//各点を二つずつ順番に線で結ぶ
		ePrimitiveType_Linestrip,			//各点を連続的に線で結ぶ
		ePrimitiveType_Trianglelist,		//各点で三角形を描画する。3つの点ごとに三角形が作られる。頂点の数が足りない場合はその頂点は描画されない。
		ePrimitiveType_Meshtrip,		//三角形をつなげたような図形で描画
		ePrimitiveType_Trianglefan,			//頂点を順番どおりに結んでいき、さらに始点と各点を結ぶ。

		ePrimitiveType_None,
	};

	/// <summary>
	/// 頂点フォーマット：頂点座標情報
	/// </summary>
	struct CVertexFmt_Pos
	{
		f32			x;       // Ｘ座標
		f32			y;       // Ｙ座標
		f32			z;       // Ｚ座標

		//コンストラクタ
		CVertexFmt_Pos(){}
		CVertexFmt_Pos( f32 x_ , f32 y_ , f32 z_ )
		{
			x = x_;
			y = y_;
			z = z_;
		}
		CVertexFmt_Pos( const math::CBasicVector3& src )
		{
			*this = src;
		}

		//代入用関数（構造体のサイズにはカウントされない）
        inline CVertexFmt_Pos& operator =( const math::CBasicVector3& src )
		{
			x = ( f32 )src.X; 
			y = ( f32 )src.Y; 
			z = ( f32 )src.Z;
			return *this;
		}
		inline math::CBasicVector3 Get(){ return math::CBasicVector3( x , y , z ); }
	};

	/// <summary>
	/// 頂点フォーマット：Ｄ３Ｄ法線向き情報
	/// </summary>
	struct CVertexFmt_Normal
	{
		f32			nx;      // 法線Ｘ向き
		f32			ny;      // 法線Ｘ向き
		f32			nz;      // 法線Ｘ向き

		//コンストラクタ
		CVertexFmt_Normal(){}
		CVertexFmt_Normal( f32 x , f32 y , f32 z )
		{
			nx = x; 
			ny = y; 
			nz = z;
		}
		CVertexFmt_Normal( const math::CBasicVector3& src )
		{
			*this = src;
		}

		//代入用関数（構造体のサイズにはカウントされない）
        inline CVertexFmt_Normal& operator =( const math::CBasicVector3& src )
		{
			nx = ( f32 )src.X; 
			ny = ( f32 )src.Y; 
			nz = ( f32 )src.Z;
			return *this;
		}
        inline CVertexFmt_Normal& operator +=( const math::CBasicVector3& src )
		{
			nx += ( f32 )src.X; 
			ny += ( f32 )src.Y; 
			nz += ( f32 )src.Z;
			return *this;
		}
        inline CVertexFmt_Normal& operator +=( const CVertexFmt_Normal& src )
		{
			nx += ( f32 )src.nx; 
			ny += ( f32 )src.ny; 
			nz += ( f32 )src.nz;
			return *this;
		}
		inline math::CBasicVector3 Get(){ return math::CBasicVector3( nx , ny , nz ); }
	};

	/// <summary>
	/// 頂点フォーマット：カラー値情報
	/// </summary>
	struct CVertexFmt_Color
	{
		f32         color[ 4 ]; //RGBA

		//コンストラクタ
		CVertexFmt_Color(){}
		CVertexFmt_Color( const graphic::CColor& src )
		{
			*this = src;
		}

		//代入用関数（構造体のサイズにはカウントされない）
        inline CVertexFmt_Color& operator =( const CColor& src )
		{
			color[ 0 ] = ( f32 )src.r;
			color[ 1 ] = ( f32 )src.g;
			color[ 2 ] = ( f32 )src.b;
			color[ 3 ] = ( f32 )src.a;
			return *this;
		}
		inline CColor Get(){ return CColor( color[ 0 ] , color[ 1 ] , color[ 2 ] , color[ 3 ] ); }
	};

	/// <summary>
	/// 頂点フォーマット：テクスチャ座標情報
	/// </summary>
	struct CVertexFmt_UV
	{
		f32			tu;      // テクスチャＵＶ座標Ｕ
		f32			tv;      // テクスチャＵＶ座標Ｖ

		//コンストラクタ
		CVertexFmt_UV(){}
		CVertexFmt_UV( const math::CBasicVector2& src )
		{
			*this = src;
		}
		CVertexFmt_UV( f32 tu_ , f32 tv_ )
		{
			tu = tu_;
			tv = tv_;
		}

		//代入用関数（構造体のサイズにはカウントされない）
        inline CVertexFmt_UV& operator =( const math::CBasicVector2& src )
		{
			tu = ( f32 )src.X; 
			tv = ( f32 )src.Y; 
			return *this;
		}
		inline math::CBasicVector2 Get(){ return math::CBasicVector2( tu , tv ); }
	};

	/// <summary>
	/// スキンメッシュ用関連行列インデックス
	/// </summary>
	struct CVertexFmt_Index
	{
		CVertexFmt_Index( f32 initValue )
		{
			for( u32 i = 0 ; i < 4 ; i++ )
			{
				index[ i ] = initValue;
			}
		}
		f32 index[ 4 ];
	};

	/// <summary>
	/// スキンメッシュ用関連行列ウェイト
	/// </summary>
	struct CVertexFmt_Weights
	{
		CVertexFmt_Weights( f32 initValue )
		{
			for( u32 i = 0 ; i < 4 ; i++ )
			{
				weights[ i ] = initValue;
			}
		}
		f32 weights[ 4 ];
	};

}

} } }