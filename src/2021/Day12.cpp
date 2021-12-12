// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using conns = std::unordered_map<std::string, std::unordered_set<std::string>>;

static conns parse(const std::vector<std::string> &input) {
	std::vector<std::pair<std::string, std::string>> pairs;
	for (auto line : input) {
		auto sides = aoc::by_line(line, '-');
		pairs.push_back({sides[0], sides[1]});
	}

	conns connections;

	for (auto pair : pairs) {
		connections[pair.first].emplace(pair.second);
		connections[pair.second].emplace(pair.first);
	}

	return connections;
}

static bool isupper(const std::string &str) {
	for (auto chr : str) {
		if (std::islower(chr)) {
			return false;
		}
	}
	return true;
}

static bool islower(const std::string &str) {
	for (auto chr : str) {
		if (std::isupper(chr)) {
			return false;
		}
	}
	return true;
}

static unsigned Part1(const std::vector<std::string> &input) {
	std::function<unsigned(std::vector<std::string> current, conns & connections)> recurse =
		[&](std::vector<std::string> current, conns &connections) {
			unsigned amount = 0;

			for (auto connection : connections[current.back()]) {
				if (connection == "end") {
					amount++;
				} else if (isupper(connection) ||
						   (islower(connection) &&
							std::find(current.begin(), current.end(), connection) == current.end())) {
					auto newer = current;
					newer.push_back(connection);
					amount += recurse(newer, connections);
				}
			}

			return amount;
		};

	auto connections = parse(input);
	return recurse({"start"}, connections);
}

static bool lowercase_repeats(const std::vector<std::string> &current) {
	std::vector<std::string> lowercase;
	for (auto visited : current) {
		if (islower(visited)) {
			lowercase.push_back(visited);
		}
	}

	std::unordered_set<std::string> set(lowercase.begin(), lowercase.end());

	return set.size() != lowercase.size();
}

static unsigned Part2(const std::vector<std::string> &input) {
	std::function<unsigned(std::vector<std::string> current, conns & connections)> recurse =
		[&](std::vector<std::string> current, conns &connections) {
			bool visited_twice = lowercase_repeats(current);

			unsigned amount = 0;

			for (auto connection : connections[current.back()]) {
				if (connection == "end") {
					amount++;
				} else if ((connection != "start") &&
						   (isupper(connection) ||
							(islower(connection) &&
							 (!visited_twice || std::find(current.begin(), current.end(), connection) ==
													current.end())))) {
					auto newer = current;
					newer.push_back(connection);
					amount += recurse(newer, connections);
				}
			}

			return amount;
		};

	auto connections = parse(input);
	return recurse({"start"}, connections);
}

extern std::vector<std::string> test_input_1;
extern std::vector<std::string> test_input_2;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input_1) == 10, "Part 1 test input 1");
	aoc::test(Part1(test_input_2) == 226, "Part 1 test input 2");
	aoc::test(Part2(test_input_1) == 36, "Part 2 test input 1");
	aoc::test(Part2(test_input_2) == 3509, "Part 2 test input 2");

	if (auto input = aoc::get_input("inputs/2021/Day12.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(lines) << std::endl;
		std::cout << "Part 2: " << Part2(lines) << std::endl;
	} else {
		return 1;
	}
}

std::vector<std::string> test_input_1 = {"start-A", "start-b", "A-c",  "A-b",
										 "b-d",		"A-end",   "b-end"};
std::vector<std::string> test_input_2 = {
	"fs-end", "he-DX", "fs-he", "start-DX", "pj-DX",	"end-zg", "zg-sl", "zg-pj", "pj-he",
	"RW-he",  "fs-DX", "pj-RW", "zg-RW",	"start-pj", "he-WI",  "zg-he", "pj-fs", "start-RW"};
