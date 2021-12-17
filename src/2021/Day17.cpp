// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <cstdio>
#include <iostream>

struct Vector {
	int x;
	int y;
};

static auto parse(const std::string &input) {
	Vector max;
	Vector min;
	std::sscanf(input.c_str(), "target area: x=%i..%i, y=%i..%i", &min.x, &max.x, &min.y, &max.y);
	return std::pair{min, max};
}

static auto Part1(const std::string &input) {
	auto [t_min, t_max] = parse(input);

	return (t_min.y * (t_min.y + 1)) / 2;
}

static auto Part2(const std::string &input) {
	auto [t_min, t_max] = parse(input);

	unsigned total = 0;

	for (int x = 10; x < 300; x++) {
		for (int y = -100; y < 200; y++) {
			Vector t = {x, y};
			Vector c = {0, 0};

			auto hit = false;
			auto max = -100;

			auto step = 0;
			while (c.x <= t_max.x && c.y >= t_min.y) {
				step++;
				c.x += t.x;
				t.x = std::max(t.x - 1, 0);

				c.y += t.y;
				t.y = t.y - 1;

				max = std::max(c.y, max);

				if (c.x >= t_min.x && c.x <= t_max.x && c.y >= t_min.y && c.y <= t_max.y) {
					hit = true;
					break;
				}
			}

			if (hit) {
				total++;
			}
		}
	}
	return total;
}

int main(int argc, char **argv) {
	if (auto input = aoc::get_input("inputs/2021/Day17.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(lines[0]) << std::endl;
		std::cout << "Part 2: " << Part2(lines[0]) << std::endl;
	} else {
		return 1;
	}
}
