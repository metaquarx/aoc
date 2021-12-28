// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

struct Point {
	char lhs;
	char rhs;

	Point() : lhs(0), rhs(0) {
	}
	Point(char l, char r) : lhs(l), rhs(r) {
	}

	bool operator==(const Point &o) const {
		return lhs == o.lhs && rhs == o.rhs;
	}

	bool operator<(const Point &o) const {
		return lhs < o.lhs;
	}

	unsigned manhattan(const Point &o) const {
		return static_cast<unsigned>(std::abs(lhs - o.lhs)) + static_cast<unsigned>(std::abs(rhs - o.rhs));
	}

	bool is_room() const {
		return rhs > 1;
	}

	bool is_hallway() const {
		return !is_room();
	}

	bool is_forbidden() const {
		if (is_hallway() && (lhs == 3 || lhs == 5 || lhs == 7 || lhs == 9)) {
			return true;
		} else {
			return false;
		}
	}
};

namespace std {
template <>
struct hash<Point> {
	size_t operator()(const Point &k) const {
		size_t res = 17;
		res = res * 31 + hash<char>()(k.lhs);
		res = res * 31 + hash<char>()(k.rhs);
		return res;
	}
};
}  // namespace std

struct Field {
	char value;

	Field() : value(0) {
	}

	Field(char v) : value(v) {
	}

	bool operator==(const Field &o) const {
		return value == o.value;
	}

	bool is_empty() const {
		return value == '.';
	}

	bool is_player() const {
		return !is_empty();
	}

	bool operator!=(Field o) const {
		return value != o.value;
	}

	unsigned cost() const {
		if (value == 'A') {
			return 1;
		} else if (value == 'B') {
			return 10;
		} else if (value == 'C') {
			return 100;
		} else if (value == 'D') {
			return 1000;
		}
		throw;
	}

	char destination() const {
		if (value == 'A') {
			return 3;
		} else if (value == 'B') {
			return 5;
		} else if (value == 'C') {
			return 7;
		} else if (value == 'D') {
			return 9;
		}
		throw;
	}
};

template <typename T>
int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

struct Maze {
	std::unordered_map<Point, Field> maze;

	Maze() {
	}
	Maze(std::unordered_map<Point, Field> mz) : maze(mz) {
	}
	// Maze(const std::unordered_map<Point, Field> &&mz) : maze(mz) {
	// }

	bool is_solved() const {
		for (auto &[p, f] : maze) {
			if (f.is_player()) {
				if (!(f.destination() == p.lhs && p.is_room())) {
					return false;
				}
			}
		}
		return true;
	}

	bool operator!=(const Maze &o) const {
		return maze != o.maze;
	}

	unsigned long hash() const {
		std::vector<std::pair<Point, Field>> ps(maze.begin(), maze.end());
		std::sort(ps.begin(), ps.end(), [](auto &l, auto &r) {
			if (r.first.rhs > l.first.rhs) {
				return true;
			}
			if (r.first.rhs == l.first.rhs && r.first.lhs > l.first.lhs) {
				return true;
			}
			return false;
		});

		unsigned long acc = 17;
		for (auto &[_, f] : ps) {
			acc = acc * 31 + std::hash<char>()(f.value);
		}
		return acc;
	}

	std::vector<std::pair<unsigned, Maze>> get_moves() const {
		std::vector<Point> targets;
		targets.reserve(maze.size());
		// empty spaces that we can move to
		for (auto &[p, f] : maze) {
			if (!p.is_forbidden() && f.is_empty()) {
				targets.push_back(p);
			}
		}

		std::vector<std::pair<unsigned, Maze>> out;
		out.reserve(30);

		// loop over players
		for (auto &[from, f] : maze) {
			if (!f.is_player()) {
				continue;
			}

			std::vector<Point> possibilities;

			if (from.is_room()) {
				// moving to reachable hallways
				for (auto &to : targets) {
					if (to.is_hallway() && is_path_y_clear(from.lhs, from.rhs, 1, true) &&
						is_path_x_clear(1, from.lhs, to.lhs, false)) {
						possibilities.push_back(to);
					}
				}
			} else {
				// if in hallway, only the deepest field is valid
				auto dest_room = f.destination();

				// any wrong types
				bool no_others_in_room = true;
				for (auto &[tp, tf] : maze) {
					if (tp.is_room() && tp.lhs == dest_room && tf.is_player() && f != tf) {
						no_others_in_room = false;
						break;
					}
				}

				char deepest = 0;
				for (auto &[p, tf] : maze) {
					// only rooms
					if (!p.is_room()) {
						continue;
					}

					// only ones that are ours and empty
					if (f.destination() == p.lhs && tf.is_empty()) {
						deepest = std::max(deepest, p.rhs);
					}
				}

				if (no_others_in_room && deepest >= 1) {
					for (auto &to : targets) {
						if (to.is_room() && to.lhs == dest_room && to.rhs == deepest &&
							is_path_x_clear(from.rhs, from.lhs, dest_room, true) &&
							is_path_y_clear(dest_room, from.rhs, to.rhs, false)) {
							possibilities.push_back(to);
						}
					}
				}
			}

			for (auto &t : possibilities) {
				auto move = maze;
				move[from] = '.';
				move[t] = f;

				unsigned cost = from.manhattan(t) * f.cost();

				out.push_back(std::pair{cost, Maze{move}});
			}
		}

		return out;
	}

