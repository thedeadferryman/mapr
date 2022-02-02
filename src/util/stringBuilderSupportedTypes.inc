//
// Created by kmeinkopf on 19.01.2022.
//

#define INT_SIZES               8, 16, 32, 64
#define INT_TYPES_I(_Size)      std::int##_Size##_t, std::uint##_Size##_t,
#define INT_TYPES(_, __, _Size) INT_TYPES_I(_Size)
#define ALL_INT_TYPES() \
	BOOST_PP_SEQ_FOR_EACH(INT_TYPES, %, BOOST_PP_VARIADIC_TO_SEQ(INT_SIZES))

#define OPERATORS_FOR(...) \
	BOOST_PP_SEQ_FOR_EACH( \
		OPERATOR_FOR, %, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

OPERATORS_FOR(  //
	std::string_view,
	char,  //
	ALL_INT_TYPES()  //
	float,
	double  //
)

#undef INT_SIZES
#undef INT_TYPES_I
#undef INT_TYPES
#undef ALL_INT_TYPES
#undef OPERATOR_FOR
#undef OPERATORS_FOR
