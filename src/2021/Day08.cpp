// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>

static unsigned Part1(const std::vector<std::string> &input) {
	unsigned total = 0;

	for (auto line : input) {
		auto split = aoc::by_line(line, '|');
		auto split_out = aoc::by_line(split[1], ' ');

		for (auto digit : split_out) {
			if (digit.size() == 2 || digit.size() == 4 || digit.size() == 3 || digit.size() == 7) {
				total++;
			}
		}
	}

	return total;
}

static unsigned Part2(const std::vector<std::string> &input) {
	auto counter = [](const std::string &s) {
		std::unordered_map<char, int> out;
		for (auto chr : s) {
			out[chr]++;
		}
		return out;
	};
	auto clean = [](const std::string &s) {
		auto out = s;
		out.erase(std::remove(out.begin(), out.end(), ' '), out.end());
		return out;
	};
	auto mapper = [](const std::string &s, const std::unordered_map<char, int> &mapping) {
		std::vector<int> out;
		for (char chr : s) {
			out.push_back(mapping.at(chr));
		}
		std::sort(out.begin(), out.end());
		return out;
	};

	auto default_pattern = "abcefg cf acdeg acdfg bdcf abdfg abdefg acf abcdefg abcdfg";
	auto default_mapping = counter(clean(default_pattern));

	std::map<std::vector<int>, unsigned> translator;
	auto pattern_split = aoc::by_line(default_pattern, ' ');
	for (unsigned i = 0; i < pattern_split.size(); i++) {
		translator[mapper(pattern_split[i], default_mapping)] = i;
	}

	auto process = [&counter, &mapper, &translator](const std::vector<std::string> &parts) {
		auto outs = parts[1];
		auto occurances = counter(parts[0]);

		std::string out;
		for (auto item : aoc::by_line(outs, ' ')) {
			if (item.size() == 0) {
				continue;
			}
			out += std::to_string(translator.at(mapper(item, occurances)));
		}
		return std::stoi(out);
	};

	unsigned total = 0;
	for (auto line : input) {
		auto split = aoc::by_line(line, '|');
		total += static_cast<unsigned>(process(split));
	}
	return total;
}

extern std::vector<std::string> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input) == 26, "Part 1 test input");
	aoc::test(Part2(test_input) == 61229, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day08.txt", argc, argv)) {
		auto formatted = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "Part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}

std::vector<std::string> test_input = {
	"be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe",
	"edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc",
	"fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg",
	"fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb",
	"aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea",
	"fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb",
	"dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe",
	"bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef",
	"egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb",
	"gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce"};
