#pragma once

#include "../views/IPuzzleView.h"
#include <lv/lv.hpp>
#include <cstdlib>
#include <functional>
#include <string>

class LvglPuzzleView : public IPuzzleView {
  private:
	// the on-screen keyboard is the last child of the layout and grows into
	// whatever vertical space is left, so it can never sit on top of the
	// submit button (a fixed 220px keyboard did exactly that on a 320px screen)
	lv::ObjectView root_;
	lv::Bar timeoutBar_;
	lv::Label questionLabel_;
	lv::Label feedbackLabel_;
	lv::Textarea answerInput_;
	lv::Keyboard keyboard_;
	lv::Timer hintTimer_;

	// how long a memory grid stays on screen before it is taken away again
	static constexpr uint32_t kHintVisibleMs = 4000;

	// what the textarea holds decides how the answer is read back
	enum class AnswerKind { Number, Text };
	AnswerKind answerKind_ = AnswerKind::Number;

	// the question is swapped out for the hint while the hint is showing, so it
	// has to be kept to put back
	std::string question_;
	bool hintShowing_ = false;

	std::function<void(const PuzzleResponse&)> onSubmitCallback_;
	std::function<void()> onAnyInputCallback_;

	void onHintExpired() {
		hintShowing_ = false;
		questionLabel_.font(&lv_font_montserrat_24);
		questionLabel_.text(question_.c_str());
		lv_obj_set_style_text_color(questionLabel_.get(), lv_color_hex(0x000000), LV_PART_MAIN);
	}

	void handleAnyInput() {
		// typing again means the previous verdict is stale
		clearFeedback();
		// copy before invoking: the callback might destroy this view
		auto callback = onAnyInputCallback_;
		if (callback) callback();
	}

	void handleSubmit() {
		const char* raw = answerInput_.text();
		// an empty field is not an attempt, so there is nothing to report
		if (!raw || raw[0] == '\0') return;

		PuzzleResponse response = answerKind_ == AnswerKind::Text
									  ? PuzzleResponse(std::string(raw))
									  : PuzzleResponse(std::atoi(raw));
		// copy before invoking: the callback may destroy this view (it
		// navigates away once the answer is right)
		auto callback = onSubmitCallback_;
		if (callback) callback(response);
	}

	void clearFeedback() {
		feedbackLabel_.text("");
		lv_obj_set_style_border_width(answerInput_.get(), 1, LV_PART_MAIN);
		lv_obj_set_style_border_color(answerInput_.get(), lv_color_hex(0x9E9E9E), LV_PART_MAIN);
		if (!hintShowing_) {
			lv_obj_set_style_text_color(questionLabel_.get(), lv_color_hex(0x000000), LV_PART_MAIN);
		}
		lv_obj_set_style_translate_x(answerInput_.get(), 0, LV_PART_MAIN);
		lv::anim_delete(answerInput_, nullptr);
	}

	void shakeAnswer() {
		// three swings that start and end centred on the box
		lv::anim_delete(answerInput_, nullptr);
		lv::Anim()
			.var(answerInput_)
			.exec([](void* obj, int32_t value) {
				const int32_t swing = value <= 12 ? value : 24 - value;
				lv_obj_set_style_translate_x(static_cast<lv_obj_t*>(obj), swing - 6, 0);
			})
			.values(0, 24)
			.duration(220)
			.on_complete([](lv_anim_t* a) {
				lv_obj_set_style_translate_x(static_cast<lv_obj_t*>(a->var), 0, 0);
			})
			.start();
	}

	void pulseFeedback() {
		lv::Anim()
			.exec_opa(feedbackLabel_)
			.values(0, 255)
			.duration(160)
			.playback(0)
			.start();
	}

