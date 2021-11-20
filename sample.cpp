//
// Created by kmeinkopf on 13.11.2021.
//
#include <vector>

using namespace std;

int square(int k) {
	return k * k;
}

auto square2(int k) {
	return k * k;
}

auto sum(int a, int b) {
	return a + b;
}

auto sumOver(vector<int> v) {
	int a = 0;

	for (auto item : v) {
		a += item;
	}

	return a;
}

vector<int> repeat(int a, int b) {
	auto v = vector<int>();

	for (int i = 0; i < b; ++i) {
		v.push_back(a);
	}

	return v;
}