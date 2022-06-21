#ifdef __cplusplus
#	define {exportSpecifier} extern "C"
#	define {exportSpecifier}_OPEN extern "C" {{
#	define {exportSpecifier}_CLOSE }} // extern "C"
#else
#	define {exportSpecifier}
#	define {exportSpecifier}_OPEN
#	define {exportSpecifier}_CLOSE
#endif