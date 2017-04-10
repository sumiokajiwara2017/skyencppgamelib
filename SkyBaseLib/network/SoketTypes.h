#pragma once

namespace sky { namespace lib { namespace net {

//ソケットハンドル型
typedef SOCKET socket_hndl;

//デフォルトのソケット接続カウント
static u32 DEFAULT_SOKET_CONNECT_COUNT = 5;

//デフォルトの受信バッファサイズ
static u32 DEFAULT_RECEIVE_TEMP_BUF_SIZE = 256;

} } }