	bool is_path_x_clear(char y, char from, char to, bool skip) const {
		auto dir = sgn(to - from);

		if (skip) {
			from += dir;
		}

		while (from != to) {
			if (!maze.at(Point{from, y}).is_empty()) {
				return false;
			}
			from += dir;
		}
		return true;
	}

	bool is_path_y_clear(char x, char from, char to, bool skip) const {
		auto dir = sgn(to - from);

		if (skip) {
			from += dir;
		}

		while (from != to) {
			if (!maze.at(Point{x, from}).is_empty()) {
				return false;
			}
			from += dir;
		}
		return true;
	}
};

static auto parse(const std::vector<std::string> &lines) {
	Maze out;

	for (unsigned y = 0; y < lines.size(); y++) {
		for (unsigned x = 0; x < lines[y].size(); x++) {
			if (lines[y][x] == '#' || lines[y][x] == ' ') {
				continue;
			}

			out.maze[Point(static_cast<char>(x), static_cast<char>(y))] = lines[y][x];
		}
	}

	return out;
}

struct State {
	unsigned cost;
	Maze maze;

	bool operator<(const State &o) const {
		return cost > o.cost;
	}
};

static auto run(const Maze &input) {
	std::unordered_map<unsigned long, unsigned> dist;
	std::priority_queue<State> q;

	q.push(State{0, input});

	while (!q.empty()) {
		auto state = q.top();

		if (state.maze.is_solved()) {
			return state.cost;
		}

		// add possible moves
		for (auto [d, m] : state.maze.get_moves()) {
			auto new_cost = state.cost + d;

			// only if its the cheapest path to target
			unsigned cost;
			if (dist.find(m.hash()) == dist.end()) {
				cost = std::numeric_limits<unsigned>::max();
			} else {
				cost = dist.at(m.hash());
			}

			if (cost > new_cost) {
				dist[m.hash()] = new_cost;
				q.push({new_cost, m});
			}
		}

		q.pop();
	}

	return 0u;
}

static auto Part1(const Maze &input) {
	auto cost = run(input);
	return cost;
}

static auto extend(const Maze &input) {
	Maze out;

	for (auto &[p, f] : input.maze) {
		if (p.rhs == 1 || p.rhs == 2) {
			out.maze[p] = f;
		} else if (p.rhs == 3) {
			out.maze[Point(p.lhs, p.rhs + 2)] = f;
		}
	}

	out.maze[Point(3, 3)] = 'D';
	out.maze[Point(5, 3)] = 'C';
	out.maze[Point(7, 3)] = 'B';
	out.maze[Point(9, 3)] = 'A';

	out.maze[Point(3, 4)] = 'D';
	out.maze[Point(5, 4)] = 'B';
	out.maze[Point(7, 4)] = 'A';
	out.maze[Point(9, 4)] = 'C';

	return out;
}

static auto Part2(const Maze &input) {
	auto maze = extend(input);
	auto cost = run(maze);
	return cost;
}

int main(int argc, char **argv) {
	if (auto input = aoc::get_input("inputs/2021/Day23.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		auto maze = parse(lines);

		std::cout << "Part 1: " << Part1(maze) << std::endl;
		std::cout << "Part 2: " << Part2(maze) << std::endl;
	} else {
		return 1;
	}
}
