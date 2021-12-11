// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <iostream>

static void flash(unsigned y, unsigned x, std::vector<std::vector<unsigned>> &input) {
	auto top = [&]() { return y == 0; };
	auto bottom = [&]() { return y == input.size() - 1; };
	auto left = [&]() { return x == 0; };
	auto right = [&]() { return x == input[0].size() - 1; };

	if (!top() && !left()) {
		input[y - 1][x - 1]++;
	}
	if (!top()) {
		input[y - 1][x]++;
	}
	if (!top() && !right()) {
		input[y - 1][x + 1]++;
	}
	if (!left()) {
		input[y][x - 1]++;
	}
	if (!right()) {
		input[y][x + 1]++;
	}
	if (!bottom() && !left()) {
		input[y + 1][x - 1]++;
	}
	if (!bottom()) {
		input[y + 1][x]++;
	}
	if (!bottom() && !right()) {
		input[y + 1][x + 1]++;
	}
}

static unsigned Part1(const std::vector<std::vector<unsigned>> &input) {
	auto old = input;
	unsigned flashes = 0;

	for (unsigned i = 0; i < 100; i++) {
		auto newer = old;

		// increase energy level of each
		for (unsigned y = 0; y < newer.size(); y++) {
			for (unsigned x = 0; x < newer[y].size(); x++) {
				newer[y][x]++;
			}
		}

		std::vector<std::pair<unsigned, unsigned>> already_flashed;

		// flashing
		bool keep_going = true;
		while (keep_going) {
			keep_going = false;
			for (unsigned y = 0; y < newer.size(); y++) {
				for (unsigned x = 0; x < newer[y].size(); x++) {
					if (newer[y][x] > 9 && std::find(already_flashed.begin(), already_flashed.end(),
													 std::pair(y, x)) == already_flashed.end()) {
						flash(y, x, newer);
						keep_going = true;
						already_flashed.push_back(std::pair(y, x));
					}
				}
			}
		}

		// reset
		for (auto pair : already_flashed) {
			newer[pair.first][pair.second] = 0;
		}

		flashes += already_flashed.size();

		std::swap(old, newer);
	}

	return flashes;
}

static unsigned long Part2(const std::vector<std::vector<unsigned>> &input) {
	auto old = input;

	for (unsigned i = 0; i < 99999; i++) {
		auto newer = old;

		// increase energy level of each
		for (unsigned y = 0; y < newer.size(); y++) {
			for (unsigned x = 0; x < newer[y].size(); x++) {
				newer[y][x]++;
			}
		}

		std::vector<std::pair<unsigned, unsigned>> already_flashed;

		// flashing
		bool keep_going = true;
		while (keep_going) {
			keep_going = false;
			for (unsigned y = 0; y < newer.size(); y++) {
				for (unsigned x = 0; x < newer[y].size(); x++) {
					if (newer[y][x] > 9 && std::find(already_flashed.begin(), already_flashed.end(),
													 std::pair(y, x)) == already_flashed.end()) {
						flash(y, x, newer);
						keep_going = true;
						already_flashed.push_back(std::pair(y, x));
					}
				}
			}
		}

		// reset
		for (auto pair : already_flashed) {
			newer[pair.first][pair.second] = 0;
		}

		if (already_flashed.size() == newer.size() * newer[0].size()) {
			return i + 1;
		}

		std::swap(old, newer);
	}
	return 0;
}

extern std::vector<std::vector<unsigned>> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input) == 1656, "Part 1 test input");
	aoc::test(Part2(test_input) == 195, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day11.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		std::vector<std::vector<unsigned>> rows;
		for (auto &line : lines) {
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

std::vector<std::vector<unsigned>> test_input = {
	{5, 4, 8, 3, 1, 4, 3, 2, 2, 3}, {2, 7, 4, 5, 8, 5, 4, 7, 1, 1}, {5, 2, 6, 4, 5, 5, 6, 1, 7, 3},
	{6, 1, 4, 1, 3, 3, 6, 1, 4, 6}, {6, 3, 5, 7, 3, 8, 5, 4, 7, 8}, {4, 1, 6, 7, 5, 2, 4, 6, 4, 5},
	{2, 1, 7, 6, 8, 4, 1, 7, 2, 1}, {6, 8, 8, 2, 8, 8, 1, 1, 3, 4}, {4, 8, 4, 6, 8, 4, 8, 5, 5, 4},
	{5, 2, 8, 3, 7, 5, 1, 5, 2, 6}};
