// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <execution>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct Vals {
	long a;
	long b;
	long c;
};

static auto parse(const std::vector<std::string> &lines) {
	std::vector<Vals> ret;
	ret.reserve(14);

	Vals temp;
	unsigned stage = 0;
	for (unsigned i = 0; i < lines.size(); i++) {
		if (stage == 0) {
			if (lines[i][0] == 'd' && lines[i][4] == 'z') {
				temp.c = std::stoi(lines[i].substr(6));
				stage++;
			}
		} else if (stage == 1) {
			if (lines[i][0] == 'a' && lines[i][4] == 'x' && lines[i - 1][0] == 'd') {
				temp.a = std::stoi(lines[i].substr(6));
				stage++;
			}
		} else if (stage == 2) {
			if (lines[i][0] == 'a' && lines[i][4] == 'y' && lines[i - 1][0] == 'a') {
				temp.b = std::stoi(lines[i].substr(6));
				stage = 0;
				ret.push_back(temp);
			}
		}
	}

	return ret;
}

static auto backward(Vals v, long z2, long w) {
	std::vector<long> zs;
	zs.reserve(2);
	auto x = z2 - w - v.b;
	if (x % 26 == 0) {
		zs.push_back(x / 26 * v.c);
	}
	if (0 <= w - v.a && w - v.a < 26) {
		zs.push_back(w - v.a + (z2 * v.c));
	}
	return zs;
}

static auto solve(const std::vector<Vals> &monad, bool flip = false) {
	std::unordered_map<long, std::vector<long>> result;

	std::unordered_set<long> zs{0};
	std::array<long, 9> ws{1, 2, 3, 4, 5, 6, 7, 8, 9};
	if (flip) {
		std::reverse(ws.begin(), ws.end());
	}

	for (auto v : monad) {
		decltype(zs) newzs;
		for (auto w : ws) {
			for (auto z : zs) {
				auto z0s = backward(v, z, w);
				for (auto z0 : z0s) {
					newzs.insert(z0);
					std::vector<long> t = result[z];
					t.insert(t.begin(), w);

					result[z0] = t;
				}
			}
		}
		zs = newzs;
	}

	long out = 0;
	for (auto d : result[0]) {
		out = out * 10 + d;
	}

	return out;
}

static auto Part1(const std::vector<Vals> &monad) {
	return solve(monad);
}

static auto Part2(const std::vector<Vals> &monad) {
	return solve(monad, true);
}

int main(int argc, char **argv) {
	if (auto input = aoc::get_input("inputs/2021/Day24.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		auto ins = parse(lines);
		std::reverse(ins.begin(), ins.end());

		std::cout << "Part 1: " << Part1(ins) << std::endl;
		std::cout << "Part 2: " << Part2(ins) << std::endl;
	} else {
		return 1;
	}
}
