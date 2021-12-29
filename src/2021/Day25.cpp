// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <execution>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

enum class Tile { Empty, East, South };

struct Map {
	std::vector<std::vector<Tile>> map;

	unsigned age = 0;

	std::pair<unsigned, unsigned> get_next(unsigned y, unsigned x) const {
		unsigned x_off = 0;
		unsigned y_off = 0;

		if (map[y][x] == Tile::East) {
			x_off++;
		} else if (map[y][x] == Tile::South) {
			y_off++;
		} else {
			return {0, 0};
		}

		y = (y + y_off) % map.size();
		x = (x + x_off) % map[y].size();

		return {y, x};
	}

	bool can_move(unsigned y, unsigned x) const {
		auto [yd, xd] = get_next(y, x);

		return map[yd][xd] == Tile::Empty;
	}

	bool step() {
		std::vector<std::vector<Tile>> out(map.begin(), map.end());
		unsigned move_count = 0;

		for (unsigned y = 0; y < map.size(); y++) {
			for (unsigned x = 0; x < map[y].size(); x++) {
				if (map[y][x] == Tile::East && can_move(y, x)) {
					auto [yd, xd] = get_next(y, x);

					out[yd][xd] = map[y][x];
					out[y][x] = Tile::Empty;

					move_count++;
				}
			}
		}

		std::copy(out.begin(), out.end(), map.begin());

		for (unsigned y = 0; y < map.size(); y++) {
			for (unsigned x = 0; x < map[y].size(); x++) {
				if (map[y][x] == Tile::South && can_move(y, x)) {
					auto [yd, xd] = get_next(y, x);

					out[yd][xd] = map[y][x];
					out[y][x] = Tile::Empty;

					move_count++;
				}
			}
		}

		map = std::move(out);
		age++;
		return move_count;
	}

	void print() const {
		for (auto &y : map) {
			for (auto x : y) {
				if (x == Tile::Empty) {
					std::cout << '.';
				} else if (x == Tile::South) {
					std::cout << 'v';
				} else {
					std::cout << '>';
				}
			}
			std::cout << std::endl;
		}
	}
};

static auto parse(const std::vector<std::string> &lines) {
	Map out;
	out.map.reserve(140);

	for (auto &line : lines) {
		std::vector<Tile> temp;
		temp.reserve(140);

		for (auto chr : line) {
			if (chr == '.') {
				temp.push_back(Tile::Empty);
			} else if (chr == '>') {
				temp.push_back(Tile::East);
			} else if (chr == 'v') {
				temp.push_back(Tile::South);
			}
		}

		out.map.push_back(temp);
	}

	return out;
}

static auto Part1(Map map) {
	while (map.step()) {
	}

	return map.age;
}

int main(int argc, char **argv) {
	if (auto input = aoc::get_input("inputs/2021/Day25.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		auto map = parse(lines);

		std::cout << "Part 1: " << Part1(map) << std::endl;
	} else {
		return 1;
	}
}
