// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

class Vector {
public:
	Vector(unsigned x_ = 0, unsigned y_ = 0) : x(x_), y(y_) {
	}
	bool operator==(const Vector &other) const {
		return (x == other.x) && (y == other.y);
	}
	unsigned x;
	unsigned y;
};

class Line {
public:
	Line(Vector start_ = {}, Vector end_ = {}) : start(start_), end(end_) {
	}
	Line(const std::string &input) {
		std::sscanf(input.c_str(), "%u,%u -> %u,%u", &start.x, &start.y, &end.x, &end.y);
	}
	Vector start;
	Vector end;
};

namespace std {
template <>
struct hash<Vector> {
	size_t operator()(const Vector &k) const {
		size_t res = 17;
		res = res * 31 + hash<unsigned>()(k.x);
		res = res * 31 + hash<unsigned>()(k.y);
		return res;
	}
};
}  // namespace std

static unsigned Part1(const std::vector<std::string> &input) {
	std::vector<Line> lines;

	lines.reserve(input.size());
	for (auto &line : input) {
		lines.emplace_back(line);
	}

	std::vector<Vector> points;
	for (auto line : lines) {
		if (line.start.x == line.end.x || line.start.y == line.end.y) {
			for (unsigned x = std::min(line.start.x, line.end.x);
				 x <= std::max(line.start.x, line.end.x); x++) {
				for (unsigned y = std::min(line.start.y, line.end.y);
					 y <= std::max(line.start.y, line.end.y); y++) {
					points.emplace_back(x, y);
				}
			}
		}
	}

	std::unordered_map<Vector, unsigned> rv;
	for (auto val : points) {
		rv[val]++;
	}

	unsigned total = 0;
	for (auto val : rv) {
		if (val.second > 1) {
			total++;
		}
	}

	return total;
}

static unsigned Part2(const std::vector<std::string> &input) {
	std::vector<Line> lines;

	lines.reserve(input.size());
	for (auto &line : input) {
		lines.emplace_back(line);
	}

	std::vector<Vector> points;
	for (auto line : lines) {
		if (line.start.x == line.end.x || line.start.y == line.end.y) {
			for (unsigned x = std::min(line.start.x, line.end.x);
				 x <= std::max(line.start.x, line.end.x); x++) {
				for (unsigned y = std::min(line.start.y, line.end.y);
					 y <= std::max(line.start.y, line.end.y); y++) {
					points.emplace_back(x, y);
				}
			}
		} else {
			if (line.start.x < line.end.x) {
				for (unsigned x = line.start.x; x <= line.end.x; x++) {
					if (line.start.y < line.end.y) {
						points.emplace_back(x, x - line.start.x + line.start.y);
					} else {
						points.emplace_back(x, line.start.y - (x - line.start.x));
					}
				}
			} else {
				for (unsigned x = line.end.x; x <= line.start.x; x++) {
					if (line.end.y < line.start.y) {
						points.emplace_back(x, x - line.end.x + line.end.y);
					} else {
						points.emplace_back(x, line.end.y - (x - line.end.x));
					}
				}
			}
		}
	}

	std::unordered_map<Vector, unsigned> rv;
	for (auto val : points) {
		rv[val]++;
	}

	unsigned total = 0;
	for (auto val : rv) {
		if (val.second > 1) {
			total++;
		}
	}

	return total;
}

extern std::vector<std::string> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input) == 5, "Part 1 test input");
	aoc::test(Part2(test_input) == 12, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day05.txt", argc, argv)) {
		auto formatted = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "Part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}

std::vector<std::string> test_input = {"0,9 -> 5,9", "8,0 -> 0,8", "9,4 -> 3,4", "2,2 -> 2,1",
									   "7,0 -> 7,4", "6,4 -> 2,0", "0,9 -> 2,9", "3,4 -> 1,4",
									   "0,0 -> 8,8", "5,5 -> 8,2"};
