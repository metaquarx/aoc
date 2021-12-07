// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

static int Part1(const std::vector<int> &input) {
	int min = *std::min_element(input.begin(), input.end());
	int max = *std::max_element(input.begin(), input.end());

	std::unordered_map<int, int> options;  // target position -> total fuel used

	for (int i = min; i <= max; i++) {
		int fuel_used = 0;
		for (auto crab : input) {
			fuel_used += std::abs(crab - i);
		}
		options[i] = fuel_used;
	}

	int minimum_element = 9999999;
	for (auto option : options) {
		if (option.second < minimum_element) {
			minimum_element = option.second;
		}
	}

	return minimum_element;
}

static int Part2(const std::vector<int> &input) {
	int min = *std::min_element(input.begin(), input.end());
	int max = *std::max_element(input.begin(), input.end());

	std::unordered_map<int, int> options;  // target position -> total fuel used

	for (int i = min; i <= max; i++) {
		int fuel_used = 0;
		for (auto crab : input) {
			fuel_used += (std::abs(crab - i) * std::abs(crab - i) + std::abs(crab - i)) / 2;
		}
		options[i] = fuel_used;
	}

	int minimum_element = 999999999;
	for (auto option : options) {
		if (option.second < minimum_element) {
			minimum_element = option.second;
		}
	}

	return minimum_element;
}

extern std::vector<int> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input) == 37, "Part 1 test input");
	aoc::test(Part2(test_input) == 168, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day07.txt", argc, argv)) {
		auto formatted = aoc::to_int(aoc::by_line(*input, ','));

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "Part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}

std::vector<int> test_input = {16, 1, 2, 0, 4, 2, 7, 1, 2, 14};
