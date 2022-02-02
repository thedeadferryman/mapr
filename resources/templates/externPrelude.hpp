#ifdef __cplusplus
#	define {externDefinePrefix}             extern "C"
#	define {externDefinePrefix}_BLOCK_START extern "C" {{
#	define {externDefinePrefix}_BLOCK_END   }};
#else
#	define {externDefinePrefix}
#	define {externDefinePrefix}_BLOCK_START
#	define {externDefinePrefix}_BLOCK_END
#endif