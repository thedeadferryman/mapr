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

namespace kodgen::util {

class MatchType {
	template<typename Type>
	struct RemoveClass {};

	template<typename Class, typename Ret, typename... Args>
	struct RemoveClass<Ret (Class::*)(Args...)> {
		using Type = Ret(Args...);
	};
	template<typename Class, typename Ret, typename... Args>
	struct RemoveClass<Ret (Class::*)(Args...) const> {
		using Type = Ret(Args...);
	};

	template<typename Func>
	struct ExtractSignatureI {
		using Type = typename RemoveClass<
			decltype(&std::remove_reference<Func>::type::operator())>::Type;
	};
	template<typename Ret, typename... Args>
	struct ExtractSignatureI<Ret(Args...)> {
		using Type = Ret(Args...);
	};

	template<typename Ret, typename... Args>
	struct ExtractSignatureI<Ret (*)(Args...)> {
		using Type = Ret(Args...);
	};

	template<typename Ret, typename... Args>
	struct ExtractSignatureI<Ret (&)(Args...)> {
		using Type = Ret(Args...);
	};

	template<typename Func>
	using ExtractSignature = typename ExtractSignatureI<Func>::Type;

	template<typename Result, typename Base, typename Type>
	static auto matchTypeHelper(
		const std::shared_ptr<Base>& base,
		std::function<Result(std::shared_ptr<Type>)> /*unused*/)
		-> std::shared_ptr<Type> {
		return std::dynamic_pointer_cast<Type>(base);
	}

	template<typename Result, typename Base, typename Type>
	static auto matchTypeHelper(
		const std::shared_ptr<Base>& base,
		std::function<Result(const std::shared_ptr<Type>&)> /*unused*/)
		-> std::shared_ptr<Type> {
		return std::dynamic_pointer_cast<Type>(base);
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
		using Signature = ExtractSignature<Branch>;

		if (auto matched = matchTypeHelper<Result>(
				base, static_cast<std::function<Signature>>(branch))) {
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
	static void switchType(Base /*ignored*/) {
		UNREACHABLE();
	}
};

}  // namespace kodgen::util
