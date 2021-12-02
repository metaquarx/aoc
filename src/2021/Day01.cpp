// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <iostream>

static int Part1(const std::vector<int> &input) {
	int counter = 0;

	int last = input[0];
	for (unsigned i = 1; i != input.size(); i++) {
		int current = input[i];

		if (current > last) {
			counter++;
		}

		last = current;
	}

	return counter;
}

static int Part2(const std::vector<int> &input) {
	auto count_window = [&input](unsigned index) { return input[index] + input[index + 1] + input[index + 2]; };
	int counter = 0;

	int last = count_window(0);
	for (unsigned i = 1; i != input.size() - 2; i++) {
		int current = count_window(i);

		if (current > last) {
			counter++;
		}

		last = current;
	}

	return counter;
}

int main(int argc, char **argv) {
	std::vector<int> test_input = {199, 200, 208, 210, 200, 207, 240, 269, 260, 263};
	aoc::test(Part1(test_input) == 7, "Part 1 test input");
	aoc::test(Part2(test_input) == 5, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day01.txt", argc, argv)) {
		auto formatted = aoc::to_int(aoc::by_line(*input));

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "Part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}
