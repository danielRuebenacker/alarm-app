#pragma once
#include <variant>
#include <vector>
#include <string>

using std::vector;

struct PuzzleResponse {
    // can have ints, strings or grids (vectors of vectors)
    std::variant<int, std::string, vector<vector<int>>> value;

    PuzzleResponse(int val) : value(val) {}
    PuzzleResponse(std::string val) : value(val) {}
    PuzzleResponse(vector<vector<int>> val) : value(val) {}
};
