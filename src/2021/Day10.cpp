// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <iostream>
#include <stack>

static unsigned Part1(const std::vector<std::string> &input) {
	auto get_points = [](char chr) {
		if (chr == ')') {
			return 3u;
		} else if (chr == ']') {
			return 57u;
		} else if (chr == '}') {
			return 1197u;
		} else if (chr == '>') {
			return 25137u;
		}
		return 0u;
	};

	auto count_to_illegal = [&get_points](const std::string &line) {
		unsigned points = 0;

		std::stack<char> stack;
		for (auto chr : line) {
			switch (chr) {
				case '(': {
					stack.push(')');
					break;
				}
				case '[': {
					stack.push(']');
					break;
				}
				case '{': {
					stack.push('}');
					break;
				}
				case '<': {
					stack.push('>');
					break;
				}
				case ')':
					[[fallthrough]];
				case ']':
					[[fallthrough]];
				case '}':
					[[fallthrough]];
				case '>': {
					if (stack.top() != chr) {
						points += get_points(chr);
						return points;
					}
					stack.pop();
				}
			}
		}
		return points;
	};

	unsigned points = 0;
	for (auto line : input) {
		points += count_to_illegal(line);
	}
	return points;
}

static unsigned long Part2(const std::vector<std::string> &input) {
	auto get_points = [](char chr) {
		if (chr == ')') {
			return 1u;
		} else if (chr == ']') {
			return 2u;
		} else if (chr == '}') {
			return 3u;
		} else if (chr == '>') {
			return 4u;
		}
		return 0u;
	};

	auto complete = [&get_points](const std::string &line) {
		std::stack<char> stack;
		for (auto chr : line) {
			switch (chr) {
				case '(': {
					stack.push(')');
					break;
				}
				case '[': {
					stack.push(']');
					break;
				}
				case '{': {
					stack.push('}');
					break;
				}
				case '<': {
					stack.push('>');
					break;
				}
				case ')':
					[[fallthrough]];
				case ']':
					[[fallthrough]];
				case '}':
					[[fallthrough]];
				case '>': {
					if (stack.top() != chr) {
						return 0ul;
					}
					stack.pop();
				}
			}
		}

		unsigned long points = 0;
		while (!stack.empty()) {
			points = (points * 5) + get_points(stack.top());
			stack.pop();
		}
		return points;
	};

	std::vector<unsigned long> points;
	unsigned lineno = 0;
	for (auto line : input) {
		auto completed = complete(line);
		if (completed != 0) {
			points.push_back(completed);
		}
		lineno++;
	}
	unsigned long n = points.size() / 2u;
	std::nth_element(points.begin(), points.begin() + n, points.end());
	return points[n];
}

extern std::vector<std::string> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input) == 26397, "Part 1 test input");
	aoc::test(Part2(test_input) == 288957, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day10.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(lines) << std::endl;
		std::cout << "Part 2: " << Part2(lines) << std::endl;
	} else {
		return 1;
	}
}

std::vector<std::string> test_input = {"[({(<(())[]>[[{[]{<()<>>", "[(()[<>])]({[<{<<[]>>(",
									   "{([(<{}[<>[]}>{[]{[(<()>", "(((({<>}<{<{<>}{[]{[]{}",
									   "[[<[([]))<([[{}[[()]]]",   "[{[{({}]{}}([{[{{{}}([]",
									   "{<[[]]>}<{[{[{[]{()[[[]",  "[<(<(<(<{}))><([]([]()",
									   "<{([([[(<>()){}]>(<<{{",   "<{([{{}}[<[[[<>{}]]]>[]]"};
