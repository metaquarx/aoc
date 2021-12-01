// SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: Apache-2.0

#include <optional>
#include <string>
#include <vector>

#pragma once

namespace aoc {

void test(bool result, const std::string &message);

std::optional<std::string> get_input(const std::string &filename, int argc, char **argv);

std::optional<std::string> read_file(const std::string &filename);

std::vector<std::string> by_line(const std::string &input);

std::vector<int> to_int(const std::vector<std::string> &input);

}  // namespace aoc
