#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

inline void SetClearColor( const CColor &color )
{
	glClearAccum( color.r , color.g , color.b , color.a );
}

inline void Clear()
{
	glClear( GL_ACCUM_BUFFER_BIT );
}

inline void TransmitFromColorBuffer( f32 rate )
{
	glAccum( GL_LOAD , rate );
}

inline void AddFromColorBuffer( f32 rate )
{
	glAccum( GL_ACCUM, rate );
}

inline void Add( f32 rate )
{
	glAccum( GL_ADD , rate );
}

inline void Mul( f32 rate )
{
	glAccum( GL_MULT , rate );
}

inline void TransmitToColorBuffer( f32 rate )
{
	glAccum( GL_RETURN , rate );
}

} } }

#endif