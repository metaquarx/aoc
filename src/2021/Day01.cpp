// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <iostream>

static int Part1(const std::vector<int> &input) {
	int times_incremented = 0;
	int last_distance = 0;

	for (auto distance : input) {
		if (last_distance != 0) {
			if (distance > last_distance) {
				times_incremented++;
			}
		}

		last_distance = distance;
	}

	return times_incremented;
}

static int Part2(const std::vector<int> &input) {
	int times_incremented = 0;
	int last_distance = input[0] + input[1] + input[2];

	for (unsigned i = 3; i < input.size(); i++) {
		int current_window_distance = input[i] + input[i - 1] + input[i - 2];

		if (current_window_distance > last_distance) {
			times_incremented++;
		}

		last_distance = current_window_distance;
	}

	return times_incremented;
}

int main(int argc, char **argv) {
	std::vector<int> test_input = {199, 200, 208, 210, 200, 207, 240, 269, 260, 263};
	aoc::test(Part1(test_input) == 7, "Part 1 test input");
	aoc::test(Part2(test_input) == 5, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day01.txt", argc, argv)) {
		auto formatted = aoc::to_int(aoc::by_line(*input));

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}
