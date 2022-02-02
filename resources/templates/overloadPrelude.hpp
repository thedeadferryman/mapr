#define {overloadPreludeDefinePrefix}_CAT2_I(x, y)    x##y
#define {overloadPreludeDefinePrefix}_CAT2(x, y)     {overloadPreludeDefinePrefix}_CAT2_I(x, y)

#define {overloadPreludeDefinePrefix}_CAT3_I(x, y, z) x##y##z
#define {overloadPreludeDefinePrefix}_CAT3(x, y, z)  {overloadPreludeDefinePrefix}_CAT3_I(x, y, z)

#define {overloadPreludeDefinePrefix}_ARGCOUNT_ORDINALS() \
	63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, \
		45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, \
		28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, \
		11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define {overloadPreludeDefinePrefix}_COMMA_ORDINALS() \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0

#define {overloadPreludeDefinePrefix}_MATCH_ORDINALS(_1, \
                       _2, \
                       _3, \
                       _4, \
                       _5, \
                       _6, \
                       _7, \
                       _8, \
                       _9, \
                       _10, \
                       _11, \
                       _12, \
                       _13, \
                       _14, \
                       _15, \
                       _16, \
                       _17, \
                       _18, \
                       _19, \
                       _20, \
                       _21, \
                       _22, \
                       _23, \
                       _24, \
                       _25, \
                       _26, \
                       _27, \
                       _28, \
                       _29, \
                       _30, \
                       _31, \
                       _32, \
                       _33, \
                       _34, \
                       _35, \
                       _36, \
                       _37, \
                       _38, \
                       _39, \
                       _40, \
                       _41, \
                       _42, \
                       _43, \
                       _44, \
                       _45, \
                       _46, \
                       _47, \
                       _48, \
                       _49, \
                       _50, \
                       _51, \
                       _52, \
                       _53, \
                       _54, \
                       _55, \
                       _56, \
                       _57, \
                       _58, \
                       _59, \
                       _60, \
                       _61, \
                       _62, \
                       _63, \
                       N, \
                       ...) \
	N

#define {overloadPreludeDefinePrefix}_ARGCOUNT_INT(...) {overloadPreludeDefinePrefix}_MATCH_ORDINALS(__VA_ARGS__)

#define {overloadPreludeDefinePrefix}_ARGCOUNT(...)     {overloadPreludeDefinePrefix}_ARGCOUNT_INT(__VA_ARGS__, {overloadPreludeDefinePrefix}_ARGCOUNT_ORDINALS())

#define {overloadPreludeDefinePrefix}_COMMA(...)          ,

#define {overloadPreludeDefinePrefix}_HAS_COMMA_I(...)        {overloadPreludeDefinePrefix}_MATCH_ORDINALS(__VA_ARGS__)
#define {overloadPreludeDefinePrefix}_HAS_COMMA(...)         {overloadPreludeDefinePrefix}_HAS_COMMA_I(__VA_ARGS__, {overloadPreludeDefinePrefix}_COMMA_ORDINALS())

#define {overloadPreludeDefinePrefix}_COMMA_SLUG(...) \
	{overloadPreludeDefinePrefix}_CAT2({overloadPreludeDefinePrefix}_HAS_COMMA(__VA_ARGS__), {overloadPreludeDefinePrefix}_HAS_COMMA({overloadPreludeDefinePrefix}_COMMA __VA_ARGS__()))

#define {overloadPreludeDefinePrefix}_ORD_SLUG(ORD)                {overloadPreludeDefinePrefix}_CAT3($, ORD, $$)
#define {overloadPreludeDefinePrefix}_OVERLOAD_SLUG(ORD, SLUG)     {overloadPreludeDefinePrefix}_CAT2({overloadPreludeDefinePrefix}_ORD_SLUG(ORD), SLUG)

#define {overloadPreludeDefinePrefix}_SELECT_OVERLOAD_NAME(NAME, SLUG) {overloadPreludeDefinePrefix}_CAT2(NAME, SLUG)
#define {overloadPreludeDefinePrefix}_SELECT_OVERLOAD_SLUG(...) {overloadPreludeDefinePrefix}_OVERLOAD_SLUG({overloadPreludeDefinePrefix}_ARGCOUNT(__VA_ARGS__), {overloadPreludeDefinePrefix}_COMMA_SLUG(__VA_ARGS__))
#define {overloadPreludeDefinePrefix}_SELECT_OVERLOAD(NAME, ...) {overloadPreludeDefinePrefix}_SELECT_OVERLOAD_NAME(NAME, {overloadPreludeDefinePrefix}_SELECT_OVERLOAD_SLUG(__VA_ARGS__))
#define {overloadPreludeDefinePrefix}_CALL_OVERLOAD(NAME, ...) {overloadPreludeDefinePrefix}_SELECT_OVERLOAD(NAME, __VA_ARGS__)(__VA_ARGS__)
