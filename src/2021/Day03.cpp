// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <iostream>
#include <sstream>

static int Part1(const std::vector<std::string> &input) {
	std::vector<int> sum_columns;
	unsigned width = static_cast<unsigned>(input[0].size());
	int height = static_cast<int>(input.size());
	sum_columns.resize(width, 0);

	for (auto line : input) {
		for (unsigned i = 0; i < line.size(); i++) {
			if (line[i] == '1') {
				sum_columns[i]++;
			}
		}
	}

	std::string most_common;
	std::string least_common;
	for (unsigned i = 0; i < sum_columns.size(); i++) {
		if (height - sum_columns[i] > sum_columns[i]) {
			most_common.push_back('0');
			least_common.push_back('1');
		} else {
			most_common.push_back('1');
			least_common.push_back('0');
		}
	}

	int gamma_rate = std::stoi(most_common, nullptr, 2);
	int epsilon_rate = std::stoi(least_common, nullptr, 2);

	return gamma_rate * epsilon_rate;
}


int main(int argc, char **argv) {
	std::vector<std::string> test_input = {
	  "00100", "11110", "10110", "10111", "10101", "01111", "00111", "11100", "10000", "11001", "00010", "01010"};
	aoc::test(Part1(test_input) == 198, "Part 1 test input");
	// aoc::test(Part2(test_input) == 900, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day03.txt", argc, argv)) {
		auto formatted = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		// std::cout << "Part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}
