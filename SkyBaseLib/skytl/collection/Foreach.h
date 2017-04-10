/// <summary>
/// Collection型をforeach風にまわす
/// t = 型
/// i = イテレーター
/// c = Collectionの実体
/// 例）FOREACH( HOGECLASS , it , list )
/// </summary>
#define FOREACH( t , it , collection ) \
for( t::iterator it=( collection ).Begin() ; ( it ) != ( collection ).End() ; ( it )++ )

#define FOREACH_REVERS( t , it , collection ) \
t::iterator it = ( collection ).End();        \
it--;                                         \
for( ; ( collection ).IsBeginBefor( it ) == skyFalse ; ( it )-- )

#define FOREACH_NOINC( t , it , collection ) \
for( t::iterator it=( collection ).Begin() ; ( it ) != ( collection ).End() ; )
