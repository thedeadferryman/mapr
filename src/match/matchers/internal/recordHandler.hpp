//
// Created by karl on 31.03.22.
//

#pragma once

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include "view/declContext.hpp"
#include "view/record/recordCtorDecl.hpp"
#include "view/record/recordDecl.hpp"
#include "view/record/recordDtorDecl.hpp"
#include "view/record/recordMethodDecl.hpp"

namespace mapr::match {

class RecordHandler {
	const clang::SourceManager* sourceManager;
	const clang::ASTContext* astContext;
	std::shared_ptr<view::DeclContext> declContext;

  public:
	RecordHandler(const clang::ASTContext* astContext,
	              const clang::SourceManager* sourceManager,
	              std::shared_ptr<view::DeclContext> declContext);

	void handle(const clang::CXXRecordDecl* pRecordDecl,
	            const view::QualifiedName& qualName);

  private:
	void buildParmList(const clang::CXXMethodDecl* methodDecl,
	                   std::vector<std::shared_ptr<const view::VarDecl>>& args) const;

	[[nodiscard]] auto mapRecordMethod(
		const clang::CXXMethodDecl* methodDecl,
		const std::shared_ptr<const view::RecordDecl>& owner) const
		-> std::shared_ptr<const view::FunctionDecl>;

	[[nodiscard]] auto mapOverloadedOperator(
		const clang::CXXMethodDecl* operDecl,
		const std::shared_ptr<const view::RecordDecl>& owner) const
		-> std::shared_ptr<const view::FunctionDecl>;

	[[nodiscard]] auto mapCtor(const clang::CXXConstructorDecl* ctorDecl,
	                           const std::shared_ptr<const view::RecordDecl>& owner)
		const -> std::shared_ptr<const view::FunctionDecl>;

	[[nodiscard]] auto mapDtor(const clang::CXXDestructorDecl* dtorDecl,
	                           const std::shared_ptr<const view::RecordDecl>& owner)
		const -> std::shared_ptr<const view::FunctionDecl>;

	[[nodiscard]] auto mapMethods(
		const clang::CXXRecordDecl* pRecordDecl,
		const std::shared_ptr<const view::RecordDecl>& record)
		-> std::vector<std::shared_ptr<const view::FunctionDecl>>;

	[[nodiscard]] auto mapFieldsToMethods(
		const clang::CXXRecordDecl* pRecordDecl,
		const std::shared_ptr<const view::RecordDecl>& record) const
		-> std::vector<std::shared_ptr<const view::FunctionDecl>>;

	[[nodiscard]] auto makeSyntheticSetter(
		const clang::FieldDecl* field,
		const std::shared_ptr<const view::RecordDecl>& owner) const
		-> std::shared_ptr<const view::FunctionDecl>;

	[[nodiscard]] auto makeSyntheticGetter(
		const clang::FieldDecl* field,
		const std::shared_ptr<const view::RecordDecl>& owner) const
		-> std::shared_ptr<const view::FunctionDecl>;

	[[nodiscard]] auto makeDeclLoc(const clang::Decl* decl) const
		-> std::shared_ptr<view::SingleSourceLoc>;
};

}  // namespace mapr::match