//
// Created by kmeinkopf on 19.01.2022.
//

#pragma once

#include <functional>
#include <memory>
#include <optional>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/type_traits/function_traits.hpp>

namespace mapr::util {

class MatchType {
	template<int32_t Ordinal, typename Head, typename... Args>
	struct GetArgI {
		using Type = GetArgI<Ordinal - 1, Args...>;
	};

	template<typename Head, typename... Args>
	struct GetArgI<0, Head, Args...> {
		using Type = Head;
	};

	template<int32_t Ordinal, typename... Args>
	using GetArg = typename GetArgI<Ordinal, Args...>::Type;

	template<typename... Args>
	struct ParamPack {
		template<int32_t Ordinal>
		using At = GetArg<Ordinal, Args...>;
	};

	template<typename Type>
	struct RemoveClass {};

	template<typename Class, typename Ret, typename... Args>
	struct RemoveClass<Ret (Class::*)(Args...)> {
		using Signature = Ret(Args...);
		using Arguments = ParamPack<Args...>;
	};

	template<typename Class, typename Ret, typename... Args>
	struct RemoveClass<Ret (Class::*)(Args...) const> {
		using Signature = Ret(Args...);
		using Arguments = ParamPack<Args...>;
	};

	template<typename Func>
	struct ExtractSignatureI {
		using ClassRemoved = RemoveClass<
			decltype(&std::remove_reference<Func>::type::operator())>;
		using Signature = typename ClassRemoved::Signature;
		using Arguments = typename ClassRemoved::Arguments;
	};

	template<typename Ret, typename... Args>
	struct ExtractSignatureI<Ret(Args...)> {
		using Signature = Ret(Args...);
		using Arguments = ParamPack<Args...>;
	};

	template<typename Ret, typename... Args>
	struct ExtractSignatureI<Ret (*)(Args...)> {
		using Signature = Ret(Args...);
		using Arguments = ParamPack<Args...>;
	};

	template<typename Ret, typename... Args>
	struct ExtractSignatureI<Ret (&)(Args...)> {
		using Signature = Ret(Args...);
		using Arguments = ParamPack<Args...>;
	};

	template<typename Tp>
	struct NakedTypeI {
		using Type = typename std::remove_reference<Tp>::type;
	};

	template<typename Tp>
	struct NakedTypeI<std::shared_ptr<Tp>> {
		using Type = Tp;
	};

	template<typename Tp>
	struct NakedTypeI<const std::shared_ptr<Tp>> {
		using Type = Tp;
	};

	template<typename Tp>
	struct NakedTypeI<const std::shared_ptr<Tp>&> {
		using Type = Tp;
	};

	template<typename Tp>
	struct NakedTypeI<std::shared_ptr<Tp>&> {
		using Type = Tp;
	};

	template<typename Type>
	using NakedType = typename NakedTypeI<Type>::Type;

	template<typename Func>
	using ExtractSignature = typename ExtractSignatureI<Func>::Signature;

	template<typename Func>
	using ExtractArguments = typename ExtractSignatureI<Func>::Arguments;

	template<typename Type, typename Base>
	static auto matchTypeHelper(const std::shared_ptr<Base>& base) -> typename std::remove_reference<Type>::type {
		return std::dynamic_pointer_cast<NakedType<Type>>(base);
	}

	template<typename Base, typename Type>
	static auto switchTypeHelper(
		std::shared_ptr<Base> base,
		std::function<void(std::shared_ptr<Type>)> branch) -> bool {
		if (auto casted = std::dynamic_pointer_cast<Type>(base)) {
			branch(casted);
			return true;
		}

		return false;
	}

	template<typename Base, typename Type>
	static auto switchTypeHelper(
		const std::shared_ptr<Base>& base,
		std::function<void(const std::shared_ptr<Type>&)> branch) -> bool {
		if (auto casted = std::dynamic_pointer_cast<Type>(base)) {
			branch(casted);
			return true;
		}

		return false;
	}

  public:
	template<typename Result,
	         typename Base,
	         typename Branch,
	         typename... RestBranches>
	static auto matchType(std::shared_ptr<Base> base,
	                      Branch&& branch,
	                      RestBranches&&... restBranches) -> Result {
		using BranchMatchedType =
			typename ExtractArguments<Branch>::template At<0>;

		if (auto matched = matchTypeHelper<BranchMatchedType, Base>(base)) {
			return branch(matched);
		}

		return matchType<Result>(base, restBranches...);
	}

	template<typename Result, typename Base>
	static auto matchType(Base /*ignored*/) -> Result {
		UNREACHABLE();
	}

	template<typename Base, typename Branch, typename... RestBranches>
	static void switchType(std::shared_ptr<Base> base,
	                       Branch&& branch,
	                       RestBranches&&... restBranches) {
		using Signature = ExtractSignature<Branch>;

		if (switchTypeHelper(  //
				base,
				static_cast<std::function<Signature>>(branch))) {
			return;
		}

		switchType(base, restBranches...);
	}

	template<typename Base>
	static void switchType(Base /*ignored*/) {}
};

}  // namespace mapr::util
