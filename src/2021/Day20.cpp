// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

struct Map {
	std::unordered_map<std::pair<int, int>, bool> grid;
	int min;
	int max;
	bool background = false;

	auto operator[](std::pair<int, int> c) {
		grid.try_emplace(c, background);

		return grid[c];
	}
};

static auto parse(const std::vector<std::string> &input) {
	std::string algo = input[0];

	Map result;

	result.min = 0;
	result.max = input[2].size();

	for (unsigned l = 2; l < input.size(); l++) {
		for (unsigned x = 0; x < input[l].size(); x++) {
			unsigned y = l - 2;
			result.grid[{x, y}] = input[l][x] == '#';
		}
	}

	return std::pair{algo, result};
}

static auto enhance(const std::string &algo, Map map) {
	auto newer = map;
	newer.min--;
	newer.max++;
	newer.background = map.background ? algo[511] == '#' : algo[0] == '#';

	auto near = [](Map &mp, int x, int y) {
		unsigned idx = 0;

		idx |= static_cast<unsigned>(mp[{x - 1, y - 1}]) << 8;
		idx |= static_cast<unsigned>(mp[{x + 0, y - 1}]) << 7;
		idx |= static_cast<unsigned>(mp[{x + 1, y - 1}]) << 6;

		idx |= static_cast<unsigned>(mp[{x - 1, y + 0}]) << 5;
		idx |= static_cast<unsigned>(mp[{x + 0, y + 0}]) << 4;
		idx |= static_cast<unsigned>(mp[{x + 1, y + 0}]) << 3;

		idx |= static_cast<unsigned>(mp[{x - 1, y + 1}]) << 2;
		idx |= static_cast<unsigned>(mp[{x + 0, y + 1}]) << 1;
		idx |= static_cast<unsigned>(mp[{x + 1, y + 1}]) << 0;

		return idx;
	};

	for (int y = newer.min; y < newer.max; y++) {
		for (int x = newer.min; x < newer.max; x++) {
			newer.grid[{x, y}] = algo[near(map, x, y)] == '#';
		}
	}

	return newer;
}

static auto count_light(const Map &map) {
	unsigned total = 0;

	for (auto &[key, val] : map.grid) {
		if (val) {
			total++;
		}
	}

	return total;
}

static auto Part1(const std::string &algo, const Map &map) {
	auto updated = enhance(algo, map);
	updated = enhance(algo, updated);
	return count_light(updated);
}

static auto Part2(const std::string &algo, const Map &map) {
	auto result = map;

	for (auto i = 0; i < 50; i++) {
		result = enhance(algo, result);
	}

	return count_light(result);
}

int main(int argc, char **argv) {
	if (auto input = aoc::get_input("inputs/2021/Day20.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		auto [algo, map] = parse(lines);

		std::cout << "Part 1: " << Part1(algo, map) << std::endl;
		std::cout << "Part 2: " << Part2(algo, map) << std::endl;
	} else {
		return 1;
	}
}
