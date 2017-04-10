#pragma once

namespace sky { namespace lib { namespace stream {

/// <summary>
/// ストリームの種類
/// </summary>
enum eStreamKind
{
	eStreamKind_memory = 0, //メモリ上の値をストリームとしてみてる（ほとんどこれかも）
	eStreamKind_file,
	eStreamKind_net,
	eStreamKind_console,
};

} } }