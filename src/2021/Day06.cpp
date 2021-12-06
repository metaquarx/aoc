// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <iostream>
#include <numeric>

static unsigned Part1(const std::vector<int> &input) {
	std::vector<int> timer;	 // age -> quantity
	timer.resize(9, 0);
	for (auto in : input) {
		timer[static_cast<unsigned>(in)]++;
	}

	for (int i = 0; i < 80; i++) {
		std::vector<int> updated;
		updated.resize(9, 0);

		updated[0] = timer[1];
		updated[1] = timer[2];
		updated[2] = timer[3];
		updated[3] = timer[4];
		updated[4] = timer[5];
		updated[5] = timer[6];
		updated[6] = timer[7] + timer[0];
		updated[7] = timer[8];
		updated[8] = timer[0];

		std::swap(timer, updated);
	}

	return std::accumulate(timer.begin(), timer.end(), 0u);
}

static unsigned long Part2(const std::vector<int> &input) {
	std::vector<unsigned long> timer;  // age -> quantity
	timer.resize(9, 0);
	for (auto in : input) {
		timer[static_cast<unsigned>(in)]++;
	}

	for (int i = 0; i < 256; i++) {
		std::vector<unsigned long> updated;
		updated.resize(9, 0);

		updated[0] = timer[1];
		updated[1] = timer[2];
		updated[2] = timer[3];
		updated[3] = timer[4];
		updated[4] = timer[5];
		updated[5] = timer[6];
		updated[6] = timer[7] + timer[0];
		updated[7] = timer[8];
		updated[8] = timer[0];

		std::swap(timer, updated);
	}

	return std::accumulate(timer.begin(), timer.end(), 0ul);
}

extern std::vector<int> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input) == 5934, "Part 1 test input");
	aoc::test(Part2(test_input) == 26984457539, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day06.txt", argc, argv)) {
		auto formatted = aoc::to_int(aoc::by_line(*input, ','));

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "Part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}

std::vector<int> test_input = {3, 4, 3, 1, 2};
