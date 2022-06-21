# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(maprTests LANGUAGES CXX)

add_executable(mapr_test src/mapr_test.cpp)
target_link_libraries(mapr_test PRIVATE mapr-base)
target_compile_features(mapr_test PRIVATE cxx_std_20)

add_test(NAME mapr_test COMMAND mapr_test)
