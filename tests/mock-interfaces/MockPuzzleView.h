#pragma once
#include <functional>
#include <string>

#include "src/UI/views/IPuzzleView.h"

class MockPuzzleView : public IPuzzleView {
  public:
	std::string question;
	int lastPercent = -1;
	int loadCount = 0;

	std::function<void(const PuzzleResponse&)> onSubmit;
	std::function<void()> onAnyInput;

	void updateTimeoutBar(int percentLeft) override { lastPercent = percentLeft; }

	void loadPuzzle(const PuzzleType&, const IPuzzle& puzzle) override {
		question = puzzle.toString();
		++loadCount;
	}

	void setOnSubmitCallback(std::function<void(const PuzzleResponse&)> callback) override {
		onSubmit = std::move(callback);
	}

	void setOnAnyInputCallback(std::function<void()> callback) override {
		onAnyInput = std::move(callback);
	}

	void submit(const PuzzleResponse& response) {
		if (onSubmit) onSubmit(response);
	}

	void pressAnyKey() {
		if (onAnyInput) onAnyInput();
	}
};
