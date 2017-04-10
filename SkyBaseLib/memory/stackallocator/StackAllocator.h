#pragma once

namespace sky { namespace lib { namespace memory {

/// <summary>
/// スタックアロケーター
/// 基本確保した領域を積み上げていく形式だが、スタックの途中の領域を削除宣言できる。
/// 削除宣言された領域は削除リストに登録され、上位の領域がポップされた時に実際に使用可能な領域として開放される。
/// ※※※メモ※※※
/// ・スタックリストから未使用のメモリを捜すと処理負荷が高すぎるので削除リストを使用。
/// ・削除リストからの取得は線形検索なので改良の余地がある。
/// </summary>
class CStackAllocator : public IAllocator
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CStackAllocator();

	/// <summary>
	/// 指定の領域を管理させる
	/// </summary>
	inline void AttachMemory( void *pBuffer , u32 bufferSize , u32 blockSize = 32 , u32 backBlockSize = 32 , eAllocType eType = eAllocType_AllocatorClass_New );

	/// <summary>
	/// 全体メモリ確保
	/// </summary>
	inline void CreateMemory( u32 bufferSize , u32 blockSize = 32 , u32 backBlockSize = 32 , eAllocType eType = eAllocType_AllocatorClass_New );

	/// <summary>
	/// 全体メモリ開放
	/// </summary>
	inline void DeleteMemory();

	/// <summary>
	/// メモリ確保
	/// </summary>
	inline void *Malloc( u32 size , const skyMString *pFname , s32 line , eAllocSizeType eSize = eAllocSizeType_Block );

	/// <summary>
	/// メモリ後方確保
	/// </summary>
	inline void *BackMalloc( u32 size , const skyMString *pFname , s32 line , eAllocSizeType eSize = eAllocSizeType_Block );

	/// <summary>
	/// メモリ再確保
	/// </summary>
	inline void *Realloc( void *pData , u32 size , const skyMString *pFname , s32 line );

	/// <summary>
	/// 不要な値をメモリから削除
	/// もし削除された領域がスタックのトップじゃなかった場合、削除リストに登録し、削除済みのフラグを立てておく
	/// もし削除された領域がスタックのトップだった場合、自分と自分の直下の削除済みフラグのたった領域を消す（実際はトップの位置変数をマイナスするだけ）
	/// </summary>
	inline void Delete( void *pData );

	/// <summary>
	/// スタックのクリア
	/// </summary>
	inline void Clear();

	/// <summary>
	/// アロケート情報からアロケート情報を切り分ける
	/// </summary>
	inline void MargeMemoryArea( CAllocatorDataHeader *pMemory ); 

	/// <summary>
	/// 未使用のアロケート連続領域をマージする（使いどころは慎重に）
	/// </summary>
	inline CAllocatorDataHeader *MallocFromMallocData( CAllocatorDataHeader *pMemory , u32 Size );

	/// <summary>
	/// 全体サイズの取得
	/// </summary>
	inline u32 GetAllSize() const;

	/// <summary>
	/// 使用可能領域の取得
	/// </summary>
	inline u32 GetUsefulSize() const;

	/// <summary>
	/// スタックの使用中領域の取得(ヘッダー、フッターのサイズも加わるので確保した値よりも多くなる）
	/// </summary>
	inline u32 GetUsedStackSize() const;
	inline u32 GetFrontUsedStackSize() const;
	inline u32 GetBackUsedStackSize() const;

	/// <summary>
	/// 使用中領域の取得(スタックの使用中領域から削除リストの値を引いた値）
	/// </summary>
	inline u32 GetUsedSize() const;
	inline u32 GetFrontUsedSize() const;
	inline u32 GetBackUsedSize() const;

	/// <summary>
	/// メモリアロック数の取得
	/// </summary>
	inline u32 GetMallocNum() const;

	/// <summary>
	/// 削除リストのアイテム数（この値が高いと断片化しているといえる）
	/// </summary>
	inline u32 GetDeleteListItemNum() const;

	/// <summary>
	/// メモリチェック
	/// </summary>
	inline void AssertCheckMemory( void *pData );

	/// <summary>
	/// 指定のアドレスがアロケーター管理内だったらtrueを返す
	/// </summary>
	inline skyBool CheckAdr( void *pAdr ) const;

//------------インターフェースの実装（不要なパラメーターは使わないで中継する）--------------

	inline void *Malloc( u32 size ,  s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line );
	inline void *Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line );
	inline u32 GetAllocatorId();

	/// <summary>
	/// メモリ情報の出力
	/// </summary>
	inline void PrintMemory();

	/// <summary>
	/// メモリ破壊のチェック
	/// </summary>
	inline void CheckMemBreak();

private:

	/// <summary>
	/// スタック先頭位置
	/// </summary>
	u32  m_BlockSize;

	/// <summary>
	/// スタック先頭位置
	/// </summary>
	u32  m_BackBlockSize;

	/// <summary>
	/// スタック先頭位置
	/// </summary>
	u32  m_StackTop;

	/// <summary>
	/// スタック先頭番号
	/// </summary>
	u32  m_StackNo;

	/// <summary>
	/// 削除リスト登録メモリ量
	/// </summary>
	u32  m_DeleteListMemorySize;

	/// <summary>
	/// 後方スタック先頭位置
	/// </summary>
	u32  m_BackStackTop;

	/// <summary>
	/// 後方スタック先頭番号
	/// </summary>
	u32  m_BackStackNo;

	/// <summary>
	/// 後方削除リスト登録メモリ量
	/// </summary>
	u32  m_BackDeleteListMemorySize;

	/// <summary>
	/// 確保領域サイズ
	/// </summary>
	u32 m_BufferSize;

	/// <summary>
	/// 領域のポインタ
	/// </summary>
	void *m_pBuffer;

	/// <summary>
	/// アロケーターID
	/// </summary>
	u32 m_AllocatorId;

	/// <summary>
	/// アロックタイプ
	/// </summary>
	eAllocType m_eType;

	/// <summary>
	/// スタックリスト(双方向リストなのでその型自体がルートになる）
	/// </summary>
	CAllocatorDataHeaderPtrDList  m_StackList;

	/// <summary>
	/// バックスタックリスト(双方向リストなのでその型自体がルートになる）
	/// </summary>
	CAllocatorDataHeaderPtrDList  m_BackStackList;

	/// <summary>
	/// 削除リスト(双方向リストなのでその型自体がルートになる）
	/// </summary>
	CAllocatorDeleteDataHeaderPtrDList m_DeleteList;

	/// <summary>
	/// 後方削除リスト(双方向リストなのでその型自体がルートになる）
	/// </summary>
	CAllocatorDeleteDataHeaderPtrDList m_BackDeleteList;

	/// <summary>
	/// 32バイトブロック削除リスト(双方向リストなのでその型自体がルートになる）
	/// </summary>
	CAllocatorDeleteDataHeaderPtrDList m_BlockDeleteList;

	/// <summary>
	/// 32バイト後方ブロック削除リスト(双方向リストなのでその型自体がルートになる）
	/// </summary>
	CAllocatorDeleteDataHeaderPtrDList m_BackBlockDeleteList;
};

} } }

#include "StackAllocator.inl"