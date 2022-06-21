//
// Created by karl on 31.03.22.
//

#include "recordHandler.hpp"

#include "match/matchers/bannedOperators.hpp"
#include "match/nameResolver.hpp"
#include "match/overloadMismatchException.hpp"
#include "match/typeResolver.hpp"

#include "view/record/recordOperatorDecl.hpp"
#include "view/record/syntheticGetterDecl.hpp"
#include "view/record/syntheticSetterDecl.hpp"
#include "view/types/builtinType.hpp"
#include "view/types/recordType.hpp"

#include "util/stringBuilder.hpp"

#include "match/matchers/internal/functionHelpers.hpp"

using mapr::match::OverloadMismatchException;
using mapr::match::RecordHandler;

using mapr::view::BuiltinType;
using mapr::view::ConstMode;
using mapr::view::DeclBase;
using mapr::view::FunctionDecl;
using mapr::view::FunctionOverload;
using mapr::view::PointerType;
using mapr::view::RecordCtorDecl;
using mapr::view::RecordDecl;
using mapr::view::RecordDtorDecl;
using mapr::view::RecordKind;
using mapr::view::RecordMethodDecl;
using mapr::view::RecordType;
using mapr::view::SingleSourceLoc;
using mapr::view::SparseSourceLoc;
using mapr::view::VarDecl;

using mapr::util::StringBuilder;

using clang::CXXConstructorDecl;
using clang::CXXDestructorDecl;
using clang::CXXMethodDecl;

using clang::CXXRecordDecl;

[[nodiscard]] auto createDefaultDtor(
	const std::shared_ptr<const RecordDecl>& owner)
	-> std::shared_ptr<const FunctionDecl>;

[[nodiscard]] auto createDefaultCtor(
	const std::shared_ptr<const RecordDecl>& owner)
	-> std::shared_ptr<const FunctionDecl>;

[[nodiscard]] auto getRecordKind(const CXXRecordDecl* decl) -> RecordKind;

[[nodiscard]] auto insertAsOverload(
	const std::shared_ptr<const FunctionDecl>& methDecl,
	const std::shared_ptr<const DeclBase>& foundDecl)
	-> std::shared_ptr<FunctionDecl>;

RecordHandler::RecordHandler(const clang::ASTContext* astContext,
                             const clang::SourceManager* sourceManager,
                             std::shared_ptr<view::DeclContext> declContext)
	: sourceManager(sourceManager)
	, astContext(astContext)
	, declContext(std::move(declContext)) {}

auto RecordHandler::mapRecordMethod(
	const CXXMethodDecl* methodDecl,
	const std::shared_ptr<const RecordDecl>& owner) const
	-> std::shared_ptr<const FunctionDecl> {
	auto qualName = NameResolver::resolveDeclName(methodDecl, astContext);
	auto args = std::vector<std::shared_ptr<const VarDecl>>();

	qualName = owner->getQualifiedName() / qualName.basename();

	if (!methodDecl->isStatic()) {
		auto classType = RecordType::makePointer(  //
			owner->getQualifiedName(),
			(methodDecl->isConst()  //
		         ? ConstMode::ConstValue  //
		         : ConstMode::None));

		args.push_back(std::make_shared<VarDecl>(
			"self", classType, std::make_shared<SparseSourceLoc>()  //
			));
	}

	buildParmList(methodDecl, args);

	auto funcOverload = FunctionOverload(
		TypeResolver::resolve(methodDecl->getReturnType(), astContext),
		std::move(args),
		makeDeclLoc(methodDecl)  //
	);

	[[unlikely]] if (methodDecl->isStatic()) {
		return std::make_shared<FunctionDecl>(qualName.str(),
		                                      std::move(qualName),
		                                      std::move(funcOverload)  //
		);
	}

	return std::make_shared<RecordMethodDecl>(qualName.str(),
	                                          std::move(qualName),
	                                          std::move(funcOverload)  //
	);
}

