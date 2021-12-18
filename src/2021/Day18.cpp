// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

static auto parse(const std::string &s) {
	std::vector<std::string> out;
	out.reserve(s.size());

	for (auto chr : s) {
		if (chr != ',') {
			out.push_back(std::string() + chr);
		}
	}

	return out;
}

static std::vector<std::string> add(const std::vector<std::string> &lhs, const std::vector<std::string> &rhs) {
	std::vector<std::string> out;

	out.reserve(lhs.size() + rhs.size() + 2);
	out.push_back("[");
	out.insert(out.end(), lhs.begin(), lhs.end());
	out.insert(out.end(), rhs.begin(), rhs.end());
	out.push_back("]");

	return out;
}

static auto sum_magnitudes(const std::vector<std::string> &lhs) {
	std::vector<unsigned long> stack;

	for (auto &s : lhs) {
		if (s == "[") {
			continue;
		} else if (s == "]") {
			auto b = stack.back();
			stack.pop_back();
			auto a = stack.back();
			stack.pop_back();

			stack.push_back(3 * a + 2 * b);
		} else {
			stack.push_back(static_cast<unsigned long>(std::stoi(s)));
		}
	}

	return stack.back();
}

static auto single_reduce(std::vector<std::string> &lhs) {
	unsigned depth = 0;
	unsigned last_digit_index = 999999999;

	// explode
	for (unsigned i = 0; i < lhs.size(); i++) {
		if (lhs[i] == "[") {
			depth++;
		} else if (lhs[i] == "]") {
			depth--;
		} else {
			if (depth > 4) {
				if (last_digit_index < 99999999) {
					lhs[last_digit_index] =
						std::to_string(std::stoi(lhs[last_digit_index]) + std::stoi(lhs[i]));
				}

				for (unsigned j = i + 2; j < lhs.size(); j++) {
					if (std::isdigit(lhs[j][0])) {
						lhs[j] = std::to_string(std::stoi(lhs[i + 1]) + std::stoi(lhs[j]));
						break;
					}
				}

				std::vector<std::string> ret;
				ret.reserve(lhs.size() + 1);
				ret.insert(ret.end(), lhs.begin(), lhs.begin() + i - 1);
				ret.insert(ret.end(), "0");
				ret.insert(ret.end(), lhs.begin() + i + 3, lhs.end());
				return std::pair{true, ret};
			} else {
				last_digit_index = i;
			}
		}
	}

	// split
	for (unsigned i = 0; i < lhs.size(); i++) {
		if (lhs[i] == "[") {
			depth++;
		} else if (lhs[i] == "]") {
			depth--;
		} else {
			unsigned num = static_cast<unsigned>(std::stoi(lhs[i]));
			if (num > 9) {
				std::vector<std::string> ret;
				ret.reserve(lhs.size() + 4);
				ret.insert(ret.end(), lhs.begin(), lhs.begin() + i);
				ret.insert(ret.end(),
						   {"[", std::to_string(num / 2), std::to_string((num + 1) / 2), "]"});
				ret.insert(ret.end(), lhs.begin() + i + 1, lhs.end());
				return std::pair{true, ret};
			}
		}
	}

	return std::pair{false, lhs};
}

static auto reduce(const std::vector<std::string> &lhs) {
	bool edited = true;
	auto line = lhs;
	while (edited) {
		auto [cont, line_] = single_reduce(line);
		line = std::move(line_);
		edited = cont;
	}
	return line;
}

static auto Part1(const std::vector<std::string> &input) {
	auto total = parse(input[0]);

	for (unsigned i = 1; i < input.size(); i++) {
		total = reduce(add(total, parse(input[i])));
	}

	return sum_magnitudes(total);
}

static auto Part2(const std::vector<std::string> &input) {
	unsigned long max = 0;

	for (unsigned i = 0; i < input.size(); i++) {
		for (unsigned j = 0; j < input.size(); j++) {
			auto result = sum_magnitudes(reduce(add(parse(input[i]), parse(input[j]))));

			if (result > max) {
				max = result;
			}
		}
	}

	return max;
}

int main(int argc, char **argv) {
	aoc::test(Part1({"[9,1]", "[1,9]"}) == 129, "Part 1 test input");
	// aoc::test(Part2("C200B40A82") == 3, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day18.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(lines) << std::endl;
		std::cout << "Part 2: " << Part2(lines) << std::endl;
	} else {
		return 1;
	}
}