  public:
	LvglPuzzleView(lv::ObjectView parent = lv::screen_active()) {
		root_ = lv::vbox(parent).fill().gap(6).padding(8);

		timeoutBar_ = lv::Bar::create(root_)
						  .fill_width()
						  .height(10)
						  .range(0, 100)
						  .value(100);

		questionLabel_ = lv::Label::create(root_)
							 .text("...")
							 .font(&lv_font_montserrat_24);

		feedbackLabel_ = lv::Label::create(root_).text("");

		answerInput_ = lv::Textarea::create(root_)
						   .fill_width()
						   .height(38)
						   .one_line(true)
						   .max_length(8)
						   .accepted_chars("0123456789")
						   .placeholder("Answer");

		lv::Button::create(root_)
			.fill_width()
			.height(34)
			.text("Submit")
			.on_click<&LvglPuzzleView::handleSubmit>(this);

		keyboard_ = lv::Keyboard::create(root_)
						.fill_width()
						.grow(1)
						.textarea(answerInput_)
						.mode_number();

		// any key press (on-screen keyboard button, physical key or backspace)
		// counts as activity and resets the timeout
		answerInput_.on<&LvglPuzzleView::handleAnyInput>(LV_EVENT_VALUE_CHANGED, this);
		answerInput_.on<&LvglPuzzleView::handleAnyInput>(LV_EVENT_KEY, this);
		keyboard_.on<&LvglPuzzleView::handleAnyInput>(LV_EVENT_VALUE_CHANGED, this);
		// the keypad's tick mark is an implicit "submit" for a one line field.
		// Only the textarea is bound: the keyboard forwards its own ready event
		// to the textarea, so binding both would submit twice.
		answerInput_.on<&LvglPuzzleView::handleSubmit>(LV_EVENT_READY, this);
	}

	void updateTimeoutBar(int percentLeft) override {
		timeoutBar_.value(percentLeft);
	}

	void loadPuzzle(const PuzzleType& puzzleType, const IPuzzle& puzzle) override {
		clearFeedback();
		hintTimer_.del();
		hintShowing_ = false;
		question_ = puzzle.toString();
		questionLabel_.font(&lv_font_montserrat_24);
		questionLabel_.text(question_.c_str());

		// each puzzle type dictates both what can be typed and how it is read
		if (puzzleType == PuzzleType::CAPTCHA) {
			answerKind_ = AnswerKind::Text;
			answerInput_.accepted_chars(nullptr).max_length(24).placeholder("Word");
			keyboard_.mode_text_lower();
		} else {
			answerKind_ = AnswerKind::Number;
			answerInput_.accepted_chars("0123456789").max_length(8).placeholder("Answer");
			keyboard_.mode_number();
		}

		answerInput_.text("");
	}

	void showHint(const std::string& hint) override {
		// shown in place of the question: it costs no extra widget, and the
		// smaller font keeps a multi line grid from squeezing the keypad
		hintShowing_ = true;
		questionLabel_.text(hint.c_str());
		questionLabel_.font(&lv_font_montserrat_14);
		lv_obj_set_style_text_color(questionLabel_.get(), lv_color_hex(0x1565C0), LV_PART_MAIN);
		// RAII: if this view dies first the timer dies with it
		hintTimer_ = lv::timer_once<&LvglPuzzleView::onHintExpired>(kHintVisibleMs, this);
	}

	void showAnswerFeedback(bool correct) override {
		const lv_color_t colour = lv_color_hex(correct ? 0x2E7D32 : 0xD32F2F);

		// clear before styling: writing to the textarea emits VALUE_CHANGED,
		// which would immediately wipe the colours we are about to set
		answerInput_.text("");
		clearFeedback();

		feedbackLabel_.text(correct ? "Correct!" : "Wrong - try again");
		lv_obj_set_style_text_color(feedbackLabel_.get(), colour, LV_PART_MAIN);
		lv_obj_set_style_border_color(answerInput_.get(), colour, LV_PART_MAIN);
		lv_obj_set_style_border_width(answerInput_.get(), 2, LV_PART_MAIN);

		if (correct) {
			lv_obj_set_style_text_color(questionLabel_.get(), colour, LV_PART_MAIN);
			pulseFeedback();
		} else {
			shakeAnswer();
		}
	}

	void setOnSubmitCallback(std::function<void(const PuzzleResponse& response)> callback) override {
		onSubmitCallback_ = std::move(callback);
	}

	void setOnAnyInputCallback(std::function<void()> callback) override {
		onAnyInputCallback_ = std::move(callback);
	}
};
