// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <array>
#include <iostream>
#include <map>
#include <unordered_map>

static auto Part1(const std::vector<std::vector<int>> &input) {
	int max_y = input.size();
	int max_x = input[0].size();

	std::unordered_map<std::pair<int, int>, int> costs;
	std::multimap<int, std::pair<int, int>> visit;
	visit.emplace(0, std::pair{0, 0});

	while (!visit.empty()) {
		auto it = visit.begin();
		auto cost = it->first;
		auto x = it->second.second;
		auto y = it->second.first;
		visit.erase(it);

		if (!(costs.emplace(std::pair{y, x}, cost).second)) {
			continue;
		}

		std::array<std::array<int, 2>, 4> dx{{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};
		for (auto &dxy : dx) {
			if (dxy[0] + x < 0 || dxy[0] + x >= max_x) {
				continue;
			}

			if (dxy[1] + y < 0 || dxy[1] + y >= max_y) {
				continue;
			}

			visit.emplace(cost + input[dxy[1] + y][dxy[0] + x], std::pair{dxy[1] + y, dxy[0] + x});
		}
	}

	return costs.at(std::pair{max_y - 1, max_x - 1});
}

static auto Part2(const std::vector<std::vector<int>> &input) {
	auto normalise = [](int lhs) { return ((lhs - 1) % 9) + 1; };

	std::vector<std::vector<int>> right;
	right.reserve(input.size());

	for (int y = 0; y < input.size(); y++) {
		std::vector<int> temp;
		temp.reserve(input[y].size() * 5);

		for (int r = 0; r < 5; r++) {
			for (int x = 0; x < input[y].size(); x++) {
				temp.push_back(normalise(input[y][x] + r));
			}
		}
		right.push_back(temp);
	}

	std::vector<std::vector<int>> down;
	down.reserve(input.size() * 5);

	for (int r = 0; r < 5; r++) {
		for (auto &y : right) {
			std::vector<int> temp;
			temp.reserve(y.size());

			for (auto &x : y) {
				temp.push_back(normalise(x + r));
			}

			down.push_back(temp);
		}
	}
	return Part1(down);
}

extern std::vector<std::vector<int>> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input) == 40, "Part 1 test input");
	aoc::test(Part2(test_input) == 315, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day15.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);
		std::vector<std::vector<int>> formatted;
		formatted.reserve(lines.size());
		for (auto &line : lines) {
			std::vector<int> temp;
			temp.reserve(line.size());
			for (auto chr : line) {
				temp.push_back(std::stoi(std::string() + chr));
			}
			formatted.push_back(temp);
		}

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "Part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}

std::vector<std::vector<int>> test_input = {
	{1, 1, 6, 3, 7, 5, 1, 7, 4, 2}, {1, 3, 8, 1, 3, 7, 3, 6, 7, 2}, {2, 1, 3, 6, 5, 1, 1, 3, 2, 8},
	{3, 6, 9, 4, 9, 3, 1, 5, 6, 9}, {7, 4, 6, 3, 4, 1, 7, 1, 1, 1}, {1, 3, 1, 9, 1, 2, 8, 1, 3, 7},
	{1, 3, 5, 9, 9, 1, 2, 4, 2, 1}, {3, 1, 2, 5, 4, 2, 1, 6, 3, 9}, {1, 2, 9, 3, 1, 3, 8, 5, 2, 1},
	{2, 3, 1, 1, 9, 4, 4, 5, 8, 1}};
