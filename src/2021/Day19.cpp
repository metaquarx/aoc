// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace std {
template <>
struct hash<std::vector<int>> {
	std::size_t operator()(const std::vector<int> &key) const {
		return std::hash<std::string>()(std::to_string(key[0]) + std::to_string(key[1]) +
										std::to_string(key[2]));
	}
};
}  // namespace std

struct Scanner {
	std::vector<std::vector<int>> beacons;
	int axis = 0;
	std::vector<int> correction = {0, 0, 0};

	void rotate_axis() {
		for (auto &beacon : beacons) {
			beacon = {-beacon[1], beacon[0], beacon[2]};
		}
	}

	void change_axis() {
		switch (axis) {
			case 0:
				[[fallthrough]];
			case 1:
				[[fallthrough]];
			case 2:
				[[fallthrough]];
			case 3: {
				for (auto &beacon : beacons) {
					beacon = {beacon[2], beacon[1], -beacon[0]};
				}
				break;
			}
			case 4: {
				for (auto &beacon : beacons) {
					beacon = {beacon[0], -beacon[2], beacon[1]};
				}
				break;
			}
			case 5: {
				for (auto &beacon : beacons) {
					beacon = {beacon[0], -beacon[1], -beacon[2]};
				}
				break;
			}
		}

		axis++;
	}
	bool operator==(const std::unordered_set<std::vector<int>> &other) {
		std::unordered_map<std::vector<int>, int> distances;
		std::vector<int> direction;

		for (auto &a : other) {
			for (auto &b : beacons) {
				direction = {b[0] - a[0], b[1] - a[1], b[2] - a[2]};

				if (++distances[direction] == 4) {
					correction = direction;
					return true;
				}
			}
		}
		return false;
	}

	void align() {
		for (auto &beacon : beacons) {
			for (unsigned i = 0; i < 3; i++) {
				beacon[i] -= correction[i];
			}
		}
	}
};

static auto parse(const std::vector<std::string> &lines) {
	std::vector<Scanner> scanners;
	scanners.reserve(35);

	Scanner temp;
	temp.beacons.reserve(35);
	for (auto &line : lines) {
		if (line[1] == '-') {
			if (temp.beacons.size() > 0) {
				scanners.push_back(temp);
			}
			temp.beacons.clear();
		} else if (line.size() > 1) {
			temp.beacons.push_back({});
			for (auto num : aoc::by_line(line, ',')) {
				temp.beacons.back().push_back(std::stoi(num));
			}
		}
	}
	scanners.push_back(temp);

	return scanners;
}

static auto Part1(std::vector<Scanner> &scanners) {
	std::unordered_set<std::vector<int>> beacons;
	for (auto &beacon : scanners.front().beacons) {
		beacons.insert(beacon);
	}

	for (auto it = scanners.begin() + 1; it != scanners.end(); it++) {
		bool matched = false;

		// 6 axis
		for (int i = 0; i < 6 && !matched; i++) {
			it->change_axis();

			// 4 rotations per axis
			for (int j = 0; j < 4 && !matched; j++) {
				it->rotate_axis();

				if (*it == beacons) {
					it->align();
					for (auto &beacon : it->beacons) {
						beacons.insert(beacon);
					}
					matched = true;
				}
			}
		}

		if (!matched) {
			// try again later
			it->axis = 0;
			scanners.push_back(*it);
			it = scanners.erase(it) - 1;
		}
	}

	return beacons.size();
}

static int distance(const std::vector<int> &lhs, const std::vector<int> &rhs) {
	int result = 0;

	result += std::abs(rhs[0] - lhs[0]);
	result += std::abs(rhs[1] - lhs[1]);
	result += std::abs(rhs[2] - lhs[2]);

	return result;
}

static auto Part2(std::vector<Scanner> &scanners) {
	int result = 0;

	for (auto it = scanners.begin(); it != scanners.end(); it++) {
		for (auto itt = scanners.begin(); itt != scanners.end(); itt++) {
			if (it != itt) {
				result = std::max(result, distance(it->correction, itt->correction));
			}
		}
	}

	return result;
}

int main(int argc, char **argv) {
	if (auto input = aoc::get_input("inputs/2021/Day19.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		auto scanners = parse(lines);

		std::cout << "Part 1: " << Part1(scanners) << std::endl;
		std::cout << "Part 2: " << Part2(scanners) << std::endl;
	} else {
		return 1;
	}
}
