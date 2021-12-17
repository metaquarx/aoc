// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <functional>
#include <iostream>

static auto parse(const std::string &input) {
	std::vector<char> bits;	 // only 0 or 1

	auto add_nibble = [&](char in) {
		bits.push_back((in >> 3) & 1);
		bits.push_back((in >> 2) & 1);
		bits.push_back((in >> 1) & 1);
		bits.push_back((in >> 0) & 1);
	};

	for (auto &chr : input) {
		if (chr <= '9') {
			add_nibble(chr - '0');
		} else {
			add_nibble(chr - 'A' + 10);
		}
	}

	return bits;
}

template <typename it>
static unsigned long to_int(it begin, unsigned size) {
	unsigned long ret = 0;
	for (unsigned i = 0; i < size; i++) {
		if (*(begin + i) == 1) {
			ret |= 1UL << (size - (i + 1));
		}
	}
	return ret;
}

static auto Part1(const std::string &input) {
	auto bits = parse(input);

	std::function<std::pair<unsigned long, unsigned>(unsigned)> parse_packet = [&](unsigned offset) {
		auto version = to_int(bits.begin() + offset, 3);
		auto type = to_int(bits.begin() + offset + 3, 3);
		unsigned parsed = 6;

		if (type == 4) {  // literal
			while (to_int(bits.begin() + offset + parsed, 1) == 1) {
				parsed += 5;
			}
			parsed += 5;
			return std::pair{version, parsed};
		} else {  // operator
			auto length_type_id = to_int(bits.begin() + offset + 6, 1);
			parsed++;

			if (length_type_id == 0) {	// by size
				auto total_size = to_int(bits.begin() + offset + parsed, 15);
				parsed += 15;

				unsigned current_size = 0;
				while (current_size < total_size) {

					auto [v, size] = parse_packet(offset + parsed);
					version += v;
					current_size += size;
					parsed += size;
				}

				return std::pair{version, parsed};
			} else {  // by packet count
				auto packets = to_int(bits.begin() + offset + parsed, 11);
				parsed += 11;

				for (unsigned i = 0; i < packets; i++) {
					auto [v, size] = parse_packet(offset + parsed);
					version += v;
					parsed += size;
				}

				return std::pair{version, parsed};
			}
		}
	};

	return parse_packet(0).first;
}

struct Packet {
	unsigned long version;
	unsigned long type;
	unsigned long size;
	unsigned long value;
};

static Packet parse_packet(const std::vector<char> &bits, unsigned offset) {
	auto version = to_int(bits.begin() + offset, 3);
	auto type = to_int(bits.begin() + offset + 3, 3);
	unsigned parsed = 6;

	if (type == 4) {  // literal
		unsigned long literal = 0;
		while (to_int(bits.begin() + offset + parsed, 1) == 1) {
			unsigned long literal_u = 0;
			literal_u = literal << 4;
			literal_u |= to_int(bits.begin() + offset + parsed + 1, 4);
			parsed += 5;
			literal = literal_u;
		}
		unsigned long literal_u = literal << 4;
		literal_u |= to_int(bits.begin() + offset + parsed + 1, 4);
		return {version, type, parsed + 5, literal_u};
	} else {  // operator
		auto length_type_id = to_int(bits.begin() + offset + 6, 1);
		parsed++;

		auto max = to_int(bits.begin() + offset + parsed, length_type_id ? 11 : 15);
		parsed += length_type_id ? 11 : 15;

		std::vector<Packet> packets;
		for (unsigned long current = 0; current < max;) {
			auto pkt = parse_packet(bits, offset + parsed);
			packets.push_back(pkt);

			parsed += pkt.size;
			if (length_type_id == 0) {
				current += pkt.size;
			} else {
				current++;
			}
		}

		// handle
		unsigned long total = 0;

		if (type == 0) {  // sum
			for (auto &pkt : packets) {
				total += pkt.value;
			}
		} else if (type == 1) {	 // product
			total = 1;
			for (auto &pkt : packets) {
				total *= pkt.value;
			}
		} else if (type == 2) {	 // minimum
			total = 999999999999999999;
			for (auto &pkt : packets) {
				total = std::min(total, pkt.value);
			}
		} else if (type == 3) {	 // maximum
			for (auto &pkt : packets) {
				total = std::max(total, pkt.value);
			}
		} else if (type == 5) {	 // greater than
			total = (packets[0].value > packets[1].value);
		} else if (type == 6) {	 // less than
			total = (packets[0].value < packets[1].value);
		} else if (type == 7) {	 // equal to
			total = (packets[0].value == packets[1].value);
		}

		return {version, type, parsed, total};
	}
}

static auto Part2(const std::string &input) {
	auto bits = parse(input);

	return parse_packet(bits, 0).value;
}

int main(int argc, char **argv) {
	aoc::test(Part1("D2FE28") == 6, "Part 1 test input 1");
	aoc::test(Part1("8A004A801A8002F478") == 16, "Part 1 test input 2");
	aoc::test(Part1("620080001611562C8802118E34") == 12, "Part 1 test input 3");
	aoc::test(Part1("C0015000016115A2E0802F182340") == 23, "Part 1 test input 4");
	aoc::test(Part1("A0016C880162017C3686B18A3D4780") == 31, "Part 1 test input 5");
	aoc::test(Part2("C200B40A82") == 3, "Part 2 test input 1");
	aoc::test(Part2("04005AC33890") == 54, "Part 2 test input 2");
	aoc::test(Part2("880086C3E88112") == 7, "Part 2 test input 3");
	aoc::test(Part2("CE00C43D881120") == 9, "Part 2 test input 4");
	aoc::test(Part2("D8005AC2A8F0") == 1, "Part 2 test input 5");
	aoc::test(Part2("F600BC2D8F") == 0, "Part 2 test input 6");
	aoc::test(Part2("9C005AC2F8F0") == 0, "Part 2 test input 7");
	aoc::test(Part2("9C0141080250320F1802104A08") == 1, "Part 2 test input 8");

	if (auto input = aoc::get_input("inputs/2021/Day16.txt", argc, argv)) {
		auto lines = aoc::by_line(*input);

		std::cout << "Part 1: " << Part1(lines[0]) << std::endl;
		std::cout << "Part 2: " << Part2(lines[0]) << std::endl;
	} else {
		return 1;
	}
}
