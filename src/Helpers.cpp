// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include "Helpers.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

namespace aoc {


void test(bool result, const std::string &message) {
	std::cout << (result ? "Passed " : "Failed ") << message << std::endl;
}

std::optional<std::string> get_input(const std::string &filename, int argc, char **argv) {
	if (argc > 1) {
		auto file = read_file(argv[1]);
		if (!file) {
			std::cout << "Could not read file specified in arguments, falling back to " << filename << std::endl;
		} else {
			return file;
		}
	}

	return read_file(filename);
}

std::optional<std::string> read_file(const std::string &filename) {
	std::ifstream file(filename);

	if (!file) {
		std::cout << "Unable to open file " << filename << std::endl;
		return std::nullopt;
	}

	std::string output;

	file.seekg(0, std::ios::end);
	// When directories are passed as a filename, this seeks to 2^63-1. Im not sure how else to avoid this
	if (file.tellg() > 99999) {
		std::cout << "Invalid file " << filename << std::endl;
		return std::nullopt;
	}

	output.resize(static_cast<unsigned long>(file.tellg()));
	file.seekg(0);

	if (!file.read(output.data(), static_cast<long>(output.size()))) {
		std::cout << "Unable to read from file " << filename << std::endl;
		return std::nullopt;
	}

	return output;
}

std::vector<std::string> by_line(const std::string &input) {
	std::vector<std::string> output;
	std::stringstream ss(input);

	std::string line;
	while (std::getline(ss, line, '\n')) {
		output.push_back(line);
	}

	return output;
}

std::vector<int> to_int(const std::vector<std::string> &input) {
	std::vector<int> output;

	for (auto line : input) {
		output.push_back(std::stoi(line));
	}

	return output;
}

}  // namespace aoc
