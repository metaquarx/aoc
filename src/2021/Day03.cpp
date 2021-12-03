// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <iostream>
#include <sstream>

static char get_most_common(const std::vector<std::string> &read, unsigned index, char bias) {
	int ones = 0;

	for (auto line : read) {
		if (line[index] == '1') {
			ones++;
		}
	}

	if (static_cast<int>(read.size()) - ones == ones) {
		return bias;
	} else if (static_cast<int>(read.size()) - ones < ones) {
		return '1';
	} else {
		return '0';
	}
};

static char get_least_common(const std::vector<std::string> &read, unsigned index, char bias = '1') {
	if (get_most_common(read, index, bias) == '0') {
		return '1';
	} else {
		return '0';
	}
};

static int Part1(const std::vector<std::string> &input) {
	std::string most_common;
	std::string least_common;

	auto width = input[0].size();

	for (unsigned i = 0; i < width; i++) {
		most_common.push_back(get_most_common(input, i, '0'));
		least_common.push_back(get_least_common(input, i));
	}

	int gamma_rate = std::stoi(most_common, nullptr, 2);
	int epsilon_rate = std::stoi(least_common, nullptr, 2);

	return gamma_rate * epsilon_rate;
}

static int Part2(const std::vector<std::string> &input) {
	auto keep_most = [&input](char (*op)(const std::vector<std::string> &, unsigned, char bias), char bias) {
		std::vector<std::string> out(input);

		for (unsigned i = 0; i < input[0].size(); i++) {
			auto side = op(out, i, bias);

			for (auto it = out.begin(); it < out.end();) {
				if ((*it)[i] != side) {
					it = out.erase(it);
				} else {
					it++;
				}
			}

			if (out.size() > 1) {
				continue;
			} else {
				break;
			}
		}

		return out.front();
	};

	auto most_common = keep_most(get_most_common, '1');
	auto least_common = keep_most(get_least_common, '1');

	return std::stoi(most_common, nullptr, 2) * std::stoi(least_common, nullptr, 2);
}

int main(int argc, char **argv) {
	std::vector<std::string> test_input = {
	  "00100", "11110", "10110", "10111", "10101", "01111", "00111", "11100", "10000", "11001", "00010", "01010"};
	aoc::test(Part1(test_input) == 198, "Part 1 test input");
	aoc::test(Part2(test_input) == 230, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day03.txt", argc, argv)) {
		auto formatted = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "Part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}
