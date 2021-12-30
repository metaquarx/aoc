// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <unordered_map>

struct instruction {
	std::pair<int, int> x;
	std::pair<int, int> y;
	std::pair<int, int> z;

	bool state;
};

static auto parse(const std::vector<std::string> &lines) {
	std::vector<instruction> ins;
	ins.reserve(lines.size());

	for (auto &line : lines) {
		instruction temp;

		unsigned offset = 3;
		if (line[1] == 'n') {
			temp.state = true;
		} else {
			temp.state = false;
			offset++;
		}

		std::sscanf(line.c_str() + offset, "x=%d..%d,y=%d..%d,z=%d..%d", &temp.x.first,
					&temp.x.second, &temp.y.first, &temp.y.second, &temp.z.first, &temp.z.second);

		ins.push_back(temp);
	}

	return ins;
}

struct coord {
	int x;
	int y;
	int z;

	coord(int u = 0) : x(u), y(u), z(u) {
	}
	coord(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {
	}

	coord min(const coord &o) const {
		return {std::min(x, o.x), std::min(y, o.y), std::min(z, o.z)};
	}

	coord max(const coord &o) const {
		return {std::max(x, o.x), std::max(y, o.y), std::max(z, o.z)};
	}

	coord operator-(const coord &o) const {
		return {x - o.x, y - o.y, z - o.z};
	}

	bool operator==(const coord &other) const {
		return x == other.x && y == other.y && z == other.z;
	}
};

namespace std {
template <>
struct hash<coord> {
	size_t operator()(const coord &k) const {
		size_t res = 17;
		res = res * 31 + hash<int>()(k.x);
		res = res * 31 + hash<int>()(k.y);
		res = res * 31 + hash<int>()(k.z);
		return res;
	}
};
}  // namespace std

static auto Part1(const std::vector<instruction> &instructions) {
	std::unordered_map<coord, bool> states;

	for (auto &ins : instructions) {

		if (ins.x.first >= -50 && ins.x.second <= 50) {
			for (int x = ins.x.first; x <= ins.x.second; x++) {
				for (int y = ins.y.first; y <= ins.y.second; y++) {
					for (int z = ins.z.first; z <= ins.z.second; z++) {
						states[{x, y, z}] = ins.state;
					}
				}
			}
		}
	}

	unsigned total = 0;
	for (auto &[c, state] : states) {
		if (state) {
			total++;
		}
	}
	return total;
}

struct Cube {
	coord min;
	coord max;

	bool state;

	Cube(coord mi, coord ma, bool st) : min(mi), max(ma), state(st) {
	}

	std::optional<Cube> intersection(const Cube &o) const {
		auto mi = min.max(o.min);
		auto ma = max.min(o.max);

		if (mi.x < ma.x && mi.y < ma.y && mi.z < ma.z) {
			return Cube{mi, ma, o.state};
		} else {
			return std::nullopt;
		}
	}

	unsigned long volume() const {
		auto d = max - min;
		auto [dx, dy, dz] = d;
		return static_cast<unsigned long>(std::abs(dx)) * static_cast<unsigned long>(std::abs(dy)) *
			   static_cast<unsigned long>(std::abs(dz));
	}
};

struct Reactor {
	std::vector<Cube> cuboids;
	unsigned long count;

	Reactor() : count(0) {
	}

	void add(const Cube &o) {
		std::vector<Cube> offsets;
		for (auto &cube : cuboids) {
			auto overlap = o.intersection(cube);
			if (overlap) {
				overlap->state = !overlap->state;

				if (overlap->state) {
					count += overlap->volume();
				} else {
					count -= overlap->volume();
				}

				offsets.push_back(*overlap);
			}
		}
		cuboids.insert(cuboids.end(), offsets.begin(), offsets.end());

		if (o.state) {
			count += o.volume();
			cuboids.push_back(o);
		}
	}
};

static auto Part2(const std::vector<instruction> &instructions) {
	Reactor rect;

	for (auto &ins : instructions) {
		Cube cb(coord{ins.x.first, ins.y.first, ins.z.first},
				coord{ins.x.second + 1, ins.y.second + 1, ins.z.second + 1}, ins.state);

		rect.add(cb);
	}

	return rect.count;
}

int main(int argc, char **argv) {
	if (auto input = aoc::get_input("inputs/2021/Day22.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		auto ins = parse(lines);

		std::cout << "Part 1: " << Part1(ins) << std::endl;
		std::cout << "Part 2: " << Part2(ins) << std::endl;
	} else {
		return 1;
	}
}