auto RecordHandler::mapOverloadedOperator(
	const clang::CXXMethodDecl* operDecl,
	const std::shared_ptr<const view::RecordDecl>& owner) const
	-> std::shared_ptr<const view::FunctionDecl> {
	auto qualName = NameResolver::resolveDeclName(operDecl, astContext);
	auto args = std::vector<std::shared_ptr<const VarDecl>>();

	qualName = owner->getQualifiedName() / qualName.basename();

	if (!operDecl->isStatic()) {
		auto classType = RecordType::makePointer(  //
			owner->getQualifiedName(),
			(operDecl->isConst()  //
		         ? ConstMode::ConstValue  //
		         : ConstMode::None));

		args.push_back(std::make_shared<VarDecl>(
			"self", classType, std::make_shared<SparseSourceLoc>()  //
			));
	}

	buildParmList(operDecl, args);

	auto funcOverload = FunctionOverload(
		TypeResolver::resolve(operDecl->getReturnType(), astContext),
		std::move(args),
		makeDeclLoc(operDecl)  //
	);

	[[unlikely]] if (operDecl->isStatic()) {
		return std::make_shared<FunctionDecl>(qualName.str(),
		                                      std::move(qualName),
		                                      std::move(funcOverload)  //
		);
	}

	return std::make_shared<view::RecordOperatorDecl>(
		qualName.str(),
		std::move(qualName),
		std::move(funcOverload),
		operDecl->getOverloadedOperator()  //
	);
}

auto RecordHandler::mapCtor(const CXXConstructorDecl* ctorDecl,
                            const std::shared_ptr<const RecordDecl>& owner)
	const -> std::shared_ptr<const FunctionDecl> {
	auto qualName = NameResolver::resolveDeclName(ctorDecl, astContext);
	auto args = std::vector<std::shared_ptr<const VarDecl>>();
	auto ownerPtr = RecordType::makePointer(owner->getQualifiedName());

	buildParmList(ctorDecl, args);

	auto ctorOverload = FunctionOverload(ownerPtr,
	                                     std::move(args),
	                                     makeDeclLoc(ctorDecl)  //
	);

	if (!qualName.isValid()) {
		llvm::errs() << qualName.str() << ' '
					 << ctorDecl->getQualifiedNameAsString() << ' '
					 << ctorDecl->getBeginLoc().printToString(*sourceManager)
					 << '\n';
	}

	return std::make_shared<view::RecordCtorDecl>(
		qualName.str(), std::move(qualName), std::move(ctorOverload));
}

auto RecordHandler::mapDtor(const CXXDestructorDecl* dtorDecl,
                            const std::shared_ptr<const RecordDecl>& owner)
	const -> std::shared_ptr<const FunctionDecl> {
	auto qualName = NameResolver::resolveDeclName(dtorDecl, astContext);
	auto classType = RecordType::makePointer(owner->getQualifiedName());

	auto classDRef = std::make_shared<PointerType>(std::move(classType));

	auto dtorOverload = FunctionOverload(
		view::BuiltinType::makeVoid(),
		{
			std::make_shared<VarDecl>(
				"self", classDRef, std::make_shared<SparseSourceLoc>()),
		},
		makeDeclLoc(dtorDecl)  //
	);

	return std::make_shared<view::RecordDtorDecl>(
		qualName.str(), std::move(qualName), std::move(dtorOverload));
}

void RecordHandler::buildParmList(
	const clang::CXXMethodDecl* methodDecl,
	std::vector<std::shared_ptr<const view::VarDecl>>& args) const {
	std::transform(methodDecl->param_begin(),
	               methodDecl->param_end(),
	               std::back_inserter(args),
	               [this](const clang::ParmVarDecl* parm) {
					   return std::make_shared<view::VarDecl>(  //
						   FunctionHelpers::getParmName(parm),  //
						   TypeResolver::resolve(parm->getType(), astContext),
						   makeDeclLoc(parm)  //
					   );
				   });
}

