#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

#include "SkyGraphicLib/DataBase.h"

#ifdef SW_SKYLIB_DEBUG_ON

namespace sky { namespace lib { namespace graphic {

typedef u32	BinSize;

struct BinOffset
{
	BinSize	offset;

	// コンストラクタ
	BinOffset()
		: offset(0)
	{}

	// バイナリ取得
	inline void* GetBinary()
	{
		if( offset == 0 ) { return NULL; }
		return reinterpret_cast<void*>( reinterpret_cast<BinSize>(this) + offset );
	}
	inline const void* GetBinary() const
	{
		if( offset == 0 ) { return NULL; }
		return reinterpret_cast<const void*>( reinterpret_cast<BinSize>(this) + offset );
	}
	inline void* GetBinary( BinSize ofs )
	{
		if( offset == 0 ) { return NULL; }
		return reinterpret_cast<void*>( reinterpret_cast<BinSize>(this) + offset + ofs );
	}
	inline const void* GetBinary( BinSize ofs ) const
	{
		if( offset == 0 ) { return NULL; }
		return reinterpret_cast<const void*>( reinterpret_cast<BinSize>(this) + offset + ofs );
	}
	inline       void* GetBinaryBySize()       { return GetBinary( sizeof(BinSize) ); }
	inline const void* GetBinaryBySize() const { return GetBinary( sizeof(BinSize) ); }
	inline       void* GetBinaryByCount()       { return GetBinary( sizeof(int) ); }
	inline const void* GetBinaryByCount() const { return GetBinary( sizeof(int) ); }

