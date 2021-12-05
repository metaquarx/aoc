// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <tuple>

class Boards {
public:
	std::vector<unsigned> draw_order;
	std::vector<std::array<std::array<std::pair<unsigned, bool>, 5>, 5>> boards;

	void loadFromLines(const std::vector<std::string> &input) {
		std::stringstream os(input[0]);
		for (unsigned i; os >> i;) {
			draw_order.push_back(i);
			if (os.peek() == ',') {
				os.ignore();
			}
		}

		for (unsigned i = 1; i + 5 < input.size(); i += 6) {
			std::stringstream ss(
			  input[i + 1] + "\n" + input[i + 2] + "\n" + input[i + 3] + "\n" + input[i + 4] + "\n" + input[i + 5]);
			boards.push_back({});

			unsigned offset = 0;
			for (unsigned j; ss >> j;) {
				boards.back()[offset / 5][offset % 5] = {j, false};

				offset++;
			}
		}
	}

	void mark(unsigned item) {
		for (auto &board : boards) {
			for (auto &y : board) {
				for (auto &x : y) {
					if (x.first == item) {
						x.second = true;
					}
				}
			}
		}
	}

	bool check_for_wins(unsigned board_index) {
		auto &board = boards[board_index];

		for (unsigned z = 0; z < 5; z++) {
			if ((board[z][0].second && board[z][1].second && board[z][2].second && board[z][3].second && board[z][4].second)
				|| (board[0][z].second && board[1][z].second && board[2][z].second && board[3][z].second && board[4][z].second)) {
				return true;
			}
		}

		return false;
	}

	unsigned sum_unmarked(unsigned board_index) {
		auto &board = boards[board_index];
		unsigned sum = 0;

		for (auto &y : board) {
			for (auto &x : y) {
				if (!x.second) {
					sum += x.first;
				}
			}
		}

		return sum;
	}
};

static unsigned Part1(const std::vector<std::string> &input) {
	Boards brd;
	brd.loadFromLines(input);

	for (auto pulled : brd.draw_order) {
		brd.mark(pulled);

		for (unsigned i = 0; i < brd.boards.size(); i++) {
			if (brd.check_for_wins(i)) {
				return brd.sum_unmarked(i) * pulled;
			}
		}
	}
	return 0;
}

static unsigned Part2(const std::vector<std::string> &input) {
	Boards brd;
	brd.loadFromLines(input);

	unsigned previous_pull = 0;
	unsigned previous_sum = 0;
	for (auto pulled : brd.draw_order) {
		brd.mark(pulled);

		if (brd.boards.size() == 0) {
			return previous_pull * previous_sum;
		}

		for (unsigned i = 0; i < brd.boards.size(); i++) {
			if (brd.check_for_wins(i)) {
				brd.boards.erase(brd.boards.begin() + i);
			}
		}

		previous_sum = brd.sum_unmarked(0);
		previous_pull = pulled;
	}

	return 0;
}

extern std::vector<std::string> test_input;

int main(int argc, char **argv) {
	aoc::test(Part1(test_input) == 4512, "Part 1 test input");
	aoc::test(Part2(test_input) == 1924, "Part 2 test input");

	if (auto input = aoc::get_input("inputs/2021/Day04.txt", argc, argv)) {
		auto formatted = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(formatted) << std::endl;
		std::cout << "Part 2: " << Part2(formatted) << std::endl;
	} else {
		return 1;
	}
}

std::vector<std::string> test_input = {"7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1\n", "\n",
  "22 13 17 11  0\n", " 8  2 23  4 24\n", "21  9 14 16  7\n", " 6 10  3 18  5\n", " 1 12 20 15 19\n", "\n",
  " 3 15  0  2 22\n", " 9 18 13 17  5\n", "19  8  7 25 23\n", "20 11 10 24  4\n", "14 21 16 12  6\n", "\n",
  "14 21 17 24  4\n", "10 16 15  9 19\n", "18  8 23 26 20\n", "22 11 13  6  5\n", " 2  0 12  3  7\n"};
