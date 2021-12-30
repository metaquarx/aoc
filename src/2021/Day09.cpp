// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <iostream>

static unsigned Part1(const std::vector<std::vector<unsigned>> &input) {
	unsigned lowpoints_total = 0;

	for (unsigned y = 0; y < input.size(); y++) {
		for (unsigned x = 0; x < input[y].size(); x++) {
			// is lowpoint
			auto current = input[y][x];

			if (y >= 1 && input[y - 1][x] <= current) {	 // up
				continue;
			}

			if (y <= input.size() - 2 && input[y + 1][x] <= current) {	// down
				continue;
			}

			if (x >= 1 && input[y][x - 1] <= current) {	 // left
				continue;
			}

			if (x <= input[y].size() - 2 && input[y][x + 1] <= current) {  // right
				continue;
			}

			lowpoints_total += current + 1;
		}
	}

	return lowpoints_total;
}

static unsigned Part2(const std::vector<std::vector<unsigned>> &input) {
	auto adjacent = [&input](unsigned y, unsigned x) {
		std::vector<std::pair<unsigned, unsigned>> adj;

		if (y >= 1) {  // up
			adj.push_back({y - 1, x});
		}
		if (x >= 1) {  // left
			adj.push_back({y, x - 1});
		}
		if (y <= input.size() - 2) {  // down
			adj.push_back({y + 1, x});
		}
		if (x <= input[y].size() - 2) {	 // right
			adj.push_back({y, x + 1});
		}

		return adj;
	};

	std::vector<std::pair<unsigned, unsigned>> explored;
	auto basin_size = [&explored, &input, &adjacent](unsigned y, unsigned x) {
		std::vector<std::pair<unsigned, unsigned>> stack = {{y, x}};
		unsigned size = 0;
		while (!stack.empty()) {
			auto i = stack.back().first;
			auto j = stack.back().second;
			stack.pop_back();

			// explored
			if (std::find(explored.begin(), explored.end(), std::pair{i, j}) != explored.end()) {
				continue;
			}

			// too high
			if (input[i][j] == 9) {
				continue;
			}

			auto adj = adjacent(i, j);

			stack.insert(stack.end(), adj.begin(), adj.end());
			explored.push_back({i, j});
			size++;
		}

		return size;
	};

	std::vector<unsigned> basins;

	for (unsigned y = 0; y < input.size(); y++) {
		for (unsigned x = 0; x < input[y].size(); x++) {
			if (input[y][x] != 9) {
				basins.push_back(basin_size(y, x));
			}
		}
	}

	std::sort(basins.begin(), basins.end());
	//
	return basins[basins.size() - 1] * basins[basins.size() - 2] * basins[basins.size() - 3];
}

extern std::vector<std::vector<unsigned>> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input) == 15, "Part 1 test input");
	aoc::test(Part2(test_input) == 1134, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day09.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);
		std::vector<std::vector<unsigned>> rows;
		for (auto line : lines) {
			std::vector<unsigned> temp;
			for (auto chr : line) {
				temp.push_back(static_cast<unsigned>(std::stoi(std::string(1, chr))));
			}
			rows.push_back(temp);
		}

		std::cout << "Part 1: " << Part1(rows) << std::endl;
		std::cout << "Part 2: " << Part2(rows) << std::endl;
	} else {
		return 1;
	}
}

std::vector<std::vector<unsigned>> test_input = {{2, 1, 9, 9, 9, 4, 3, 2, 1, 0},
												 {3, 9, 8, 7, 8, 9, 4, 9, 2, 1},
												 {9, 8, 5, 6, 7, 8, 9, 8, 9, 2},
												 {8, 7, 6, 7, 8, 9, 6, 7, 8, 9},
												 {9, 8, 9, 9, 9, 6, 5, 6, 7, 8}};