auto createDefaultCtor(const std::shared_ptr<const RecordDecl>& owner)
	-> std::shared_ptr<const FunctionDecl> {
	auto ownerLoc =
		std::dynamic_pointer_cast<SingleSourceLoc>(owner->getLocation());

	if (!ownerLoc) {
		UNREACHABLE();
	}

	auto ctorOverload = FunctionOverload(
		RecordType::makePointer(owner->getQualifiedName()), {}, ownerLoc);
	auto qualName =
		owner->getQualifiedName() / owner->getQualifiedName().basename();

	return std::make_shared<RecordCtorDecl>(
		qualName.str(), std::move(qualName), std::move(ctorOverload));
}
auto getRecordKind(const CXXRecordDecl* decl) -> RecordKind {
	if (decl->isStruct()) {
		return RecordKind::Struct;
	}
	if (decl->isClass()) {
		return RecordKind::Class;
	}

	return RecordKind::Union;
}
auto createDefaultDtor(const std::shared_ptr<const RecordDecl>& owner)
	-> std::shared_ptr<const FunctionDecl> {
	auto ownerLoc =
		std::dynamic_pointer_cast<SingleSourceLoc>(owner->getLocation());

	if (!ownerLoc) {
		UNREACHABLE();
	}

	auto dtorBasename = std::string(
		StringBuilder("~") << owner->getQualifiedName().basename().str());
	auto qualName = owner->getQualifiedName() / dtorBasename;

	auto classType = RecordType::makePointer(owner->getQualifiedName());
	auto classDRef = std::make_shared<PointerType>(classType);

	auto dtorOverload = FunctionOverload(
		BuiltinType::makeVoid(),
		{
			std::make_shared<VarDecl>(
				"self", classDRef, std::make_shared<SparseSourceLoc>()),
		},
		ownerLoc  //
	);

	return std::make_shared<RecordDtorDecl>(
		qualName.str(), std::move(qualName), std::move(dtorOverload));
}

auto insertAsOverload(const std::shared_ptr<const FunctionDecl>& methDecl,
                      const std::shared_ptr<const DeclBase>& foundDecl)
	-> std::shared_ptr<FunctionDecl> {
	if (methDecl->getDeclType() != foundDecl->getDeclType()) {
		throw OverloadMismatchException(methDecl->getQualifiedName().str(),
		                                methDecl->getLocation(),
		                                foundDecl->getDeclType(),
		                                methDecl->getDeclType(),
		                                foundDecl->getLocation()  //
		);
	}

	if (auto funcDecl =
	        std::dynamic_pointer_cast<const FunctionDecl>(foundDecl)) {
		auto current = funcDecl->clone();

		for (const auto& ovl : methDecl->getOverloads()) {
			current->addOverload(ovl);
		}

		return current;
	}
	UNREACHABLE();
}

auto shouldParseMethod(const clang::CXXMethodDecl* method) -> bool {
	return (method->getVisibility() == clang::DefaultVisibility)
		&& (method->getAccess()
	        == clang::AccessSpecifier::AS_public)  // method is public
		&& !(method->isVirtual()
	         && method->isPure())  // method is not pure virtual (=0)
		&& !method->isDeleted()  // method is not deleted (=delete)
		&& !mapr::match::isBannedOperator(
			   method->getOverloadedOperator())  // OO_None (not an operator) is
	                                             // not banned
		;
}

auto RecordHandler::mapMethods(
	const clang::CXXRecordDecl* pRecordDecl,
	const std::shared_ptr<const view::RecordDecl>& record)
	-> std::vector<std::shared_ptr<const view::FunctionDecl>> {
	bool hasCtor = false;
	bool hasDtor = false;

	auto methods = std::vector<std::shared_ptr<const FunctionDecl>>();

	for (const auto* method : pRecordDecl->methods()) {
		if (const auto* ctor = llvm::dyn_cast<CXXConstructorDecl>(method)) {
			hasCtor = true;
			if (shouldParseMethod(ctor)) {
				methods.push_back(mapCtor(ctor, record));
			}
		} else if (const auto* dtor =
		               llvm::dyn_cast<CXXDestructorDecl>(method)) {
			hasDtor = true;
			if (shouldParseMethod(dtor)) {
				methods.push_back(mapDtor(dtor, record));
			}
		} else if (method->isOverloadedOperator()
		           && shouldParseMethod(method)) {
			methods.push_back(mapOverloadedOperator(method, record));
		} else if (shouldParseMethod(method)) {
			methods.push_back(mapRecordMethod(method, record));
		}
	}

	if (!hasCtor) {
		methods.push_back(createDefaultCtor(record));
	}
	if (!hasDtor) {
		methods.push_back(createDefaultDtor(record));
	}

	return methods;
}

