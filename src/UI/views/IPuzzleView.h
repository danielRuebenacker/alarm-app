#pragma once
#include <functional>

#include "../interfaces/IPuzzle.h"
#include "../types/PuzzleType.h"
#include "../types/PuzzleResponse.h"

#include "View.h"

class IPuzzleView : public View {
    public:
        virtual ~IPuzzleView() = default;
        virtual void updateTimeoutBar(int secondsLeft) = 0;
        virtual void loadPuzzle(const PuzzleType& puzzleType, const IPuzzle& puzzle) = 0;
        virtual void setOnSubmitCallback(std::function<void(const PuzzleResponse& response)> callback) = 0;
};
