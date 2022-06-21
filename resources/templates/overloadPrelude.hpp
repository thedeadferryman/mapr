#define {preludePrefix}_CAT2_I(x, y)    x##y
#define {preludePrefix}_CAT2(x, y)     {preludePrefix}_CAT2_I(x, y)

#define {preludePrefix}_CAT3_I(x, y, z) x##y##z
#define {preludePrefix}_CAT3(x, y, z)  {preludePrefix}_CAT3_I(x, y, z)

#define {preludePrefix}_ARGCOUNT_ORDINALS() \
	63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, \
		45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, \
		28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, \
		11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define {preludePrefix}_COMMA_ORDINALS() \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0

#define {preludePrefix}_MATCH_ORDINALS(_1, \
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

#define {preludePrefix}_ARGCOUNT_INT(...) {preludePrefix}_MATCH_ORDINALS(__VA_ARGS__)

#define {preludePrefix}_ARGCOUNT(...)     {preludePrefix}_ARGCOUNT_INT(__VA_ARGS__, {preludePrefix}_ARGCOUNT_ORDINALS())

#define {preludePrefix}_COMMA(...)          ,

#define {preludePrefix}_HAS_COMMA_I(...)        {preludePrefix}_MATCH_ORDINALS(__VA_ARGS__)
#define {preludePrefix}_HAS_COMMA(...)         {preludePrefix}_HAS_COMMA_I(__VA_ARGS__, {preludePrefix}_COMMA_ORDINALS())

#define {preludePrefix}_COMMA_SLUG(...) \
	{preludePrefix}_CAT2({preludePrefix}_HAS_COMMA(__VA_ARGS__), {preludePrefix}_HAS_COMMA({preludePrefix}_COMMA __VA_ARGS__()))

#define {preludePrefix}_ORD_SLUG(ORD)                {preludePrefix}_CAT3($, ORD, $$)
#define {preludePrefix}_OVERLOAD_SLUG(ORD, SLUG)     {preludePrefix}_CAT2({preludePrefix}_ORD_SLUG(ORD), SLUG)

#define {preludePrefix}_SELECT_OVERLOAD_NAME(NAME, SLUG) {preludePrefix}_CAT2(NAME, SLUG)
#define {preludePrefix}_SELECT_OVERLOAD_SLUG(...) {preludePrefix}_OVERLOAD_SLUG({preludePrefix}_ARGCOUNT(__VA_ARGS__), {preludePrefix}_COMMA_SLUG(__VA_ARGS__))
#define {preludePrefix}_SELECT_OVERLOAD(NAME, ...) {preludePrefix}_SELECT_OVERLOAD_NAME(NAME, {preludePrefix}_SELECT_OVERLOAD_SLUG(__VA_ARGS__))
#define {preludePrefix}_CALL_OVERLOAD(NAME, ...) {preludePrefix}_SELECT_OVERLOAD(NAME, __VA_ARGS__)(__VA_ARGS__)
