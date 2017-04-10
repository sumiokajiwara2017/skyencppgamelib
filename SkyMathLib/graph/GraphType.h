#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 無効グラフインデックス
/// </summary>
static const graphIndex GRAPH_INVALID_INDEX_NO = -1;

/// <summary>
/// インデックス保存用配列
/// </summary>
typedef CVectorPrimitive< graphIndex >     GraphIndexVector;

/// <summary>
/// インデックス保存用リスト
/// </summary>
typedef CList< u32 , graphIndex > GraphIndexList;

/// <summary>
/// コスト保存用配列
/// </summary>
typedef CVectorPrimitive< dectype >        CostVector;

} } }