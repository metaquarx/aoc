// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>

static auto Part1(std::string templ, const std::unordered_map<std::string, std::string> &input) {
	for (unsigned step = 0; step < 10; step++) {
		for (unsigned i = 0; i < templ.size() - 1; i++) {
			std::string sequence = std::string() + templ[i] + templ[i + 1];
			if (input.find(sequence) != input.end()) {
				templ.insert(i + 1, std::string() + input.at(sequence));
			}
			i += 1;
		}
	}

	std::unordered_map<char, unsigned> counter;
	for (auto &chr : templ) {
		counter[chr]++;
	}

	unsigned max =
		std::max_element(counter.begin(), counter.end(),
						 [](const std::pair<char, unsigned> &lhs,
							const std::pair<char, unsigned> &rhs) { return lhs.second < rhs.second; })
			->second;
	unsigned min =
		std::min_element(counter.begin(), counter.end(),
						 [](const std::pair<char, unsigned> &lhs,
							const std::pair<char, unsigned> &rhs) { return lhs.second < rhs.second; })
			->second;
	return max - min;
}

static auto Part2(std::string templ, const std::unordered_map<std::string, std::string> &input) {
	std::map<std::pair<char, char>, unsigned long> pairs;

	for (unsigned i = 0; i < templ.size() - 1; i++) {
		pairs[{templ[i], templ[i + 1]}]++;
	}

	for (unsigned step = 0; step < 40; step++) {
		std::map<std::pair<char, char>, unsigned long> new_pairs;

		for (const auto &[pair, count] : pairs) {
			auto sequence = std::string() + pair.first + pair.second;
			if (input.find(sequence) != input.end()) {
				new_pairs[std::pair{pair.first, input.at(sequence)[0]}] += count;
				new_pairs[std::pair{input.at(sequence)[0], pair.second}] += count;
			} else {
				new_pairs[std::pair{pair.first, pair.second}] += count;
			}
		}

		std::swap(pairs, new_pairs);
	}

	std::unordered_map<char, unsigned long> counter;

	for (auto &pair : pairs) {
		counter[pair.first.first] += pair.second;
	}

	unsigned long max = std::max_element(counter.begin(), counter.end(),
										 [](const std::pair<char, unsigned long> &lhs,
											const std::pair<char, unsigned long> &rhs) {
											 return lhs.second < rhs.second;
										 })
							->second;
	unsigned long min = std::min_element(counter.begin(), counter.end(),
										 [](const std::pair<char, unsigned long> &lhs,
											const std::pair<char, unsigned long> &rhs) {
											 return lhs.second < rhs.second;
										 })
							->second;
	return max - min + 1;
}

extern std::unordered_map<std::string, std::string> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1("NNCB", test_input) == 1588, "Part 1 test input");
	aoc::test(Part2("NNCB", test_input) == 2188189693529, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day14.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		std::string sequence = lines[0];
		std::unordered_map<std::string, std::string> conv;

		for (unsigned i = 2; i < lines.size(); i++) {
			auto sides = aoc::by_line(lines[i], ' ');
			conv[sides[0]] = sides[2];
		}

		std::cout << "Part 1: " << Part1(sequence, conv) << std::endl;
		std::cout << "Part 2: " << Part2(sequence, conv) << std::endl;
	} else {
		return 1;
	}
}

std::unordered_map<std::string, std::string> test_input = {
	{"CH", "B"}, {"HH", "N"}, {"CB", "H"}, {"NH", "C"}, {"HB", "C"}, {"HC", "B"},
	{"HN", "C"}, {"NN", "C"}, {"BH", "H"}, {"NC", "B"}, {"NB", "B"}, {"BN", "B"},
	{"BB", "N"}, {"BC", "B"}, {"CC", "N"}, {"CN", "C"}};
