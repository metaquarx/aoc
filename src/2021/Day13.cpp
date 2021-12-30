// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>

static auto fold(const std::set<std::pair<int, int>> &points, char axis, int fold) {
	std::set<std::pair<int, int>> ret;

	for (auto &point : points) {
		if (axis == 'x') {
			ret.insert({fold - std::abs(point.first - fold), point.second});
		} else {
			ret.insert({point.first, fold - std::abs(static_cast<long>(point.second - fold))});
		}
	}

	return ret;
}

static auto Part1(const std::set<std::pair<int, int>> points, const std::vector<std::pair<char, int>> &folds) {
	return fold(points, folds[0].first, folds[0].second).size();
}

static auto Part2(const std::set<std::pair<int, int>> points, const std::vector<std::pair<char, int>> &folds) {
	std::array<unsigned, 2> max = {std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::max()};

	auto working = points;

	for (auto &item : folds) {
		working = fold(working, item.first, item.second);
		max[static_cast<unsigned>(item.first - 'x')] =
			std::min(max[static_cast<unsigned>(item.first - 'x')], static_cast<unsigned>(item.second));
	}

	for (unsigned y = 0; y < max[1]; y++) {
		for (unsigned x = 0; x < max[0]; x++) {
			std::cout << (working.count({x, y}) ? '#' : ' ');
		}
		std::cout << std::endl;
	}

	return 0;
}

extern std::set<std::pair<int, int>> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input, {{'y', 7}, {'x', 5}}) == 17, "Part 1 test input");

	if (auto input = aoc::get_input("inputs/2021/Day13.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		std::set<std::pair<int, int>> points;
		std::vector<std::pair<char, int>> folds;

		for (auto &line : lines) {
			auto sides = aoc::by_line(line, ',');
			if (sides.size() > 1) {
				points.insert({std::stoi(sides[0]), std::stoi(sides[1])});
			} else if (line.size() > 4) {
				auto eq = aoc::by_line(line, '=');
				auto letter = aoc::by_line(eq[0], ' ');
				folds.push_back({letter[2][0], std::stoi(eq[1])});
			}
		}

		std::cout << "Part 1: " << Part1(points, folds) << std::endl;
		Part2(points, folds);
	} else {
		return 1;
	}
}

std::set<std::pair<int, int>> test_input = {{{6, 10},
											 {0, 14},
											 {9, 10},
											 {0, 3},
											 {10, 4},
											 {4, 11},
											 {6, 0},
											 {6, 12},
											 {4, 1},
											 {0, 13},
											 {10, 12},
											 {3, 4},
											 {3, 0},
											 {8, 4},
											 {1, 10},
											 {2, 14},
											 {8, 10},
											 {9, 0}}};