	// サイズ取得
	inline BinSize GetSize() const
	{
		if( offset == 0 ) { return 0; }
		return *reinterpret_cast<BinSize*>( reinterpret_cast<BinSize>(this) + offset );
	}
	// 個数取得
	inline int GetCount() const
	{
		if( offset == 0 ) { return 0; }
		return *reinterpret_cast<int*>( reinterpret_cast<BinSize>(this) + offset );
	}
};

/*

inline void SetBinOffset( BinOffset* pOffset, void* pointer )
{
	MSL_ASSERT( pOffset < pointer );
	pOffset->offset = reinterpret_cast<BinSize>(pointer) - reinterpret_cast<BinSize>(pOffset);
}

template <typename T>
class ResourceBase
{
public:
	enum Flag
	{
		FLAG_LOADED_FILE		= 1 << 0,				//!< バイナリファイルはこのクラスが読み込んだ
	};

public:
	ResourceBase()
		: m_pAllocator( NULL )
		, m_pBinary( NULL ), m_BinarySize( 0 )
		, m_Flag( 0 )
	{}
	ResourceBase( void* pBin, size_t size )
		: m_pAllocator( NULL )
		, m_pBinary( pBin ), m_BinarySize( size )
		, m_Flag( 0 )
	{
		if( pBin )
		{
			MSL_ASSERT( 0 < size );
		}
	}
	ResourceBase( msl::HeapAllocator& allocator, const char* filename )
	{
		bool	isLoadSuccess = detail::LoadFile( allocator, filename, &m_pBinary, &m_BinarySize );
		m_pAllocator = &allocator;
		m_Flag       = FLAG_LOADED_FILE;
		MSL_ASSERT( isLoadSuccess );
	}
	ResourceBase( const ResourceBase& res )
		: m_pAllocator( NULL )
		, m_pBinary( res.m_pBinary ), m_BinarySize( res.m_BinarySize )
		, m_Flag( 0 )
	{
		// ポインタとサイズだけコピー
		// Note: 元のリソースが解放された場合にアクセス不能になるので注意
	}
	virtual ~ResourceBase()
	{
		Clear();
	}

	ResourceBase& operator=( const ResourceBase& res )
	{
		// ポインタとサイズだけコピー
		// Note: 元のリソースが解放された場合にアクセス不能になるので注意
		m_pAllocator = NULL;
		m_pBinary    = res.m_pBinary;
		m_BinarySize = res.m_BinarySize;
		m_Flag       = 0;
		return *this;
	}

	void Clear()
	{
		if( util::ChkFlag<msl::u32>(m_Flag, FLAG_LOADED_FILE) )
		{	// このクラスで読み込まれているので、メモリを解放する
			m_pAllocator->Free( m_pBinary );
		}
		m_pAllocator = NULL;
		m_pBinary    = NULL;
		m_BinarySize = 0;
		m_Flag       = 0;
	}

	void Swap( ResourceBase& res )
	{
		util::Swap( m_pAllocator, res.m_pAllocator );
		util::Swap( m_pBinary, res.m_pBinary );
		util::Swap( m_BinarySize, res.m_BinarySize );
		util::Swap( m_Flag, res.m_Flag );
	}

	bool IsValid() const { return (m_pBinary != NULL); }

	// Getter
	T* GetData()					{ return reinterpret_cast<T*>(m_pBinary); }
	const T* GetData()		const	{ return reinterpret_cast<const T*>(m_pBinary); }
	const void* GetBinary()	const	{ return m_pBinary; }
	size_t GetBinarySize()	const	{ return m_BinarySize; }

protected:
	HeapAllocator*	m_pAllocator;
	void*			m_pBinary;
	size_t			m_BinarySize;
	msl::u32		m_Flag;
};

//-------------------------------------------------------------------------------------------------
/*
enum SimpleVertexAttribute
{
	SIMPLE_VATTR_POSITION,
	SIMPLE_VATTR_BONE_WEIGHT,
	SIMPLE_VATTR_BONE_INDEX,
	SIMPLE_VATTR_NORMAL,
	SIMPLE_VATTR_TANGENT,
	SIMPLE_VATTR_BINORMAL,
	SIMPLE_VATTR_COLOR,
	SIMPLE_VATTR_UV,

	SIMPLE_VATTR_MAX
};

struct _ResourceSimpleGeometory
{
	msl::u32		indexCount;				// インデックス数
	msl::BinOffset	indices;				// インデックス
};

struct _ResourceSimpleShape
{
	msl::BinOffset	attributes;				// 頂点属性
	msl::u32		vertexSize;				// 頂点1つのサイズ
	msl::u32		vertexCount;			// 頂点数
	msl::BinOffset	vertices;				// 頂点データ
	msl::BinOffset	geometories;			// ジオメトリデータ
};

struct _ResourceSimpleMesh
{
	int		material;						// マテリアル番号
	int		shape;							// シェイプ番号
	int		geometory;						// ジオメトリ番号
};

struct _ResourceSimpleMaterial
{
	msl::math::Vec4f	ambient;			// アンビエントカラー
	msl::math::Vec4f	diffuse;			// ディフューズカラー
	msl::math::Vec4f	specular;			// スペキュラカラー
};

struct _ResourceSimpleBone
{
	msl::tchar			name[32];			// 名前
	msl::math::Mtx34f	mtxInvInit;			// 初期行列の逆行列
	msl::math::Quatf	rotation;			// 回転
	msl::math::Vec3f	translate;			// 平行移動
	msl::math::Vec3f	scaling;			// スケーリング
	int					parentNo;			// 親ボーンの番号
};

struct _ResourceSimpleModel
{
	msl::BinOffset	materials;
	msl::BinOffset	shapes;
	msl::BinOffset	meshes;
	msl::BinOffset	bones;
};


//-------------------------------------------------------------------------
class ResourceSimpleGeometory
	: public msl::ResourceBase<_ResourceSimpleGeometory>
{
public:
	ResourceSimpleGeometory( void* pBin )
		: ResourceBase( pBin, sizeof(_ResourceSimpleGeometory) )
	{}
	ResourceSimpleGeometory( const ResourceSimpleGeometory& res )
		: ResourceBase( res )
	{}

	msl::u32 GetIndexCount() const { return GetData()->indexCount; }
	const void* GetIndices() const { return GetData()->indices.GetBinary(); }
};	// ResourceSimpleGeometory

class ResourceSimpleShape
	: public msl::ResourceBase<_ResourceSimpleShape>
{
public:
	ResourceSimpleShape( void* pBin )
		: ResourceBase( pBin, sizeof(_ResourceSimpleShape) )
	{}
	ResourceSimpleShape( const ResourceSimpleShape& res )
		: ResourceBase( res )
	{}

	// 頂点属性
	int GetNumAttributes() const { return GetData()->attributes.GetCount(); }
	const SimpleVertexAttribute* GetAttributes() const { return reinterpret_cast<const SimpleVertexAttribute*>(GetData()->attributes.GetBinaryByCount()); }

	// 頂点データ
	msl::u32 GetVertexSize() const { return GetData()->vertexSize; }
	msl::u32 GetVertexCount() const { return GetData()->vertexCount; }
	const void* GetVertices() const { return GetData()->vertices.GetBinary(); }

	// ジオメトリ
	int GetNumGeometories() const { return GetData()->geometories.GetCount(); }
	ResourceSimpleGeometory GetGeometory( int no )
	{
		MSL_ASSERT( (0 <= no) && (no < GetNumGeometories()) );
		_ResourceSimpleGeometory*	pData = reinterpret_cast<_ResourceSimpleGeometory*>( GetData()->geometories.GetBinaryByCount() );
		return ResourceSimpleGeometory( pData + no );
	}
};	// ResourceSimpleShape

class ResourceSimpleMesh
	: public msl::ResourceBase<_ResourceSimpleMesh>
{
public:
	ResourceSimpleMesh( void* pBin )
		: ResourceBase( pBin, sizeof(_ResourceSimpleMesh) )
	{}
	ResourceSimpleMesh( const ResourceSimpleMesh& res )
		: ResourceBase( res )
	{}

	int GetMaterial()  const { return GetData()->material; }
	int GetShape()     const { return GetData()->shape; }
	int GetGeometory() const { return GetData()->geometory; }
};	// ResourceSimpleMesh

class ResourceSimpleMaterial
	: public msl::ResourceBase<_ResourceSimpleMaterial>
{
public:
	ResourceSimpleMaterial( void* pBin )
		: ResourceBase( pBin, sizeof(_ResourceSimpleMaterial) )
	{}
	ResourceSimpleMaterial( const ResourceSimpleMaterial& res )
		: ResourceBase( res )
	{}

	const msl::math::Vec4f& GetAmbient()  const { return GetData()->ambient; }
	const msl::math::Vec4f& GetDiffuse()  const { return GetData()->diffuse; }
	const msl::math::Vec4f& GetSpecular() const { return GetData()->specular; }
};	// ResourceSimpleMaterial

class ResourceSimpleBone
	: public msl::ResourceBase<_ResourceSimpleBone>
{
public:
	ResourceSimpleBone( void* pBin )
		: ResourceBase( pBin, sizeof(_ResourceSimpleBone) )
	{}
	ResourceSimpleBone( const ResourceSimpleBone& res )
		: ResourceBase( res )
	{}

	const msl::tchar* GetName()              const { return GetData()->name; }
	const msl::math::Mtx34f& GetMtxInvInit() const { return GetData()->mtxInvInit; }
	const msl::math::Quatf& GetRotation()    const { return GetData()->rotation; }
	const msl::math::Vec3f& GetTranslate()   const { return GetData()->translate; }
	const msl::math::Vec3f&	GetScaling()     const { return GetData()->scaling; }
	int GetParentNo()                        const { return GetData()->parentNo; }
};	// ResourceSimpleBone

class ResourceSimpleModel
	: public msl::ResourceBase<_ResourceSimpleModel>
{
public:
	ResourceSimpleModel()
		: ResourceBase()
	{}
	ResourceSimpleModel( void* pBin, size_t size )
		: ResourceBase( pBin, size )
	{}
	ResourceSimpleModel( msl::HeapAllocator& allocator, const char* filename )
		: ResourceBase( allocator, filename )
	{}
	ResourceSimpleModel( const ResourceSimpleModel& res )
		: ResourceBase( res )
	{}

	// マテリアル取得
	int GetNumMaterials() const { return GetData()->materials.GetCount(); }
	ResourceSimpleMaterial GetMaterial( int no )
	{
		MSL_ASSERT( (0 <= no) && (no < GetNumMaterials()) );
		_ResourceSimpleMaterial*	pData = reinterpret_cast<_ResourceSimpleMaterial*>(GetData()->materials.GetBinaryByCount());
		return ResourceSimpleMaterial( pData + no );
	}

	// シェイプ取得
	int GetNumShapes() const { return GetData()->shapes.GetCount(); }
	ResourceSimpleShape GetShape( int no )
	{
		MSL_ASSERT( (0 <= no) && (no < GetNumShapes()) );
		_ResourceSimpleShape*	pData = reinterpret_cast<_ResourceSimpleShape*>(GetData()->shapes.GetBinaryByCount());
		return ResourceSimpleShape( pData + no );
	}

	// メッシュ取得
	int GetNumMeshes() const { return GetData()->meshes.GetCount(); }
	ResourceSimpleMesh GetMesh( int no )
	{
		MSL_ASSERT( (0 <= no) && (no < GetNumMeshes()) );
		_ResourceSimpleMesh*	pData = reinterpret_cast<_ResourceSimpleMesh*>(GetData()->meshes.GetBinaryByCount());
		return ResourceSimpleMesh( pData + no );
	}

	// ボーン取得
	int GetNumBones() const { return GetData()->bones.GetCount(); }
	ResourceSimpleBone GetBone( int no )
	{
		MSL_ASSERT( (0 <= no) && (no < GetNumBones()) );
		_ResourceSimpleBone*	pData = reinterpret_cast<_ResourceSimpleBone*>(GetData()->bones.GetBinaryByCount());
		return ResourceSimpleBone( pData + no );
	}
};	// ResourceSimpleModel
*/
void CMNSSampleModelFileLoader::LoadFile( const hash::CHashStringPtr &path )
{
}

} } }

#endif