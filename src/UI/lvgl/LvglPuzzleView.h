#pragma once

#include "../views/IPuzzleView.h"
#include <lv/lv.hpp>
#include <cstdlib>
#include <functional>

class LvglPuzzleView : public IPuzzleView {
  private:
	lv::Bar timeoutBar_;
	lv::Label questionLabel_;
	lv::Textarea answerInput_;
	lv::Keyboard keyboard_;

	std::function<void(const PuzzleResponse&)> onSubmitCallback_;
	std::function<void()> onAnyInputCallback_;

	void handleAnyInput() {
		// copy before invoking: the callback might destroy this view
		auto callback = onAnyInputCallback_;
		if (callback) callback();
	}

	void handleSubmit() {
		// read everything we need before invoking: the callback may destroy
		// this view (it navigates away on a correct answer)
		const char* raw = answerInput_.text();
		int answer = raw ? std::atoi(raw) : 0;
		// copy before invoking for the same reason
		auto callback = onSubmitCallback_;
		if (callback) callback(PuzzleResponse(answer));
	}

  public:
	LvglPuzzleView(lv::ObjectView parent = lv::screen_active()) {
		auto root = lv::vbox(parent).fill().gap(12).padding(12);

		timeoutBar_ = lv::Bar::create(root)
			.fill_width()
			.height(12)
			.range(0, 100)
			.value(100);

		questionLabel_ = lv::Label::create(root)
			.text("...")
			.font(&lv_font_montserrat_24);

		answerInput_ = lv::Textarea::create(root)
			.one_line(true)
			.max_length(8)
			.accepted_chars("0123456789-")
			.placeholder("Answer");

		lv::Button::create(root)
			.fill_width()
			.text("Submit")
			.on_click<&LvglPuzzleView::handleSubmit>(this);

		keyboard_ = lv::Keyboard::create(lv::screen_active())
			.textarea(answerInput_)
			.mode_number()
			.fill_width()
			.height(220)
			.align_bottom();

		// any key press (on-screen keyboard button, physical key or backspace)
		// counts as activity and resets the timeout
		keyboard_.on<&LvglPuzzleView::handleAnyInput>(LV_EVENT_VALUE_CHANGED, this);
		answerInput_.on<&LvglPuzzleView::handleAnyInput>(LV_EVENT_KEY, this);
	}

	void updateTimeoutBar(int percentLeft) override {
		timeoutBar_.value(percentLeft);
	}

	void loadPuzzle(const PuzzleType&, const IPuzzle& puzzle) override {
		questionLabel_.text(puzzle.toString().c_str());
		answerInput_.text("");
	}

	void setOnSubmitCallback(std::function<void(const PuzzleResponse& response)> callback) override {
		onSubmitCallback_ = std::move(callback);
	}

	void setOnAnyInputCallback(std::function<void()> callback) override {
		onAnyInputCallback_ = std::move(callback);
	}
};
