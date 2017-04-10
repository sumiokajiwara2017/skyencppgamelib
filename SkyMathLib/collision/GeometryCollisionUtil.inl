namespace sky { namespace lib { namespace math {

namespace GeometryCollisionUtil {

template< typename T >
inline skyBool Pos2CrossPos2( const CVector2< T > &pos1 , const CVector2< T > &pos2 , CollisionResultInfo *info )
{
	skyBool result = ( CMathUtil::NearyEq( pos1.X , pos2.X , 0.00001f ) &&
		     CMathUtil::NearyEq( pos1.Y , pos2.Y , 0.00001f ) );

	if ( result == skyTrue && info != skyNull )
	{
		info->m_CrossPos = pos1;
	}

	return result;
}

template< typename T >
inline T Pos2AndPos2Angle( const CVector2< T > &pos1 , const CVector2< T > &pos2 )
{
    T               angle;
    CVector2< T >   vec;

	vec = pos2 - pos1;
	vec.Normalize();
	angle = CMathUtil::Atan2( vec.Y, vec.X );

	return angle;
}

template< typename T >
inline skyBool Pos2ToLineVector( const CVector2< T > &pos , const CLine2< T > &line , CVector2< T > &dest )
{
    T   t;        //媒介変数
    CVector2< T > vec1, vec2, vec3, cross;
  
    //線分Ａと線分ＢのベクトルＡＢを作成
	vec1 = line.m_Pos2 - line.m_Pos1;

    //線分Ａとある点ＰのベクトルＡＰを作成
	vec2 = pos - line.m_Pos1;

    //ベクトルＡＢ、ＡＰの内積より媒介変数ｔを求め、線分内にあるか調べる
	t = vec1.Dot( vec2 ) / ( vec1.X * vec1.X + vec1.Y * vec1.Y );

    //ある点Ｐから下ろした垂線が線分上にない。
    if( t < 0.0 || t > 1.0 )
	{
        return skyFalse;
	}

    //線分Ａと交点ＱのベクトルＡＱを作成（ベクトル方程式より）
	vec3 = vec1 * t;

    //交点Ｑを求める
	cross = line.m_Pos1 + vec3;

    //ある点Ｐと交点ＱのベクトルＰＱを作成（ベクトルの減法より）
	dest = cross - pos;

	return skyTrue;

}

template< typename T >
inline T Pos2ToLineSide( const CVector2< T > &pos , const CLine2< T > &line )
{
    T    a;
    CVector2< T >   vec1, vec2;

    //線分Ａと線分ＢのベクトルＡＢを作成
	vec1 = line.m_Pos2 - line.m_Pos1;

    //線分Ａとある点ＰのベクトルＡＰを作成
	vec2 = pos - line.m_Pos1;

    //ベクトルＡＢ、ＡＰの外積を求める
    a = Cross( vec1, vec2 );

    //正負で判定
    if( a > 0 )         //左側
        return 1;
    else if( a < 0 )    //右側
        return -1;
    else                //直線上
        return 0;

}

// ２つの直線の情報を入れ替える
template< typename T >
inline void SwapLineData( CRay2< T > &ray1, CRay2< T > &ray2 )
{
	T w;
	w = ray1.m_Pos.X;
	ray1.m_Pos.X = ray2.m_Pos.X;
	ray2.m_Pos.X = w;
	w = ray1.m_Pos.Y;
	ray1.m_Pos.Y = ray2.m_Pos.Y;
	ray2.m_Pos.Y = w;

	w = ray1.m_Vec.X;
	ray1.m_Vec.X = ray2.m_Vec.X;
	ray2.m_Vec.X = w;
	w = ray1.m_Vec.Y;
	ray1.m_Vec.Y = ray2.m_Vec.Y;
	ray2.m_Vec.Y = w;
}

template< typename T >
inline skyBool CheckParallel( CVector2< T > v1, CVector2< T > v2 )
{
	v1.Normalize();
	v2.Normalize();
	if( abs( v1.X ) != abs( v2.X ) )return skyFalse;
	if( abs( v1.Y ) != abs( v2.Y ) )return skyFalse;
	return skyTrue;
}

template< class T >
inline s32 Line2CrossLine2( const CLine2< T > &line1 , const CLine2< T > &line2 , CollisionResultInfo *info )
{
	CRay2< T > ray1;// 直線1
	ray1.m_Pos.X = line1.m_Pos1.X;
	ray1.m_Pos.Y = line1.m_Pos1.Y;
	ray1.m_Vec.X = line1.m_Pos2.X - line1.m_Pos1.X;
	ray1.m_Vec.Y = line1.m_Pos2.Y - line1.m_Pos1.Y;

	CRay2< T > ray2;// 直線2
	ray2.m_Pos.X = line2.m_Pos1.X;
	ray2.m_Pos.Y = line2.m_Pos1.Y;
	ray2.m_Vec.X = line2.m_Pos2.X - line2.m_Pos1.X;
	ray2.m_Vec.Y = line2.m_Pos2.Y - line2.m_Pos1.Y;

	if( CheckParallel< T >( ray1.m_Vec, ray2.m_Vec ) )
	{
		// 平行である場合
		skyBool length_zero2 = skyFalse;
		skyBool length_zero3 = skyFalse;
		
		CVector2< T > v1;
		v1.X = line1.m_Pos2.X - line1.m_Pos1.X;
		v1.Y = line1.m_Pos2.Y - line1.m_Pos1.Y;
		v1.Normalize();
		CVector2< T > v2;
		v2.X = line2.m_Pos1.X - line1.m_Pos1.X;
		v2.Y = line2.m_Pos1.Y - line1.m_Pos1.Y;
		v2.Normalize();
		if( (v2.X == 0.0f) && (v2.Y == 0.0f) )length_zero2 = skyTrue;
		CVector2< T > v3;
		v3.X = line2.m_Pos2.X - line1.m_Pos1.X;
		v3.Y = line2.m_Pos2.Y - line1.m_Pos1.Y;
		v3.Normalize();
		if( (v3.X == 0.0f) && (v3.Y == 0.0f) )length_zero3 = skyTrue;
		if( length_zero2 || length_zero3 )
		{
			// line21またはline22のどちらかがline11と同じ位置にある場合(接触)
			return 2;
		}
		// line21またはline22のどちらかがline11から見てline12と同じ方向に存在するなら交差している
		if( (v1.X == v2.X) && (v1.Y == v2.Y) )return 2;
		if( (v1.X == v3.X) && (v1.Y == v3.Y) )return 2;
		return 0;
	}
	if( ray1.m_Vec.X == 0.0f )
	{
		// あとでray1.m_Vec.Xによる除算があるため
		// ray1とray2を入れ替える
		SwapLineData< T >( ray1, ray2 );
	}

	dectype d = ray2.m_Vec.X * ray1.m_Vec.Y - ray2.m_Vec.Y * ray1.m_Vec.X;
	if( d == 0.0f )return 0;// なることはないと思うが念のため

	// t2を求めます
	dectype t2 = ( ray2.m_Pos.Y - ray1.m_Pos.Y - ( ( ray2.m_Pos.X - ray1.m_Pos.X ) / ray1.m_Vec.X ) * ray1.m_Vec.Y ) * ray1.m_Vec.X / d;

	// t1を求めます
	dectype t1 = ( ray2.m_Pos.X + t2*ray2.m_Vec.X - ray1.m_Pos.X ) / ray1.m_Vec.X;

	// 線分内での交差の条件はt1、t2ともに0.0～1.0の間の値をとること
	if( t1 < 0.0f || t1 > 1.0f )return 0;
	if( t2 < 0.0f || t2 > 1.0f )return 0;

	if ( info != skyNull )
	{
		info->m_CrossPos.X = ray1.m_Pos.X + t1 * ray1.m_Vec.X;
		info->m_CrossPos.Y = ray1.m_Pos.Y + t1 * ray1.m_Vec.Y;
	}

	return 1;
}

/*
　点：P(x2, y2)
線分：始点P0(x0, y0)、終点P1(x1, y1)
線分の長さ L1 = sqrt( (x1-x0)^2 + (y1-y0)^2 )
線分の始点から点までの長さ L2 = sqrt( (x2-x0)^2 + (y2-y0)^2 )

(x1-x0)*(x2-x0) + (y1-y0)*(y2-y0) が L1*L2 に等しく、かつL1≧L2の時衝突している*/
template< typename T >
inline skyBool Point2CrossLine2( const CVector2< T > &point , const CLine2< T > &line )
{
	dectype L1 = CMathUtil::Sqrt( CMathUtil::Pow( line.m_Pos2.X - line.m_Pos1.X , 2 ) + CMathUtil::Pow( line.m_Pos2.Y - line.m_Pos1.Y , 2 ) );
	dectype L2 = CMathUtil::Sqrt( CMathUtil::Pow( point.X - line.m_Pos1.X , 2 ) + CMathUtil::Pow( point.Y - line.m_Pos1.Y , 2 ) );

    dectype w1 = ( line.m_Pos2.X - line.m_Pos1.X ) * ( point.X - line.m_Pos1.X ) + ( line.m_Pos2.Y - line.m_Pos1.Y ) * ( point.Y - line.m_Pos1.Y );
    dectype w2 = L1 * L2;

	return( CMathUtil::NearyEq( w1 , w2 , 0.00001f ) && L1 >= L2 );
}

/*
円1 ： 中心点C1(xc1, yc1)、半径r1
円2 ： 中心点C2(xc2, yc2)、半径r2
(xc1-xc2)^2 + (yc1-yc2)^2 ≦ (r1+r2)^2ならば衝突している
*/
template< typename T >
inline skyBool CircleCrossCircle( const CCircle< T > &circle1 , const CCircle< T > &circle2 )
{
	return ( CMathUtil::Pow( circle1.X() - circle2.X() , 2 ) + CMathUtil::Pow( circle1.Y() - circle2.Y() , 2 ) <=
		     CMathUtil::Pow( circle1.GetRadius() + circle2.GetRadius() , 2 ) );
}

template< typename T >
inline skyBool Point2CrossRect( const CVector2< T > &pos , const CRect< T > &rect )
{   
	T left   = rect.X();
	T top    = rect.Y();
	T bottom = rect.Y() + rect.H();
	T right  = rect.X() + rect.W();

	if ( top > pos.Y || bottom < pos.Y || left > pos.X || right < pos.X )
	{
		return skyFalse;
	}
	else
	{
		return skyTrue;
	}
}

/*
矩形１座標   x1,y1
矩形１サイズ w1,h1
矩形２座標   x2,y2
矩形２サイズ w2,h2
( x1 + w1 >= x2 ) && ( x2 + w2 >= x1 ) && ( y1 + h2 >= y2 ) && ( y2 + h2 >= y1 )なら衝突している
*/
template< typename T >
inline skyBool RectCrossRect( const CRect< T > &rect1 , const CRect< T > &rect2 )
{
    return ( rect1.X() + rect1.W() >= rect2.X() ) && ( rect2.X() + rect2.W() >= rect1.X() ) 
        && ( rect1.Y() + rect2.H() >= rect2.Y() ) && ( rect2.Y() + rect2.H() >= rect1.Y() );
}

/*
矩形１中心座標   x1,y1
矩形１サイズ     w1,h1
矩形２中心座標   x2,y2
矩形２サイズ     w2,h2
( abs( x2 - x1 ) < ( w1 + w2 ) / 2 ) && abs( y2 - y1 ) < ( h1 + h2 ) / 2 ) )なら衝突している
*/
template< typename T >
inline skyBool RectCrossRect( const CVector2< T > &center1 , const CSize< T > &size1 , const  CVector2< T > &center2 , const CSize< T > &size2 )
{
	return ( CMathUtil::Abs( center2.X - center1.X ) <= ( size1.W + size2.W ) / 2 && 
             CMathUtil::Abs( center2.Y - center1.Y ) <= ( size1.H + size2.H ) / 2 );
}
	
/*
点 ： P(xp, yp)
円 ： 中心点C(xc, yc)、半径r
(xp-xc)^2 + (yp-yc)^2 ≦ r^2ならば衝突している
*/
	template< typename T >
	inline skyBool Point2CrossCircle( const CVector2< T > &point , const CCircle< T > &circle )
{
	return ( CMathUtil::Pow( point.X - circle.X() , 2 ) + CMathUtil::Pow( point.Y - circle.Y() , 2 ) <=
		     CMathUtil::Pow( circle.GetRadius() , 2 ) );
}

template< typename T >
inline skyBool Line2CrossCircle( const CLine2< T > &line , const CCircle< T > &circle )
{
    CVector2< T > v( line.m_Pos2.X - line.m_Pos1.X , line.m_Pos2.Y - line.m_Pos1.Y );
    CVector2< T > c( circle.X() - line.m_Pos1.X , circle.Y() - line.m_Pos1.Y );

	//二つのベクトルの内積を求める
    dectype n1 = v.Dot( c );

	if( n1 < 0 )
    {
		//Cの長さが円の半径より小さい場合 : 交差している
		return ( c.Length() <= circle.GetRadius() ) ? skyTrue : skyFalse;
	}

    dectype n2 = v.Dot( v );

	if( n1 > n2 )
    {
		//線分の終点と円の中心の距離の二乗を求める
        CVector2< T > w( circle.X() - line.m_Pos2.X , circle.Y() - line.m_Pos2.Y );
		dectype len = CMathUtil::Pow( w.Length() , 2 );

		//円の半径の二乗よりも小さい場合 : 交差している
		return ( len <= CMathUtil::Pow( circle.GetRadius() , 2 ) ) ? skyTrue : skyFalse;
	}
    else
    {
		dectype n3 = c.Dot( c );
		return ( n3 - ( n1 / n2 ) * n1 <= CMathUtil::Pow( circle.GetRadius() , 2 ) ) ? skyTrue : skyFalse;
	}
}

/*
点：P(x2, y2)
レイ：「点(x0, y0) を通り向きv=(vx, vy)」
(x2-x0)*vy - vx*(y2-y0)が 0 ならば点と線は衝突している
*/
template< typename T >
inline skyBool Point2CrossRay2( const CVector2< T > &point , const CRay2< T > &ray2 )
{
    dectype src1 = ( point.X - ray2.m_Pos.X ) * ray2.m_Vec.Y - ray2.m_Vec.X * ( point.Y - ray2.m_Pos.Y );

	return CMathUtil::NearyEq( src1 , 0.0f , 0.00001f );
}

template< typename T >
inline skyBool Point2CrossEllipse( const CVector2< T > &point , const CEllipse< T > &ellipse )
{
	// 点に楕円→真円変換行列を適用
	dectype Ofs_x = point.X - ellipse.X();
	dectype Ofs_y = point.Y - ellipse.Y();
	dectype After_x = Ofs_x * CMathUtil::Cos( ellipse.ROT() ) + Ofs_y * CMathUtil::Sin( ellipse.ROT() );
	dectype After_y = ellipse.RW() / ellipse.RH() * ( -Ofs_x * CMathUtil::Sin( ellipse.ROT() ) + Ofs_y * CMathUtil::Cos( ellipse.ROT() ) );

	// 原点から移動後点までの距離を算出
	if( After_x * After_x + After_y * After_y <= ellipse.RW() * ellipse.RW() )
		return skyTrue;   // 衝突

	return skyFalse;
}

template< typename T >
inline skyBool EllipseCrossEllipse( const CEllipse< T > &ellipse1 , const CEllipse< T > &ellipse2 )
{

   // STEP1 : E2を単位円にする変換をE1に施す
   dectype DefAng = ellipse1.ROT()-ellipse2.ROT();
   dectype Cos = CMathUtil::Cos( DefAng );
   dectype Sin = CMathUtil::Sin( DefAng );
   dectype nx = ellipse2.RW() * Cos;
   dectype ny = -ellipse2.RW() * Sin;
   dectype px = ellipse2.RH() * Sin;
   dectype py = ellipse2.RH() * Cos;
   dectype ox = CMathUtil::Cos( ellipse1.ROT() )*(ellipse2.X()-ellipse1.X()) + CMathUtil::Sin(ellipse1.ROT())*(ellipse2.Y()-ellipse1.Y());
   dectype oy = -CMathUtil::Sin( ellipse1.ROT() )*(ellipse2.X()-ellipse1.X()) + CMathUtil::Cos(ellipse1.ROT())*(ellipse2.Y()-ellipse1.Y());

   // STEP2 : 一般式A～Gの算出
   dectype rx_pow2 = 1/(ellipse1.RW()*ellipse1.RW());
   dectype ry_pow2 = 1/(ellipse1.RH()*ellipse1.RH());
   dectype A = rx_pow2*nx*nx + ry_pow2*ny*ny;
   dectype B = rx_pow2*px*px + ry_pow2*py*py;
   dectype D = 2*rx_pow2*nx*px + 2*ry_pow2*ny*py;
   dectype E = 2*rx_pow2*nx*ox + 2*ry_pow2*ny*oy;
   dectype F = 2*rx_pow2*px*ox + 2*ry_pow2*py*oy;
   dectype G = ( ox / ellipse1.RW() )*( ox / ellipse1.RW() ) + ( oy / ellipse1.RH() ) * ( oy / ellipse1.RH() ) - 1;

   // STEP3 : 平行移動量(h,k)及び回転角度θの算出
   dectype tmp1 = 1/(D*D-4*A*B);
   dectype h = (F*D-2*E*B)*tmp1;
   dectype k = (E*D-2*A*F)*tmp1;
   dectype Th = (B-A)==0?0:CMathUtil::Atan( D/(B-A) ) * 0.5f;

   // STEP4 : +1楕円を元に戻した式で当たり判定
   dectype CosTh = CMathUtil::Cos(Th);
   dectype SinTh = CMathUtil::Sin(Th);
   dectype A_tt = A*CosTh*CosTh + B*SinTh*SinTh - D*CosTh*SinTh;
   dectype B_tt = A*SinTh*SinTh + B*CosTh*CosTh + D*CosTh*SinTh;
   dectype KK = A*h*h + B*k*k + D*h*k - E*h - F*k + G;
   if(KK>0) KK = 0; // 念のため
   dectype Rx_tt = 1+sqrt(-KK/A_tt);
   dectype Ry_tt = 1+sqrt(-KK/B_tt);
   dectype x_tt = CosTh*h-SinTh*k;
   dectype y_tt = SinTh*h+CosTh*k;
   dectype JudgeValue = x_tt*x_tt/(Rx_tt*Rx_tt) + y_tt*y_tt/(Ry_tt*Ry_tt);

   if( JudgeValue <= 1 )
      return skyTrue; // 衝突

   return skyFalse;
}

template< typename T >
inline skyBool Pos3CrossPos3( const CVector3< T > &pos1 , const CVector3< T > &pos2 )
{
	return ( CMathUtil::NearyEq( pos1.X , pos2.X , 0.00001f ) &&
	         CMathUtil::NearyEq( pos1.Y , pos2.Y , 0.00001f ) &&
		     CMathUtil::NearyEq( pos1.Z , pos2.Z , 0.00001f ) );
}

/*
線 ： 線上の点P0(x0, y0, z0)、線の方向を示すベクトルv=(vx, vy, vz)
点 ： P1(x1, y1, z1)
v1 = P1 - P0
|v1×v|が0ならば衝突を起こしている
*/
template< typename T >
inline skyBool Pos3CrossRay3( const CVector3< T > &pos , const CRay3< T > &ray )
{
	//P0を始点としてP1を終点とするベクトルv1を算出
	CVector3< T > v1 = pos - ray.m_Pos;
	CVector3< T > a  = Cross( v1 , ray.m_Vec );

	return CMathUtil::NearyEq( a.Length() , 0 , 0.00001f );
}

/*
線分 ： Ps(sx,sy,sz), Pe(ex,ey,ez)
　点 ： P1(x1, y1, z1)
線分の方向を表すベクトルをv=(ex-sx, ey-sy, ez-sz)、PsからP1へ向かうベクトルをv1
v = Pe - Ps
v1 = P1 - Ps
v1×v|が0、かつ|v1|≦|v|ならば衝突を起こしている
*/
template< typename T >
inline skyBool Pos3CrossLine3( const CVector3< T > &pos , const CLine3< T > &line )
{
	CVector3< T > v = line.m_Pos2 - line.m_Pos1;
	CVector3< T > v1 = pos - line.m_Pos1;

    CVector3< T > a = Cross( v1 , v );
	return CMathUtil::NearyEq( a.Length() , 0 , 0.00001f ) && v1.Length() <= v.Length();
}

/*
線分 ： Ps(sx,sy,sz), Pe(ex,ey,ez)
　点 ： P1(x1, y1, z1)
v = Pe - Ps
vP= P1 - Ps
v * Dot( Normalize( v ), vP ) / Length( v ) - vP
*/
template< typename T >
inline CVector3< T > Pos3ToLine3( const CVector3< T > &pos , const CLine3< T > &line )
{
	CVector3< T > v  = line.m_Pos2 - line.m_Pos1;
	CVector3< T > nv = line.m_Pos2 - line.m_Pos1; nv.Normalize();
	CVector3< T > vP = pos - line.m_Pos1;

	CVector3< T > a = v * ( nv.Dot( vP ) / v.Length() ) - vP;
	return a;
}

template< typename T >
inline skyBool Pos3CrossSphere( const CVector3< T > &point , const CSphere< T > &sphere )
{
	return ( CMathUtil::Pow( CMathUtil::Abs( point.X - sphere.X() ) , 2 ) + CMathUtil::Pow( CMathUtil::Abs( point.Y - sphere.Y() ) , 2 ) + CMathUtil::Pow( CMathUtil::Abs( point.Z - sphere.Z() ) , 2 ) <=
		     CMathUtil::Pow( sphere.GetRadius() , 2 ) );
}

/*
線分 ： n1
線分 ： n2
v1とv2の外積を取るとそれらベクトルを含む平面の法線ベクトルn1
v1とv3でも同様に法線ベクトルn2
n1の始点からn2の始点へ向かうベクトルn3
・ |n2|==0ならば衝突を起こしている。
・ |n2|!=0で、|n1|>0の時、|n1×n2|==0なら衝突を起こしている。
*/
template< typename T >
inline skyBool Line3CrossLine3( const CLine3< T > &line1 , const CLine3< T > &line2 )
{
	CVector3< T > v1 = line1.m_Pos2 - line1.m_Pos1;
	CVector3< T > v3 = line2.m_Pos2 - line1.m_Pos1;
	CVector3< T > n2 = Cross( v1 , v3 );
	if( CMathUtil::NearyEq( n2.Length() , 0 , 0.00001f ) ) return skyTrue;

	CVector3< T > v2 = line2.m_Pos2 - line2.m_Pos1;
	CVector3< T > n1 = Cross( v1 , v2 );

	if( n2.Length() != 0 && n1.Length() > 0 && CMathUtil::NearyEq( Cross( n1 , n2 ).Length() , 0 , 0.00001f ) )
	{
		return skyTrue;
	}
	return skyFalse;
}

inline skyBool Line3CrossLine3Fence( const CBasicLine3 &line , const math::CBasicVector3Vector &fence , skyBool isHitCheck2D )
{
	skyBool result = skyFalse;
	if ( fence.GetNum() > 1 )
	{
		for ( u32 i = 0 ; i < fence.GetNum() - 1 ; i++ )
		{
			CBasicVector3 pos1 = fence[ i ];
			CBasicVector3 pos2 = fence[ i + 1 ];

			if ( isHitCheck2D == skyTrue )
			{
				CBasicLine2 line( CBasicVector2( line.m_Pos1.X , line.m_Pos1.Z ) , CBasicVector2( line.m_Pos2.X , line.m_Pos2.Z ) );
				result = 0 != Line2CrossLine2< dectype >( line , CBasicLine2( CBasicVector2( pos1.X , pos1.Z ) , CBasicVector2( pos2.X , pos2.Z ) ) );
			}
			else
			{
				result = Line3CrossLine3< dectype >( line , CBasicLine3( pos1 , pos2 ) );
			}

			if ( result == skyTrue )
			{
				break;
			}
		}
	}
	return result;
}

template< typename T >
inline void ColPos3Normal( const CVector3< T > &pos1, const CVector3< T > &pos2, const CVector3< T > &pos3, CVector3< T > &normal )
{
    CVector3< T > vec1, vec2;

    //法線ベクトルを求める
	vec1 = pos2 - pos1;
	vec2 = pos3 - pos1;
	normal = Cross( vec1 , vec2 );
}

template< typename T >
inline skyBool PlanePos3ToTriangleInSide( const CPlane< T > &plane , const CTriangle3< T > &triangle )
{
    T              a;
    CVector3< T >  vec;

    // 三角形ＰＡＢの法線ベクトルＮ１を作成 
	ColPos3Normal< T >( plane.m_Pos , triangle.m_Pos[ 0 ] , triangle.m_Pos[ 1 ] , vec );
	a = vec.Dot( plane.m_Normal );

    // 法線の向きとちがうので外側
    if( a < 0.0 )
        return skyFalse;

    // 三角形ＰＢＣの法線ベクトルＮ２を作成
	ColPos3Normal< T >( plane.m_Pos , triangle.m_Pos[ 1 ] , triangle.m_Pos[ 2 ] , vec );
	a = vec.Dot( plane.m_Normal );

    // 法線の向きとちがうので外側
    if( a < 0.0 )
        return skyFalse;

    // 三角形ＰＢＣの法線ベクトルＮ２を作成
	ColPos3Normal< T >( plane.m_Pos , triangle.m_Pos[ 1 ] , triangle.m_Pos[ 2 ] , vec );
	a = vec.Dot( plane.m_Normal );

    // 法線の向きとちがうので外側
    if( a < 0.0 )
        return skyFalse;

    // 三角形ＰＣＡの法線ベクトルＮ３を作成
	ColPos3Normal< T >( plane.m_Pos , triangle.m_Pos[ 2 ] , triangle.m_Pos[ 0 ] , vec );
	a = vec.Dot( plane.m_Normal );

    // 法線の向きとちがうので外側
    if( a < 0.0 )
        return skyFalse;

	return skyTrue;
}

template< typename T >
inline skyBool Ray3CrossPlane( const CRay3< T > &ray , const CPlane< T > &plane , CollisionResultInfo *info )
{
    dectype    w = 0 , a = 0, b = 0, t = 0;
    CVector3< T >   vec;

	vec = plane.m_Pos - ray.m_Pos;
	w = ray.m_Vec.Dot( vec );

	//レイは平面と反対方向に行ってるので交点なし
	if ( 0 > w )
		return skyFalse;

	a = vec.Dot( plane.m_Normal );
	b = ray.m_Vec.Dot( plane.m_Normal );

    //平行なので交点なし
	if( CMathUtil::NearyEq( b , 0 , 0.00001f ) )
        return skyFalse;

    //媒介変数ｔを求める
    t = a / b;

	if ( info != skyNull )
	{
		//直線の方程式に代入し、交点を求める
		info->m_CrossPos.X = ray.m_Pos.X + ray.m_Vec.X * t;
		info->m_CrossPos.Y = ray.m_Pos.Y + ray.m_Vec.Y * t;
		info->m_CrossPos.Z = ray.m_Pos.Z + ray.m_Vec.Z * t;
	}

    //交点あり
    return skyTrue;
}

template< typename T >
inline skyBool Ray3CrossTriangle3( const CRay3< T > &ray , const CTriangle3< T > &triangle , CollisionResultInfo *info )
{
	skyBool result = skyFalse;

	//三角形から平面を作る
	CPlane< T > plane( triangle );

	//レイと平面の交点を求める
	result = Ray3CrossPlane< T >( ray , plane , info );

	if ( result == skyTrue )
	{
		//交点と三角形の内外判定を行う
		result = PlanePos3ToTriangleInSide< T >( plane , triangle );
	}

	return result;
}

template< typename T >
inline skyBool SphereCrossSphere( const CSphere< T > &spher1 , const CSphere< T > &spher2 )
{
	T distance = ( spher1.GetPos() - spher2.GetPos() ).Length();

	return ( distance <= spher1.R() + spher2.R() ) ? skyTrue : skyFalse;
}

template< typename T >
inline skyBool SphereCrossTriangle3( const CSphere< T > &sphere , const CTriangle3< T > &triangle )
{
	const CVector3< T >& center = sphere.GetPos();
	T radius = sphere.GetRadius();
	const CVector3< T >& vertex0 = triangle.m_Pos[ 0 ];
	const CVector3< T >& vertex1 = triangle.m_Pos[ 1 ];
	const CVector3< T >& vertex2 = triangle.m_Pos[ 2 ];

	// 面法線を求める
	CVector3< T > edge0 = vertex1 - vertex0;
	CVector3< T > edge1 = vertex2 - vertex1;
	CVector3< T > normal = edge0.Cross( edge1 ).Normalize();

	// 平面と球の距離を求める
	T constant = -normal.Dot( vertex0 );
	T distance = normal.Dot( center ) + constant;

	// 球の中心が面に接しているか、負の方向にある場合は当たらない。
	if( distance <= 0.f )
	{ 
		return skyFalse;
	}
	// 半径以上はなれていれば当たらない
	if( distance > radius )
	{ 
		return skyFalse;
	}

	// 球が三角内部に存在するか
	CVector3< T > edge2 = vertex0 - vertex2;
	skyBool outside = skyFalse;
	CVector3< T > distance0 = center - vertex0;
	outside |= ( normal.Dot( edge0.Cross( distance0 ) ) < 0.f);
	CVector3< T > distance1 = center - vertex1;
	outside |= ( normal.Dot( edge1.Cross( distance1 ) ) < 0.f);
	CVector3< T > distance2 = center - vertex2;
	outside |= (normal.Dot( edge2.Cross( distance2 ) ) < 0.f);
	// 三角の内側なので交差する
	if( !outside )
	{
		// 交差位置は球中心から面法線の逆方向にdistance伸ばしたところ
		return skyTrue;
	}

	// セグメント0との比較
	T squaredRadius = radius * radius;
	T t0 = distance0.Dot( edge0 );
	if( t0 > 0.f )
	{
		T edgeSquaredLength = edge0.QuaredLength();
		if( t0 >= edgeSquaredLength )
		{
			// t0 = 1.f; // 接触点を求める場合は設定する
			distance0 -= edge0;
		}
		else
		{
			t0 /= edgeSquaredLength;
			distance0 -= edge0 * t0;
		}
	}
	// セグメントとの距離が半径以下
	if( distance0.QuaredLength() <= squaredRadius )
	{
		// 交差位置はvertex0 + t0 * edge0
		return skyTrue;
	}

	// セグメント1との比較
	T t1 = distance1.Dot( edge1 );
	if( t1 > 0.f )
	{
		T edgeSquaredLength = edge1.QuaredLength();
		if( t1 >= edgeSquaredLength )
		{
			// t1 = 1.f; // 接触点を求める場合は設定する
			distance1 -= edge1;
		}
		else
		{
			t1 /= edgeSquaredLength;
			distance1 -= edge1 * t1;
		}
	}
	// セグメントとの距離が半径以下
	if( distance1.QuaredLength() <= squaredRadius)
	{
		// 交差位置はvertex1 + t1 * edge1
		return skyTrue;
	}

	// セグメント2との比較
	T t2 = distance2.Dot( edge2 );
	if(t2 > 0.f)
	{
		T edgeSquaredLength = edge2.QuaredLength();
		if( t2 >= edgeSquaredLength )
		{
			// t2 = 1.f; // 接触点を求める場合は設定する
			distance2 -= edge2;
		}
		else
		{
			t2 /= edgeSquaredLength;
			distance2 -= edge2 * t2;
		}
	}
	// セグメントとの距離が半径以下
	if( distance2.QuaredLength() <= squaredRadius )
	{
		// 交差位置はvertex2 + t2 * edge2
		return skyTrue;
	}

	return skyFalse;
}

template< typename T >
inline skyBool SphereCrossPlane( const CSphere< T > &sphere , const CPlane< T > &plane )
{
	T distance = GeometryDistanceUtil::Pos3ToPlane( sphere.GetPos() , plane );
	return ( CMathUtil::Abs( distance ) <= sphere.GetRadius() );
}

template< typename T >
inline skyBool SphereCrossLine3( const CSphere< T > &sphere , const CLine3< T > &line )
{
	return ( GeometryDistanceUtil::SqDistPointSegment( line.m_Pos1 , line.m_Pos2 , sphere.GetPos() ) <= sphere.GetRadius() * sphere.GetRadius() );
}

template< typename T >
inline skyBool SphereCrossRay3( const CSphere< T > &sphere , const CRay3< T > &ray , CollisionResultInfo *info )
{
	CVector3< T > a_vQ = ray.m_Pos - sphere.GetPos();
	const CVector3< T >& a_vV = ray.m_Vec;  // 正規済みとする(よって a_a = 1.0f としてよい)

	T a_fQVdot = a_vV.Dot( a_vQ );
    T a_b = a_fQVdot;
	T a_c = a_vQ.Dot( a_vQ ) - ( sphere.GetRadius() * sphere.GetRadius() );

    // f32 a_fD = a_b * a_b - a_a * a_c; // 判定式
    T a_fD = a_b * a_b - a_c; // 判定式
    if( a_fD < 0.0f )
	{
        return skyFalse;
    }

    // f32 a_fT = (a_b * -1.0f - math::Sqrt( a_fD )) / a_a;
	T a_fT = ( a_b * -1.0f - CMathUtil::Sqrt( a_fD ) );

    if( a_fT < 0.0f )
	{
        return skyFalse;
    }

	if ( info != skyNull )
	{
		info->m_Distance = a_fT;
		info->m_CrossPos = ray.m_Pos + ( ray.m_Vec * info->m_Distance );
		info->m_CrossDir = ( info->m_CrossPos - sphere.GetPos() ) / sphere.GetRadius();
	}

    return skyTrue;
}

template< typename T >
inline skyBool SphereCrossCapsule3( const CSphere< T > &sphere , const Capsule3< T > &capsule )
{
	//球の中心とカプセルの線分の間の（平方した）距離を返す
	T dist2 = GeometryDistanceUtil::SqDistPointSegment( capsule.m_Pos1 , capsule.m_Pos2 , sphere.GetPos() );

	//（平方した）距離が（平方した）半径の総和よりも小さい場合は衝突している
	T radius = sphere.GetRadius() + capsule.m_Radius;
	return dist2 <= radius * radius;
}

template< typename T >
inline skyBool Capsule3CrossCapsule3( const Capsule3< T > &capsule1 , const Capsule3< T > &capsule2 )
{
	//カプセルの内部の構造の間の（平方した）距離を計算
	T s , t;
	CVector3< T > c1 , c2;
	T dist2 = GeometryDistanceUtil::ClosestPtSegmentSegment( capsule1.m_Pos1 , capsule1.m_Pos2 , capsule2.m_Pos1 , capsule2.m_Pos2 , s , t , c1 , c2 );

	//(平方した）距離が（平方した）半径の総和よりも小さい場合は衝突
	T radius = capsule1.m_Radius + capsule2.m_Radius;
	return dist2 <= radius * radius;
}

inline skyBool AABB2CrossPoint2( const CAABB2 &aabb2 , const CBasicVector2 &p )
{
	// それぞれの軸が重なっていないかをチェックする

	return
		( p.X >= aabb2.m_Min.X ) && ( p.X <= aabb2.m_Max.X ) &&
		( p.Y >= aabb2.m_Min.Y ) && ( p.Y <= aabb2.m_Max.Y );
}

inline skyBool AABB2CrossLine2( const CAABB2 &aabb2 , const CBasicLine2 &line , CollisionResultInfo *info )
{
	// 交差しない場合はこの巨大な数を返す

	CBasicVector2 rayOrg   = line.m_Pos1;
	CBasicVector2 rayDelta = line.m_Pos2 - line.m_Pos1;

	// 点がボックスの中にあるかどうかをチェック（トリビアルリジェクト）し、
	// 各前面からのパラメーター距離を決定する

	skyBool inside = skyTrue;

	dectype xt = 0.0f;
	dectype xn = 0.0f;
	if ( rayOrg.X < aabb2.m_Min.X )
	{
		xt = aabb2.m_Min.X - rayOrg.X;
		if ( xt > rayDelta.X ) return skyFalse;
		xt /= rayDelta.X;
		inside = skyFalse;
		xn = -1.0f;
	}
	else if ( rayOrg.X > aabb2.m_Max.X )
	{
		xt = aabb2.m_Max.X - rayOrg.X;
		if (xt < rayDelta.X) return skyFalse;
		xt /= rayDelta.X;
		inside = skyFalse;
		xn = 1.0f;
	}
	else
	{
		xt = -1.0f;
	}

	dectype yt = 0.0f; 
	dectype yn = 0.0f;
	if ( rayOrg.Y < aabb2.m_Min.Y )
	{
		yt = aabb2.m_Min.Y - rayOrg.Y;
		if ( yt > rayDelta.Y ) return skyFalse;
		yt /= rayDelta.Y;
		inside = skyFalse;
		yn = -1.0f;
	}
	else if ( rayOrg.Y > aabb2.m_Max.Y )
	{
		yt = aabb2.m_Max.Y - rayOrg.Y;
		if ( yt < rayDelta.Y ) return skyFalse;
		yt /= rayDelta.Y;
		inside = skyFalse;
		yn = 1.0f;
	}
	else
	{
		yt = -1.0f;
	}

	// ボックスの中か？

	if ( inside )
	{
		if ( info != skyNull )
		{
			info->m_CrossDir = -rayDelta;
			info->m_CrossDir.Normalize();
			info->m_CrossPos = line.m_Pos1;
		}
		return skyTrue;
	}

	// 最も遠い平面を選択する（これが交差の平面である）

	s32 which = 0;
	dectype t = xt;
	if ( yt > t )
	{
		which = 1;
		t = yt;
	}

	switch ( which )
	{

		case 0: // yz面との交差
		{
			dectype y = rayOrg.Y + rayDelta.Y*t;
			if ( y < aabb2.m_Min.Y || y > aabb2.m_Max.Y ) return skyFalse;

			if ( info != skyNull )
			{
				info->m_CrossDir.X = xn;
				info->m_CrossDir.Y = 0.0f;
			}

		} break;

		case 1: // xz面との交差
		{
			dectype x = rayOrg.X + rayDelta.X*t;
			if ( x < aabb2.m_Min.X || x > aabb2.m_Max.X ) return skyFalse;

			if ( info != skyNull )
			{
				info->m_CrossDir.X = 0.0f;
				info->m_CrossDir.Y = yn;
			}

		} break;

		case 2: // xy面との交差
		{
			dectype x = rayOrg.X + rayDelta.X * t;
			if ( x < aabb2.m_Min.X || x > aabb2.m_Max.X ) return skyFalse;
			dectype y = rayOrg.Y + rayDelta.Y * t;
			if ( y < aabb2.m_Min.Y || y > aabb2.m_Max.Y ) return skyFalse;

			if ( info != skyNull )
			{
				info->m_CrossDir.X = 0.0f;
				info->m_CrossDir.Y = 0.0f;
			}

		} break;
	}

	// 交差のパラメトリックな点を返す
	if ( info != skyNull )
	{
		info->m_CrossPos = line.Interpolate( ( f32 )t );
	}

	return skyTrue;
}

inline skyBool AABB2CrossAABB2( const CAABB2 &aabb1 ,  const CAABB2 &aabb2 , CAABB2 *boxIntersect )
{
	// 重なっていないかをチェックする

	if ( aabb1.m_Min.X > aabb2.m_Max.X ) return skyFalse;
	if ( aabb1.m_Max.X < aabb2.m_Min.X ) return skyFalse;
	if ( aabb1.m_Min.Y > aabb2.m_Max.Y ) return skyFalse;
	if ( aabb1.m_Max.Y < aabb2.m_Min.Y ) return skyFalse;

	// 重なっている（必要な場合には交差のＡＡＢＢを計算する）

	if ( boxIntersect != skyNull )
	{
		boxIntersect->m_Min.X = TemplateUtil::Max( aabb1.m_Min.X , aabb2.m_Min.X );
		boxIntersect->m_Max.X = TemplateUtil::Min( aabb1.m_Max.X , aabb2.m_Max.X );
		boxIntersect->m_Min.Y = TemplateUtil::Max( aabb1.m_Min.Y , aabb2.m_Min.Y );
		boxIntersect->m_Max.Y = TemplateUtil::Min( aabb1.m_Max.Y , aabb2.m_Max.Y );
	}

	return skyTrue;
}

inline skyBool AABB3CrossPoint3( const CAABB3 &aabb3 , const CBasicVector3 &p )
{
	// それぞれの軸が重なっていないかをチェックする

	return
		( p.X >= aabb3.m_Min.X ) && ( p.X <= aabb3.m_Max.X ) &&
		( p.Y >= aabb3.m_Min.Y ) && ( p.Y <= aabb3.m_Max.Y ) &&
		( p.Z >= aabb3.m_Min.Z ) && ( p.Z <= aabb3.m_Max.Z );
}

inline skyBool AABB3CrossSphere( const CAABB3 &aabb3 , const CBasicSphere &sphere )
{
	//その点に最も近い、ボックス上の点を求める

	CBasicVector3	closestPoint = aabb3.ClosestPointTo( sphere.GetPos() );

	//それが範囲内にあるかをチェックする

	return DistanceSquared( sphere.GetPos() , closestPoint ) < sphere.GetRadius() * sphere.GetRadius();
}

inline skyBool AABB3CrossLine3( const CAABB3 &aabb3 , const CBasicLine3 &line , CollisionResultInfo *info )
{
	// 交差しない場合はこの巨大な数を返す

	CBasicVector3 rayOrg   = line.m_Pos1;
	CBasicVector3 rayDelta = line.m_Pos2 - line.m_Pos1;

	// 点がボックスの中にあるかどうかをチェック（トリビアルリジェクト）し、
	// 各前面からのパラメーター距離を決定する

	skyBool inside = skyTrue;

	dectype xt = 0.0f;
	dectype xn = 0.0f;
	if ( rayOrg.X < aabb3.m_Min.X )
	{
		xt = aabb3.m_Min.X - rayOrg.X;
		if ( xt > rayDelta.X ) return skyFalse;
		xt /= rayDelta.X;
		inside = skyFalse;
		xn = -1.0f;
	}
	else if ( rayOrg.X > aabb3.m_Max.X )
	{
		xt = aabb3.m_Max.X - rayOrg.X;
		if (xt < rayDelta.X) return skyFalse;
		xt /= rayDelta.X;
		inside = skyFalse;
		xn = 1.0f;
	}
	else
	{
		xt = -1.0f;
	}

	dectype yt = 0.0f; 
	dectype yn = 0.0f;
	if ( rayOrg.Y < aabb3.m_Min.Y )
	{
		yt = aabb3.m_Min.Y - rayOrg.Y;
		if ( yt > rayDelta.Y ) return skyFalse;
		yt /= rayDelta.Y;
		inside = skyFalse;
		yn = -1.0f;
	}
	else if ( rayOrg.Y > aabb3.m_Max.Y )
	{
		yt = aabb3.m_Max.Y - rayOrg.Y;
		if ( yt < rayDelta.Y ) return skyFalse;
		yt /= rayDelta.Y;
		inside = skyFalse;
		yn = 1.0f;
	}
	else
	{
		yt = -1.0f;
	}

	dectype zt = 0.0f;
	dectype zn = 0.0f;
	if ( rayOrg.Z < aabb3.m_Min.Z )
	{
		zt = aabb3.m_Min.Z - rayOrg.Z;
		if ( zt > rayDelta.Z ) return skyFalse;
		zt /= rayDelta.Z;
		inside = skyFalse;
		zn = -1.0f;
	}
	else if ( rayOrg.Z > aabb3.m_Max.Z )
	{
		zt = aabb3.m_Max.Z - rayOrg.Z;
		if ( zt < rayDelta.Z ) return skyFalse;
		zt /= rayDelta.Z;
		inside = skyFalse;
		zn = 1.0f;
	}
	else
	{
		zt = -1.0f;
	}

	// ボックスの中か？

	if ( inside )
	{
		if ( info != skyNull )
		{
			info->m_CrossDir = -rayDelta;
			info->m_CrossDir.Normalize();
			info->m_CrossPos = line.m_Pos1;
		}
		return skyTrue;
	}

	// 最も遠い平面を選択する（これが交差の平面である）

	s32 which = 0;
	dectype t = xt;
	if ( yt > t )
	{
		which = 1;
		t = yt;
	}
	if ( zt > t )
	{
		which = 2;
		t = zt;
	}

	switch ( which )
	{

		case 0: // yz面との交差
		{
			dectype y = rayOrg.Y + rayDelta.Y*t;
			if ( y < aabb3.m_Min.Y || y > aabb3.m_Max.Y ) return skyFalse;
			dectype z = rayOrg.Z + rayDelta.Z * t;
			if ( z < aabb3.m_Min.Z || z > aabb3.m_Max.Z ) return skyFalse;

			if ( info != skyNull )
			{
				info->m_CrossDir.X = xn;
				info->m_CrossDir.Y = 0.0f;
				info->m_CrossDir.Z = 0.0f;
			}

		} break;

		case 1: // xz面との交差
		{
			dectype x = rayOrg.X + rayDelta.X*t;
			if ( x < aabb3.m_Min.X || x > aabb3.m_Max.X ) return skyFalse;
			dectype z = rayOrg.Z + rayDelta.Z * t;
			if ( z < aabb3.m_Min.Z || z > aabb3.m_Max.Z ) return skyFalse;

			if ( info != skyNull )
			{
				info->m_CrossDir.X = 0.0f;
				info->m_CrossDir.Y = yn;
				info->m_CrossDir.Z = 0.0f;
			}

		} break;

		case 2: // xy面との交差
		{
			dectype x = rayOrg.X + rayDelta.X * t;
			if ( x < aabb3.m_Min.X || x > aabb3.m_Max.X ) return skyFalse;
			dectype y = rayOrg.Y + rayDelta.Y * t;
			if ( y < aabb3.m_Min.Y || y > aabb3.m_Max.Y ) return skyFalse;

			if ( info != skyNull )
			{
				info->m_CrossDir.X = 0.0f;
				info->m_CrossDir.Y = 0.0f;
				info->m_CrossDir.Z = zn;
			}

		} break;
	}

	// 交差のパラメトリックな点を返す
	if ( info != skyNull )
	{
		info->m_CrossPos = line.Interpolate( ( f32 )t );
	}

	return skyTrue;

}

inline skyBool AABB3CrossPlane( const CAABB3 &aabb3 , const CBasicPlane &plane )
{
    // These two lines not necessary with a (center, extents) AABB representation
    CBasicVector3 c = ( aabb3.m_Max + aabb3.m_Min ) * 0.5f; // Compute AABB center
    CBasicVector3 e = aabb3.m_Max - c; // Compute positive extents

    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	dectype r = e[ 0 ] * CMathUtil::Abs( plane.m_Normal[ 0 ] ) + e[ 1 ] * CMathUtil::Abs( plane.m_Normal[ 1 ] ) + e[ 2 ] * CMathUtil::Abs( plane.m_Normal[ 2 ] );

    // Compute distance of box center from plane
	dectype s = plane.m_Normal.Dot( c ) - plane.d;

    // Intersection occurs when distance s falls within [-r,+r] interval
    return CMathUtil::Abs( s ) <= r;
}

inline skyBool AABB3CrossTriangle3( const CAABB3 &aabb3 , const CBasicTriangle3 &triangle )
{
	CBasicVector3 v0 = triangle.m_Pos[ 0 ] , v1 = triangle.m_Pos[ 1 ] , v2 = triangle.m_Pos[ 2 ];

    dectype p0, p1, p2, r;

    CBasicVector3 c = ( aabb3.m_Min + aabb3.m_Max ) * 0.5f;
    dectype e0 = ( aabb3.m_Max.X - aabb3.m_Min.X ) * 0.5f;
    dectype e1 = ( aabb3.m_Max.Y - aabb3.m_Min.Y ) * 0.5f;
    dectype e2 = ( aabb3.m_Max.Z - aabb3.m_Min.Z ) * 0.5f;

    v0 = v0 - c;
    v1 = v1 - c;
    v2 = v2 - c;

    CBasicVector3 f0 = v1 - v0, f1 = v2 - v1, f2 = v0 - v2;

    //a00
    p0 = -v0.Y * ( v1.Z - v0.Z ) + v0.Z * ( v1.Y - v0.Y );
    p1 = -v1.Y * ( v1.Z - v0.Z ) + v1.Z * ( v1.Y - v0.Y );
    p2 = -v2.Y * ( v1.Z - v0.Z ) + v2.Z * ( v1.Y - v0.Y );
	r = e1 * CMathUtil::Abs( f0.Z ) + e2 * CMathUtil::Abs( f0.Y );
	if( TemplateUtil::Max( -TemplateUtil::Max( TemplateUtil::Max( p0 , p1 ) , p2 ) , TemplateUtil::Min( TemplateUtil::Min( p0 , p1 ) , p2 ) ) > r )
    {
        return skyFalse;
    }
    //a01
    p0 = -v0.Y * ( v2.Z - v1.Z ) + v0.Z * ( v2.Y - v1.Y );
    p1 = -v1.Y * ( v2.Z - v1.Z ) + v1.Z * ( v2.Y - v1.Y );
    p2 = -v2.Y * ( v2.Z - v1.Z ) + v2.Z * ( v2.Y - v1.Y );
    r = e1 * CMathUtil::Abs( f1.Z ) + e2 * CMathUtil::Abs( f1.Y );
    if( TemplateUtil::Max( -TemplateUtil::Max( TemplateUtil::Max( p0 , p1 ) , p2 ) , TemplateUtil::Min( TemplateUtil::Min( p0 , p1 ) , p2 ) ) > r )
    {
        return skyFalse;
    }
    //a02
    p0 = -v0.Y * ( v0.Z - v2.Z ) + v0.Z * ( v0.Y - v2.Y );
    p1 = -v1.Y * ( v0.Z - v2.Z ) + v1.Z * ( v0.Y - v2.Y );
    p2 = -v2.Y * ( v0.Z - v2.Z ) + v2.Z * ( v0.Y - v2.Y );
    r = e1 * CMathUtil::Abs( f2.Z ) + e2 * CMathUtil::Abs( f2.Y );
    if( TemplateUtil::Max( -TemplateUtil::Max( TemplateUtil::Max( p0 , p1 ) , p2 ) , TemplateUtil::Min( TemplateUtil::Min( p0 , p1 ) , p2 ) ) > r )
    {
        return skyFalse;
    }
    //a10
    p0 = v0.X * ( v1.Z - v0.Z ) + -v0.Z * ( v1.X - v0.X );
    p1 = v1.X * ( v1.Z - v0.Z ) + -v1.Z * ( v1.X - v0.X );
    p2 = v2.X * ( v1.Z - v0.Z ) + -v2.Z * ( v1.X - v0.X );
    r = e0 * CMathUtil::Abs( f0.Z ) + e2 * CMathUtil::Abs( f0.X );
    if( TemplateUtil::Max( -TemplateUtil::Max( TemplateUtil::Max( p0 , p1 ) , p2 ) , TemplateUtil::Min( TemplateUtil::Min( p0 , p1 ) , p2 ) ) > r )
    {
        return skyFalse;
    }
    //a11
    p0 = v0.X * ( v2.Z - v1.Z ) + -v0.Z * ( v2.X - v1.X );
    p1 = v1.X * ( v2.Z - v1.Z ) + -v1.Z * ( v2.X - v1.X );
    p2 = v2.X * ( v2.Z - v1.Z ) + -v2.Z * ( v2.X - v1.X );
    r = e0 * CMathUtil::Abs( f1.Z ) + e2 * CMathUtil::Abs( f1.X );
    if( TemplateUtil::Max( -TemplateUtil::Max( TemplateUtil::Max( p0 , p1 ) , p2 ) , TemplateUtil::Min( TemplateUtil::Min( p0 , p1 ) , p2 ) ) > r )
    {
        return skyFalse;
    }

    //a12
    p0 = v0.X * ( v0.Z - v2.Z ) + -v0.Z * ( v0.X - v2.X );
    p1 = v1.X * ( v0.Z - v2.Z ) + -v1.Z * ( v0.X - v2.X );
    p2 = v2.X * ( v0.Z - v2.Z ) + -v2.Z * ( v0.X - v2.X );
    r = e0 * CMathUtil::Abs( f2.Z ) + e2 * CMathUtil::Abs( f2.X );
    if( TemplateUtil::Max( -TemplateUtil::Max( TemplateUtil::Max( p0 , p1 ) , p2 ) , TemplateUtil::Min( TemplateUtil::Min( p0 , p1 ) , p2 ) ) > r )
    {
        return skyFalse;
    }
    //a20
    p0 = -v0.X * ( v1.Y - v0.Y ) + v0.Y * ( v1.X - v0.X );
    p1 = -v1.X * ( v1.Y - v0.Y ) + v1.Y * ( v1.X - v0.X );
    p2 = -v2.X * ( v1.Y - v0.Y ) + v2.Y * ( v1.X - v0.X );
    r = e0 * CMathUtil::Abs( f0.Y ) + e1 * CMathUtil::Abs( f0.X );
    if( TemplateUtil::Max( -TemplateUtil::Max( TemplateUtil::Max( p0 , p1 ) , p2 ) , TemplateUtil::Min( TemplateUtil::Min( p0 , p1 ) , p2 ) ) > r )
    {
        return skyFalse;
    }
    //a21
    p0 = -v0.X * ( v2.Y - v1.Y ) + v0.Y * ( v2.X - v1.X );
    p1 = -v1.X * ( v2.Y - v1.Y ) + v1.Y * ( v2.X - v1.X );
    p2 = -v2.X * ( v2.Y - v1.Y ) + v2.Y * ( v2.X - v1.X );
    r = e0 * CMathUtil::Abs( f1.Y ) + e1 * CMathUtil::Abs( f1.X );
    if( TemplateUtil::Max( -TemplateUtil::Max( TemplateUtil::Max( p0 , p1 ) , p2 ) , TemplateUtil::Min( TemplateUtil::Min( p0 , p1 ) , p2 ) ) > r )
    {
        return skyFalse;
    }
    //a22
    p0 = -v0.X * ( v0.Y - v2.Y ) + v0.Y * ( v0.X - v2.X );
    p1 = -v1.X * ( v0.Y - v2.Y ) + v1.Y * ( v0.X - v2.X );
    p2 = -v2.X * ( v0.Y - v2.Y ) + v2.Y * ( v0.X - v2.X );
    r = e0 * CMathUtil::Abs( f2.Y ) + e1 * CMathUtil::Abs( f2.X );
    if( TemplateUtil::Max( -TemplateUtil::Max( TemplateUtil::Max( p0 , p1 ) , p2 ) , TemplateUtil::Min( TemplateUtil::Min( p0 , p1 ) , p2 ) ) > r )
    {
        return skyFalse;
    }

    if(TemplateUtil::Max( TemplateUtil::Max( v0.X, v1.X ) , v2.X ) < -e0 || TemplateUtil::Min( TemplateUtil::Min( v0.X , v1.X ) , v2.X ) > e0 )
    {
        return skyFalse;
    }
    if( TemplateUtil::Max( TemplateUtil::Max( v0.Y , v1.Y ) , v2.Y ) < -e1 || TemplateUtil::Min( TemplateUtil::Min( v0.Y , v1.Y ) , v2.Y ) > e1 )
    {
        return skyFalse;
    }
    if( TemplateUtil::Max(TemplateUtil::Max( v0.Z , v1.Z ) , v2.Z ) < -e2 || TemplateUtil::Min(TemplateUtil::Min( v0.Z , v1.Z ) , v2.Z ) > e2 )
    {
        return skyFalse;
    }

    CBasicPlane p;
	p.m_Normal = Cross( f0 , f1 );
	p.d = p.m_Normal.Dot( v0 );

	CAABB3 b2;

    b2.m_Min = aabb3.m_Min - c;
    b2.m_Max = aabb3.m_Max - c;

	return AABB3CrossPlane( b2 , p );
}

inline skyBool AABB3CrossAABB3( const CAABB3 &aabb1 , const CAABB3 &aabb2 , CAABB3 *boxsIntersect )
{

	// 重なっていないかをチェックする

	if ( aabb1.m_Min.X > aabb2.m_Max.X ) return skyFalse;
	if ( aabb1.m_Max.X < aabb2.m_Min.X ) return skyFalse;
	if ( aabb1.m_Min.Y > aabb2.m_Max.Y ) return skyFalse;
	if ( aabb1.m_Max.Y < aabb2.m_Min.Y ) return skyFalse;
	if ( aabb1.m_Min.Z > aabb2.m_Max.Z ) return skyFalse;
	if ( aabb1.m_Max.Z < aabb2.m_Min.Z ) return skyFalse;

	// 重なっている（必要な場合には交差のＡＡＢＢを計算する）

	if ( boxsIntersect != skyNull )
	{
		boxsIntersect->m_Min.X = TemplateUtil::Max( aabb1.m_Min.X , aabb2.m_Min.X );
		boxsIntersect->m_Max.X = TemplateUtil::Min( aabb1.m_Max.X , aabb2.m_Max.X );
		boxsIntersect->m_Min.Y = TemplateUtil::Max( aabb1.m_Min.Y , aabb2.m_Min.Y );
		boxsIntersect->m_Max.Y = TemplateUtil::Min( aabb1.m_Max.Y , aabb2.m_Max.Y );
		boxsIntersect->m_Min.Z = TemplateUtil::Max( aabb1.m_Min.Z , aabb2.m_Min.Z );
		boxsIntersect->m_Max.Z = TemplateUtil::Min( aabb1.m_Max.Z , aabb2.m_Max.Z );
	}

	return skyTrue;
}

inline skyBool AABB3CrossOBB3( const CAABB3 &aabb3 , const COBB3 &obb )
{
	skyBool result = skyFalse;

	//ＡＡＢＢとＯＢＢの点の交差
	for ( u32 i = 0 ; i < 8 ; i++ )
	{
		if ( AABB3CrossPoint3( aabb3 , obb.GetCorner( i ) ) )
		{
			result = skyTrue;
			break;
		}
	}

	//ＯＢＢとＡＡＢＢの点の交差
	CBasicVector3 dest;
	CBasicVector3 corner;
	for ( u32 i = 0 ; i < 8 ; i++ )
	{
		corner = aabb3.Corner( i );
		obb.ClosestPtPoint( corner , dest );
		if ( Distance( corner , dest ) < 0.0001 )
		{
			result = skyTrue;
			break;
		}
	}
	return result;
}

inline skyBool OBB2CrossLine2( const COBB2 &obb2 , const CBasicLine2 &line )
{
    const dectype EPSILON = ( dectype )1.175494e-37;

	CBasicVector2 m = ( line.m_Pos1 + line.m_Pos2 ) * 0.5f;
    CBasicVector2 d = line.m_Pos2 - m;
    m = m - obb2.m_Pos;
	m = CBasicVector2( obb2.GetAxis( eAxis_x ).Dot( m ) , obb2.GetAxis( eAxis_y ).Dot( m ) );
    d = CBasicVector2( obb2.GetAxis( eAxis_x ).Dot( d ) , obb2.GetAxis( eAxis_y ).Dot( d ) );

    dectype adx = CMathUtil::Abs( d.X );
    if( CMathUtil::Abs( m.X ) > obb2.GetRadius( eAxis_x ) + adx )
	{
		return skyFalse;
	}
    dectype ady = CMathUtil::Abs( d.Y );
    if( CMathUtil::Abs( m.Y ) > obb2.GetRadius( eAxis_y ) + ady )
	{
		return skyFalse;
	}
    adx += EPSILON; 
    ady += EPSILON; 
        
    if( CMathUtil::Abs( m.X * d.Y - m.Y * d.X ) > obb2.GetRadius( eAxis_x ) * ady + obb2.GetRadius( eAxis_y ) * adx )
	{
		return skyFalse;
	}

    return skyTrue;
}

inline skyBool OBB2CrossOBB2( const COBB2 &obb1 , const COBB2 &obb2 )
{
    const dectype EPSILON = ( dectype )1.175494e-37;

    dectype R[ 3 ][ 3 ], AbsR[ 3 ][ 3 ];
    for( s32 i = 0 ; i < 2 ; i++ )
    {
		eAxis axis_i = eAxis_x;
		switch( i )
		{
		case 0: axis_i = eAxis_x; break;
		case 1: axis_i = eAxis_y; break;
		}

        for( s32 j = 0 ; j < 2 ; j++ )
        {
			eAxis axis_j = eAxis_x;
			switch( j )
			{
			case 0: axis_j = eAxis_x; break;
			case 1: axis_j = eAxis_y; break;
			}

			R[ i ][ j ] = obb1.GetAxis( axis_i ).Dot( obb2.GetAxis( axis_j ) );
            AbsR[ i ][ j ] = CMathUtil::Abs( R[ i ][ j ] ) + EPSILON;
        }
    }
        
	CBasicVector2 t = obb2.m_Pos - obb1.m_Pos;
	t = CBasicVector2( t.Dot( obb1.GetAxis( eAxis_x ) ) , t.Dot( obb1.GetAxis( eAxis_y ) ) );
        
    //軸L=A0, L=A1, L=A2判定
    dectype ra, rb;

    for( s32 i = 0; i < 2; i++)
    {
		eAxis axis = eAxis_x;
		switch( i )
		{
		case 0: axis = eAxis_x; break;
		case 1: axis = eAxis_y; break;
		}

        ra = obb1.GetRadius( axis );
        rb = obb2.GetRadius( eAxis_x ) * AbsR[ i ][ 0 ] + obb2.GetRadius( eAxis_y ) * AbsR[ i ][ 1 ] + obb2.GetRadius( eAxis_z ) * AbsR[ i ][ 2 ];
        if( CMathUtil::Abs( t[ i ] ) > ra + rb )return skyFalse;
    }
    //軸L=B0, L=B1, L=B2判定
    for( s32 i = 0; i < 2; i++)
    {
		eAxis axis = eAxis_x;
		switch( i )
		{
		case 0: axis = eAxis_x; break;
		case 1: axis = eAxis_y; break;
		}

        ra = obb1.GetRadius( eAxis_x ) * AbsR[ 0 ][ i ] + obb1.GetRadius( eAxis_y ) * AbsR[ 1 ][ i ] + obb1.GetRadius( eAxis_z ) * AbsR[ 2 ][ i ];
        rb = obb2.GetRadius( axis );
        if( CMathUtil::Abs( t[ 0 ] * R[ 0 ][ i ] + t[ 1 ] * R[ 1 ][ i ] + t[ 2 ] * R[ 2 ][ i ]) > ra + rb )return skyFalse;
    }

    //軸L=A0 X B0判定
    ra = obb1.GetRadius( eAxis_y ) * AbsR[ 2 ][ 0 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 1 ][ 0 ];
    rb = obb2.GetRadius( eAxis_y ) * AbsR[ 0 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 0 ][ 1 ];
    if( CMathUtil::Abs( t[ 2 ] * R[ 1 ][ 0 ] - t[ 1 ] * R[ 2 ][ 0 ]) > ra + rb )return skyFalse;

    //軸L=A0 X B1判定
    ra = obb1.GetRadius( eAxis_y ) * AbsR[ 2 ][ 1 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 1 ][ 1 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 0 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 0 ][ 0 ];
    if( CMathUtil::Abs(t[ 2 ] * R[ 1 ][ 1 ] - t[ 1 ] * R[ 2 ][ 1 ]) > ra + rb )return skyFalse;

    //軸L=A0 X B2判定
    ra = obb1.GetRadius( eAxis_y ) * AbsR[ 2 ][ 2 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 1 ][ 2 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 0 ][ 1 ] + obb2.GetRadius( eAxis_y ) * AbsR[ 0 ][ 0 ];
    if( CMathUtil::Abs( t[ 2 ] * R[ 1 ][ 2 ] - t[ 1 ] * R[ 2 ][ 2 ]) > ra + rb )return skyFalse;

    //軸L=A1 X B0判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 2 ][ 0 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 0 ][ 0 ];
    rb = obb2.GetRadius( eAxis_y ) * AbsR[ 1 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 1 ][ 1 ];
    if(CMathUtil::Abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)return skyFalse;

    //軸L=A1 X B1判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 2 ][ 1 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 0 ][ 1 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 1 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 1 ][ 0 ];
    if( CMathUtil::Abs( t[ 0 ] * R[ 2 ][ 1 ] - t[ 2 ] * R[ 0 ][ 1 ] ) > ra + rb )return skyFalse;

    //軸L=A1 X B2判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 2 ][ 2 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 0 ][ 2 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 1 ][ 1 ] + obb2.GetRadius( eAxis_y ) * AbsR[ 1 ][ 0 ];
    if(CMathUtil::Abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)return skyFalse;

    //軸L=A2 X B0判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 1 ][ 0 ] + obb1.GetRadius( eAxis_y ) * AbsR[ 0 ][ 0 ];
    rb = obb2.GetRadius( eAxis_y ) * AbsR[ 2 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 2 ][ 1 ];
    if( CMathUtil::Abs( t[ 1 ] * R[ 0 ][ 0 ] - t[ 0 ] * R[ 1 ][ 0 ] ) > ra + rb )return skyFalse;

    //軸L=A2 X B1判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 1 ][ 1 ] + obb1.GetRadius( eAxis_y ) * AbsR[ 0 ][ 1 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 2 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 2 ][ 0 ];
    if( CMathUtil::Abs( t[ 1 ] * R[ 0 ][ 1 ] - t[ 0 ] * R[ 1 ][ 1 ] ) > ra + rb )return skyFalse;

    //軸L=A2 X B2判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 1 ][ 2 ] + obb1.GetRadius( eAxis_y ) * AbsR[ 0 ][ 2 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 2 ][ 1 ] + obb2.GetRadius( eAxis_y ) * AbsR[ 2 ][ 0 ];
    if( CMathUtil::Abs(t[ 1 ] * R[ 0 ][ 2 ] - t[ 0 ] * R[ 1 ][ 2 ] ) > ra + rb )return skyFalse;

    return skyTrue;

}

inline skyBool OBB3CrossSphere( const COBB3 &obb , const CBasicSphere &sphere , CollisionResultInfo *info )
{
	CBasicVector3 clossPoint;
	obb.ClosestPtPoint( sphere.GetPos() , clossPoint );
    CBasicVector3 v = clossPoint - sphere.GetPos();

	//ベクトルが自分自身と内積を取ると，そのベクトルの長さの2乗が出てくる。それを半径の２乗と比較している
	skyBool result = v.Dot( v ) <= sphere.GetRadius() * sphere.GetRadius();

	if ( result == skyTrue && info != skyNull )
	{
		info->m_CrossPos = clossPoint;
	}

	return result;
}

inline skyBool OBB3CrossLine3( const COBB3 &obb , const CBasicLine3 &line )
{
    const dectype EPSILON = ( dectype )1.175494e-37;

	CBasicVector3 m = ( line.m_Pos1 + line.m_Pos2 ) * 0.5f;
    CBasicVector3 d = line.m_Pos2 - m;
    m = m - obb.m_Pos;
	m = CBasicVector3( obb.GetAxis( eAxis_x ).Dot( m ) , obb.GetAxis( eAxis_y ).Dot( m ) , obb.GetAxis( eAxis_z ).Dot( m ) );
    d = CBasicVector3( obb.GetAxis( eAxis_x ).Dot( d ) , obb.GetAxis( eAxis_y ).Dot( d ) , obb.GetAxis( eAxis_z ).Dot( d ) );

    dectype adx = CMathUtil::Abs( d.X );
    if( CMathUtil::Abs( m.X ) > obb.GetRadius( eAxis_x ) + adx )
	{
		return skyFalse;
	}
    dectype ady = CMathUtil::Abs( d.Y );
    if( CMathUtil::Abs( m.Y ) > obb.GetRadius( eAxis_y ) + ady )
	{
		return skyFalse;
	}
    dectype adz = CMathUtil::Abs( d.Z );
    if( CMathUtil::Abs( m.Z ) > obb.GetRadius( eAxis_z ) + adz )
	{
		return skyFalse;
	}
    adx += EPSILON; 
    ady += EPSILON; 
    adz += EPSILON;
        
    if( CMathUtil::Abs( m.Y * d.Z - m.Z * d.Y ) > obb.GetRadius( eAxis_y ) * adz + obb.GetRadius( eAxis_z ) * ady )
	{
		return skyFalse;
	}
    if( CMathUtil::Abs( m.Z * d.X - m.X * d.Z ) > obb.GetRadius( eAxis_x ) * adz + obb.GetRadius( eAxis_z ) * adx )
	{
		return skyFalse;
	}
    if( CMathUtil::Abs( m.X * d.Y - m.Y * d.X ) > obb.GetRadius( eAxis_x ) * ady + obb.GetRadius( eAxis_y ) * adx )
	{
		return skyFalse;
	}

    return skyTrue;
}

inline skyBool OBB3CrossPlane( const COBB3 &obb , const CBasicPlane &plane )
{
    // bの半径をL(t) = b.c + t * p.n上に対して射影してできる間隔を計算
	dectype r = obb.GetRadius( eAxis_x ) * CMathUtil::Abs( plane.m_Normal.Dot( obb.GetAxis( eAxis_x ) ) ) +
              obb.GetRadius( eAxis_y ) * CMathUtil::Abs( plane.m_Normal.Dot( obb.GetAxis( eAxis_y ) ) ) +
              obb.GetRadius( eAxis_z ) * CMathUtil::Abs( plane.m_Normal.Dot( obb.GetAxis( eAxis_z ) ) );

    // 平面から箱の中心までの距離を計算
	dectype s = plane.m_Normal.Dot( obb.m_Pos ) - plane.d;

    // 距離sが間隔[-r,+r]の範囲内に収まる場合は交差が起こる
    return CMathUtil::Abs( s ) <= r;
}

inline skyBool OBB3CrossOBB3( const COBB3 &obb1 , const COBB3 &obb2 )
{
    const dectype EPSILON = ( dectype )1.175494e-37;

    dectype R[ 3 ][ 3 ], AbsR[ 3 ][ 3 ];
    for( s32 i = 0 ; i < 3 ; i++ )
    {
		eAxis axis_i = eAxis_x;
		switch( i )
		{
		case 0: axis_i = eAxis_x; break;
		case 1: axis_i = eAxis_y; break;
		case 2: axis_i = eAxis_z; break;
		}

        for( s32 j = 0 ; j < 3 ; j++ )
        {
			eAxis axis_j = eAxis_x;
			switch( j )
			{
			case 0: axis_j = eAxis_x; break;
			case 1: axis_j = eAxis_y; break;
			case 2: axis_j = eAxis_z; break;
			}

			R[ i ][ j ] = obb1.GetAxis( axis_i ).Dot( obb2.GetAxis( axis_j ) );
            AbsR[ i ][ j ] = CMathUtil::Abs( R[ i ][ j ] ) + EPSILON;
        }
    }
        
	CBasicVector3 t = obb2.m_Pos - obb1.m_Pos;
	t = CBasicVector3( t.Dot( obb1.GetAxis( eAxis_x ) ) , t.Dot( obb1.GetAxis( eAxis_y ) ) , t.Dot( obb1.GetAxis( eAxis_z ) ) );
        
    //軸L=A0, L=A1, L=A2判定
    dectype ra, rb;

    for( s32 i = 0; i < 3; i++)
    {
		eAxis axis = eAxis_x;
		switch( i )
		{
		case 0: axis = eAxis_x; break;
		case 1: axis = eAxis_y; break;
		case 2: axis = eAxis_z; break;
		}

        ra = obb1.GetRadius( axis );
        rb = obb2.GetRadius( eAxis_x ) * AbsR[ i ][ 0 ] + obb2.GetRadius( eAxis_y ) * AbsR[ i ][ 1 ] + obb2.GetRadius( eAxis_z ) * AbsR[ i ][ 2 ];
        if( CMathUtil::Abs( t[ i ] ) > ra + rb )return skyFalse;
    }
    //軸L=B0, L=B1, L=B2判定
    for( s32 i = 0; i < 3; i++)
    {
		eAxis axis;
		switch( i )
		{
		case 0: axis = eAxis_x; break;
		case 1: axis = eAxis_y; break;
		case 2: axis = eAxis_z; break;
		}

        ra = obb1.GetRadius( eAxis_x ) * AbsR[ 0 ][ i ] + obb1.GetRadius( eAxis_y ) * AbsR[ 1 ][ i ] + obb1.GetRadius( eAxis_z ) * AbsR[ 2 ][ i ];
        rb = obb2.GetRadius( axis );
        if( CMathUtil::Abs( t[ 0 ] * R[ 0 ][ i ] + t[ 1 ] * R[ 1 ][ i ] + t[ 2 ] * R[ 2 ][ i ]) > ra + rb )return skyFalse;
    }

    //軸L=A0 X B0判定
    ra = obb1.GetRadius( eAxis_y ) * AbsR[ 2 ][ 0 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 1 ][ 0 ];
    rb = obb2.GetRadius( eAxis_y ) * AbsR[ 0 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 0 ][ 1 ];
    if( CMathUtil::Abs( t[ 2 ] * R[ 1 ][ 0 ] - t[ 1 ] * R[ 2 ][ 0 ]) > ra + rb )return skyFalse;

    //軸L=A0 X B1判定
    ra = obb1.GetRadius( eAxis_y ) * AbsR[ 2 ][ 1 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 1 ][ 1 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 0 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 0 ][ 0 ];
    if( CMathUtil::Abs(t[ 2 ] * R[ 1 ][ 1 ] - t[ 1 ] * R[ 2 ][ 1 ]) > ra + rb )return skyFalse;

    //軸L=A0 X B2判定
    ra = obb1.GetRadius( eAxis_y ) * AbsR[ 2 ][ 2 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 1 ][ 2 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 0 ][ 1 ] + obb2.GetRadius( eAxis_y ) * AbsR[ 0 ][ 0 ];
    if( CMathUtil::Abs( t[ 2 ] * R[ 1 ][ 2 ] - t[ 1 ] * R[ 2 ][ 2 ]) > ra + rb )return skyFalse;

    //軸L=A1 X B0判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 2 ][ 0 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 0 ][ 0 ];
    rb = obb2.GetRadius( eAxis_y ) * AbsR[ 1 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 1 ][ 1 ];
    if(CMathUtil::Abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)return skyFalse;

    //軸L=A1 X B1判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 2 ][ 1 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 0 ][ 1 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 1 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 1 ][ 0 ];
    if( CMathUtil::Abs( t[ 0 ] * R[ 2 ][ 1 ] - t[ 2 ] * R[ 0 ][ 1 ] ) > ra + rb )return skyFalse;

    //軸L=A1 X B2判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 2 ][ 2 ] + obb1.GetRadius( eAxis_z ) * AbsR[ 0 ][ 2 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 1 ][ 1 ] + obb2.GetRadius( eAxis_y ) * AbsR[ 1 ][ 0 ];
    if(CMathUtil::Abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)return skyFalse;

    //軸L=A2 X B0判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 1 ][ 0 ] + obb1.GetRadius( eAxis_y ) * AbsR[ 0 ][ 0 ];
    rb = obb2.GetRadius( eAxis_y ) * AbsR[ 2 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 2 ][ 1 ];
    if( CMathUtil::Abs( t[ 1 ] * R[ 0 ][ 0 ] - t[ 0 ] * R[ 1 ][ 0 ] ) > ra + rb )return skyFalse;

    //軸L=A2 X B1判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 1 ][ 1 ] + obb1.GetRadius( eAxis_y ) * AbsR[ 0 ][ 1 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 2 ][ 2 ] + obb2.GetRadius( eAxis_z ) * AbsR[ 2 ][ 0 ];
    if( CMathUtil::Abs( t[ 1 ] * R[ 0 ][ 1 ] - t[ 0 ] * R[ 1 ][ 1 ] ) > ra + rb )return skyFalse;

    //軸L=A2 X B2判定
    ra = obb1.GetRadius( eAxis_x ) * AbsR[ 1 ][ 2 ] + obb1.GetRadius( eAxis_y ) * AbsR[ 0 ][ 2 ];
    rb = obb2.GetRadius( eAxis_x ) * AbsR[ 2 ][ 1 ] + obb2.GetRadius( eAxis_y ) * AbsR[ 2 ][ 0 ];
    if( CMathUtil::Abs(t[ 1 ] * R[ 0 ][ 2 ] - t[ 0 ] * R[ 1 ][ 2 ] ) > ra + rb )return skyFalse;

    return skyTrue;
}

}

} } }
