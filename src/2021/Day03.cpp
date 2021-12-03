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

static int Part2(const std::vector<std::string> &input) {
	auto get_most_common = [](const std::vector<std::string> &read, unsigned index, char bias) {
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

	auto get_least_common = [&get_most_common](const std::vector<std::string> &read, unsigned index) {
		if (get_most_common(read, index, '1') == '0') {
			return '1';
		} else {
			return '0';
		}
	};

	std::vector<std::string> keep_most_common(input);

	for (unsigned i = 0; i < input[0].size(); i++) {
		auto most_common = get_most_common(keep_most_common, i, '1');

		for (auto it = keep_most_common.begin(); it < keep_most_common.end();) {
			if ((*it)[i] != most_common) {
				it = keep_most_common.erase(it);
			} else {
				it++;
			}
		}

		if (keep_most_common.size() > 1) {
			continue;
		} else {
			break;
		}
	}

	std::vector<std::string> keep_least_common(input);

	for (unsigned i = 0; i < input[0].size(); i++) {
		auto least_common = get_least_common(keep_least_common, i);

		for (auto it = keep_least_common.begin(); it < keep_least_common.end();) {
			if ((*it)[i] != least_common) {
				it = keep_least_common.erase(it);
			} else {
				it++;
			}
		}

		if (keep_least_common.size() > 1) {
			continue;
		} else {
			break;
		}
	}

	return std::stoi(keep_most_common.front(), nullptr, 2) * std::stoi(keep_least_common.front(), nullptr, 2);
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
