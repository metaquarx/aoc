// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <iostream>

static int Part1(const std::vector<std::string> &input) {
	int horizontal = 0;
	int depth = 0;

	for (auto line : input) {
		auto offset = std::stoi(line.substr(line.find_last_not_of("0123456789") + 1));

		if (line.rfind("forward", 0) != std::string::npos) {
			depth += offset;
		} else if (line.rfind("down", 0) != std::string::npos) {
			horizontal += offset;
		} else if (line.rfind("up", 0) != std::string::npos) {
			horizontal -= offset;
		}
	}

	return horizontal * depth;
}

static int Part2(const std::vector<std::string> &input) {
	int horizontal = 0;
	int depth = 0;
	int aim = 0;

	for (auto line : input) {
		auto value = std::stoi(line.substr(line.find_last_not_of("0123456789") + 1));

		if (line.rfind("up", 0) != std::string::npos) {
			aim -= value;
		} else if (line.rfind("down", 0) != std::string::npos) {
			aim += value;
		} else if (line.rfind("forward", 0) != std::string::npos) {
			horizontal += value;
			depth += aim * value;
		}
	}

	return horizontal * depth;
}

int main(int argc, char **argv) {
	std::vector<std::string> test_input = {"forward 5", "down 5", "forward 8", "up 3", "down 8", "forward 2"};
	aoc::test(Part1(test_input) == 150, "Part 1 test input");
	aoc::test(Part2(test_input) == 900, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day02.txt", argc, argv)) {
		auto formatted = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}