auto RecordHandler::makeSyntheticGetter(
	const clang::FieldDecl* field,  //
	const std::shared_ptr<const RecordDecl>& owner  //
) const -> std::shared_ptr<const FunctionDecl> {
	auto selfArg = std::make_shared<VarDecl>(
		"self",
		RecordType::makePointer(owner->getQualifiedName(),
	                            ConstMode::ConstValue),
		std::make_shared<SparseSourceLoc>()  //
	);
	auto fieldType = TypeResolver::resolve(field->getType(), astContext);
	auto fieldName = NameResolver::resolveNamedDeclName(field, astContext);

	fieldName = owner->getQualifiedName() / fieldName.basename();

	auto overload = FunctionOverload(fieldType, {selfArg}, makeDeclLoc(field));

	return std::make_shared<view::SyntheticGetterDecl>(fieldName, overload);
}

auto RecordHandler::makeSyntheticSetter(
	const clang::FieldDecl* field,
	const std::shared_ptr<const RecordDecl>& owner) const
	-> std::shared_ptr<const FunctionDecl> {
	auto selfArg = std::make_shared<VarDecl>(
		"self",
		RecordType::makePointer(owner->getQualifiedName(), ConstMode::None),
		std::make_shared<SparseSourceLoc>()  //
	);

	auto fieldType = TypeResolver::resolve(field->getType(), astContext);
	auto fieldName = NameResolver::resolveNamedDeclName(field, astContext);
	auto fieldDecl = std::make_shared<VarDecl>(
		fieldName.basename().str(), fieldType, makeDeclLoc(field));

	fieldName = owner->getQualifiedName() / fieldName.basename();

	auto args = std::vector<std::shared_ptr<const VarDecl>>();

	args.push_back(selfArg);
	args.push_back(fieldDecl);

	auto overload = FunctionOverload(
		view::BuiltinType::makeVoid(), std::move(args), makeDeclLoc(field));

	return std::make_shared<view::SyntheticSetterDecl>(fieldName, overload);
}

auto RecordHandler::mapFieldsToMethods(
	const CXXRecordDecl* pRecordDecl,
	const std::shared_ptr<const RecordDecl>& record) const
	-> std::vector<std::shared_ptr<const FunctionDecl>> {
	auto fieldMethods = std::vector<std::shared_ptr<const FunctionDecl>>();

	for (const auto* field : pRecordDecl->fields()) {
		if (field->getAccess() == clang::AccessSpecifier::AS_public
		    && !field->getName().str().empty()) {
			fieldMethods.push_back(makeSyntheticGetter(field, record));
			fieldMethods.push_back(makeSyntheticSetter(field, record));
		}
	}

	return fieldMethods;
}

void RecordHandler::handle(const clang::CXXRecordDecl* pRecordDecl,
                           const view::QualifiedName& qualName) {
	if (pRecordDecl->isTemplated()) {
		return;  // we don't support templates as for now
	}

	auto record = std::make_shared<RecordDecl>(  //
		qualName.str(),
		qualName,
		getRecordKind(pRecordDecl),
		makeDeclLoc(pRecordDecl)  //
	);

	if (declContext->findDeclaration(record->getID()) != nullptr) {
		return;
	}

	auto methods = mapMethods(pRecordDecl, record);
	auto fieldAccessors = mapFieldsToMethods(pRecordDecl, record);

	std::copy(std::begin(fieldAccessors),
	          std::end(fieldAccessors),
	          std::back_inserter(methods));

	for (const auto& method : methods) {
		if (pRecordDecl->isAbstract()
		    && (method->getDeclType() == view::DeclType::RecordCtor
		        || method->getDeclType() == view::DeclType::RecordDtor)) {
			continue;
		}

		record->addMember(method->getID());

		if (auto foundDecl = declContext->findDeclaration(method->getID())) {
			if (method->getDeclType() == view::DeclType::RecordDtor) {
				llvm::errs()
					<< method->getID() << ' ' << foundDecl->getID() << "\n";
				UNREACHABLE_M("Got two destructors!");
			}
			declContext->replaceDeclaration(
				insertAsOverload(method, foundDecl)  //
			);
		} else {
			declContext->addDeclaration(method);
		}
	}

	declContext->addDeclaration(record);
}

auto RecordHandler::makeDeclLoc(const clang::Decl* decl) const
	-> std::shared_ptr<SingleSourceLoc> {
	return SingleSourceLoc::fromClangLoc(
		clang::FullSourceLoc(decl->getBeginLoc(), *sourceManager)  //
	);
}
