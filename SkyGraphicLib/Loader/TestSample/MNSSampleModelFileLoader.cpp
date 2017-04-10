#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

#include "SkyGraphicLib/DataBase.h"

#ifdef SW_SKYLIB_DEBUG_ON

namespace sky { namespace lib { namespace graphic {

typedef u32	BinSize;

struct BinOffset
{
	BinSize	offset;

	// �R���X�g���N�^
	BinOffset()
		: offset(0)
	{}

	// �o�C�i���擾
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

	// �T�C�Y�擾
	inline BinSize GetSize() const
	{
		if( offset == 0 ) { return 0; }
		return *reinterpret_cast<BinSize*>( reinterpret_cast<BinSize>(this) + offset );
	}
	// ���擾
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
		FLAG_LOADED_FILE		= 1 << 0,				//!< �o�C�i���t�@�C���͂��̃N���X���ǂݍ���
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
		// �|�C���^�ƃT�C�Y�����R�s�[
		// Note: ���̃��\�[�X��������ꂽ�ꍇ�ɃA�N�Z�X�s�\�ɂȂ�̂Œ���
	}
	virtual ~ResourceBase()
	{
		Clear();
	}

	ResourceBase& operator=( const ResourceBase& res )
	{
		// �|�C���^�ƃT�C�Y�����R�s�[
		// Note: ���̃��\�[�X��������ꂽ�ꍇ�ɃA�N�Z�X�s�\�ɂȂ�̂Œ���
		m_pAllocator = NULL;
		m_pBinary    = res.m_pBinary;
		m_BinarySize = res.m_BinarySize;
		m_Flag       = 0;
		return *this;
	}

	void Clear()
	{
		if( util::ChkFlag<msl::u32>(m_Flag, FLAG_LOADED_FILE) )
		{	// ���̃N���X�œǂݍ��܂�Ă���̂ŁA���������������
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
	msl::u32		indexCount;				// �C���f�b�N�X��
	msl::BinOffset	indices;				// �C���f�b�N�X
};

struct _ResourceSimpleShape
{
	msl::BinOffset	attributes;				// ���_����
	msl::u32		vertexSize;				// ���_1�̃T�C�Y
	msl::u32		vertexCount;			// ���_��
	msl::BinOffset	vertices;				// ���_�f�[�^
	msl::BinOffset	geometories;			// �W�I���g���f�[�^
};

struct _ResourceSimpleMesh
{
	int		material;						// �}�e���A���ԍ�
	int		shape;							// �V�F�C�v�ԍ�
	int		geometory;						// �W�I���g���ԍ�
};

struct _ResourceSimpleMaterial
{
	msl::math::Vec4f	ambient;			// �A���r�G���g�J���[
	msl::math::Vec4f	diffuse;			// �f�B�t���[�Y�J���[
	msl::math::Vec4f	specular;			// �X�y�L�����J���[
};

struct _ResourceSimpleBone
{
	msl::tchar			name[32];			// ���O
	msl::math::Mtx34f	mtxInvInit;			// �����s��̋t�s��
	msl::math::Quatf	rotation;			// ��]
	msl::math::Vec3f	translate;			// ���s�ړ�
	msl::math::Vec3f	scaling;			// �X�P�[�����O
	int					parentNo;			// �e�{�[���̔ԍ�
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

	// ���_����
	int GetNumAttributes() const { return GetData()->attributes.GetCount(); }
	const SimpleVertexAttribute* GetAttributes() const { return reinterpret_cast<const SimpleVertexAttribute*>(GetData()->attributes.GetBinaryByCount()); }

	// ���_�f�[�^
	msl::u32 GetVertexSize() const { return GetData()->vertexSize; }
	msl::u32 GetVertexCount() const { return GetData()->vertexCount; }
	const void* GetVertices() const { return GetData()->vertices.GetBinary(); }

	// �W�I���g��
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

	// �}�e���A���擾
	int GetNumMaterials() const { return GetData()->materials.GetCount(); }
	ResourceSimpleMaterial GetMaterial( int no )
	{
		MSL_ASSERT( (0 <= no) && (no < GetNumMaterials()) );
		_ResourceSimpleMaterial*	pData = reinterpret_cast<_ResourceSimpleMaterial*>(GetData()->materials.GetBinaryByCount());
		return ResourceSimpleMaterial( pData + no );
	}

	// �V�F�C�v�擾
	int GetNumShapes() const { return GetData()->shapes.GetCount(); }
	ResourceSimpleShape GetShape( int no )
	{
		MSL_ASSERT( (0 <= no) && (no < GetNumShapes()) );
		_ResourceSimpleShape*	pData = reinterpret_cast<_ResourceSimpleShape*>(GetData()->shapes.GetBinaryByCount());
		return ResourceSimpleShape( pData + no );
	}

	// ���b�V���擾
	int GetNumMeshes() const { return GetData()->meshes.GetCount(); }
	ResourceSimpleMesh GetMesh( int no )
	{
		MSL_ASSERT( (0 <= no) && (no < GetNumMeshes()) );
		_ResourceSimpleMesh*	pData = reinterpret_cast<_ResourceSimpleMesh*>(GetData()->meshes.GetBinaryByCount());
		return ResourceSimpleMesh( pData + no );
	}

	// �{�[���擾
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