//
// Created by kmeinkopf on 26.12.2021.
//

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-macro-usage"

#include <string_view>

#include <boost/assert.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#define S_ENUM_ENTRY_VAR(_, _EnumName, _Entry) \
	case _EnumName::_Entry: \
		return BOOST_PP_STRINGIZE(_Entry);
#define S_ENUM_ALL_ENTRIES(_EnumName, ...) \
	BOOST_PP_SEQ_FOR_EACH( \
		S_ENUM_ENTRY_VAR, _EnumName, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define S_ENUM3(_Name, _Type, ...) \
	enum class _Name : _Type { \
		__VA_ARGS__ \
	}; \
	[[maybe_unused]] [[nodiscard]] constexpr auto stringOf##_Name(_Name value) \
		->std::string_view { \
		switch (value) { S_ENUM_ALL_ENTRIES(_Name, __VA_ARGS__) } \
		return ""; \
	}

#define S_ENUM(_Name, ...) S_ENUM3(_Name, int, __VA_ARGS__)

#define BIT_ENUM3(_Name, _Type, ...) \
	enum class _Name : _Type { \
		__VA_ARGS__ \
	}; \
	[[maybe_unused]] [[nodiscard]] inline auto operator|(_Name lhs, _Name rhs) \
		->_Name { \
		return static_cast<_Name>(static_cast<_Type>(lhs) \
		                          | static_cast<_Type>(rhs)); \
	} \
	[[maybe_unused]] [[nodiscard]] inline auto operator&(_Name lhs, _Name rhs) \
		->_Name { \
		return static_cast<_Name>(static_cast<_Type>(lhs) \
		                          & static_cast<_Type>(rhs)); \
	} \
	[[maybe_unused]] [[nodiscard]] inline auto operator<=>(_Name lhs, \
	                                                       _Type rhs) { \
		return static_cast<_Type>(lhs) <=> rhs; \
	}

#define BIT_ENUM(_Name, ...) BIT_ENUM3(_Name, int, __VA_ARGS__)

#define UNREACHABLE()        BOOST_ASSERT_MSG(false, "Should not reach here!")
#define UNREACHABLE_M(_Msg) \
	BOOST_ASSERT_MSG(false, "Should not reach here: " _Msg)

#if defined(DEV_MODE)
#	define DEBUG
#else
#	define DEBUG if (false)
#endif

#pragma clang diagnostic pop