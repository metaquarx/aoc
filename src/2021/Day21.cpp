// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

static auto parse(const std::vector<std::string> &lines) {
	return std::pair{static_cast<unsigned>(std::stoi(std::string() + lines[0].back())),
					 static_cast<unsigned>(std::stoi(std::string() + lines[1].back()))};
}

static auto adv(unsigned piece, unsigned amt) {
	auto nw = piece + amt;
	while (nw > 10) {
		nw -= 10;
	}
	return nw;
}

static auto Part1(unsigned p1_s, unsigned p2_s) {
	struct d_dice {
		d_dice() : c(1), rolls(0) {
		}
		unsigned c;
		unsigned rolls;

		auto roll() {
			c++;
			rolls++;
			if (c == 102) {
				c = 2;
			}
			return c - 1;
		}
	};

	d_dice dice;

	unsigned p1_pos = p1_s;
	unsigned p2_pos = p2_s;

	unsigned p1_score = 0;
	unsigned p2_score = 0;

	while (true) {
		auto p1_roll = dice.roll() + dice.roll() + dice.roll();
		p1_pos = adv(p1_pos, p1_roll);
		p1_score += p1_pos;
		if (p1_score >= 1000) {
			return p2_score * dice.rolls;
		}

		auto p2_roll = dice.roll() + dice.roll() + dice.roll();
		p2_pos = adv(p2_pos, p2_roll);
		p2_score += p2_pos;
		if (p2_score >= 1000) {
			return p1_score * dice.rolls;
		}
	}
}

struct gamestate {
	unsigned a_pos;
	unsigned b_pos;
	unsigned a_score;
	unsigned b_score;

	bool operator==(const gamestate &other) const {
		return a_pos == other.a_pos && b_pos == other.b_pos && a_score == other.a_score &&
			   b_score == other.b_score;
	}
};

namespace std {
template <>
struct hash<gamestate> {
	size_t operator()(const gamestate &gs) const {
		size_t res = 17;
		res = res * 31 + hash<unsigned>()(gs.a_pos);
		res = res * 31 + hash<unsigned>()(gs.b_pos);
		res = res * 31 + hash<unsigned>()(gs.a_score);
		res = res * 31 + hash<unsigned>()(gs.b_score);
		return res;
	}
};
}  // namespace std

static auto Part2(unsigned p1_s, unsigned p2_s) {
	std::unordered_map<gamestate, std::pair<unsigned long, unsigned long>> cache;

	std::function<std::pair<unsigned long, unsigned long>(gamestate gs)> step = [&](gamestate gs) {
		if (cache.find(gs) != cache.end()) {
			return cache[gs];
		}

		if (gs.a_score >= 21) {
			return std::pair{1ul, 0ul};
		} else if (gs.b_score >= 21) {
			return std::pair{0ul, 1ul};
		}

		unsigned long p1 = 0;
		unsigned long p2 = 0;

		for (unsigned i = 1; i <= 3; i++) {
			for (unsigned j = 1; j <= 3; j++) {
				for (unsigned k = 1; k <= 3; k++) {
					gamestate n;
					n.a_pos = adv(gs.a_pos, i + j + k);
					n.a_score = gs.a_score + n.a_pos;

					auto [n2, n1] = step({gs.b_pos, n.a_pos, gs.b_score, n.a_score});
					p1 += n1;
					p2 += n2;
				}
			}
		}

		cache[gs] = {p1, p2};
		return std::pair{p1, p2};
	};

	auto [p1s, p2s] = step({p1_s, p2_s, 0, 0});

	return std::max(p1s, p2s);
}

int main(int argc, char **argv) {
	if (auto input = aoc::get_input("inputs/2021/Day21.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		auto [p1, p2] = parse(lines);

		std::cout << "Part 1: " << Part1(p1, p2) << std::endl;
		std::cout << "Part 2: " << Part2(p1, p2) << std::endl;
	} else {
		return 1;
	}
}
