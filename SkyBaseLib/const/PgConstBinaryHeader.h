#pragma once 
//※※【このヘッダーファイルはExcelMatrixConvertBinaryツールによる自動生成です】※※ 
//Excel表からコンバートしたバイナリファイルのヘッダーの型。この後可変長のデータが続く。
#pragma pack(push,1) 
struct ExcelConvBinType2Header { 
   u32 toolMark; 
   u32 toolVer; 
}; 
#pragma pack(pop) 
