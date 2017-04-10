namespace sky { namespace lib { namespace math {

namespace VectorUtil 
{

template< typename VECTOR3 >
inline void ComputeBestFitNormal( const VECTOR3 v[] , s32 num , VECTOR3& dest )
{
	//ゼロで初期化
	dest.Zero();

	//直前の頂点を最後の頂点として始める
	//これにより、ループ内のif文を回避できる。

	const VECTOR3 *p = &v[ num - 1 ];

	//すべての頂点について繰り返す

	for ( s32 i = 0 ; i < num ; i++ )
	{
		//現在の頂点へのショートカットを得る

		const VECTOR3 *c = &v[ i ];

		//辺ベクトルの席を適切に加える

		dest.X += ( p->Z + c->Z ) * ( p->Y - c->Y );
		dest.Y += ( p->X + c->X ) * ( p->Z - c->Z );
		dest.Z += ( p->Y + c->Y ) * ( p->X - c->X );

		// 次の頂点、どうぞ

		p = c;
	
	}

	//正規化する
	dest.Normalize();
}

}

} } }