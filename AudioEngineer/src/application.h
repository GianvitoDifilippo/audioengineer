#pragma once

#include "core\processing\fourier.h"
#include "core\streams\inputstream.h"
#include "events\eventsystem.h"
#include "graphics\gui\gui.h"
#include "graphics\renderers\graphrenderer.h"
#include "graphics\renderers\guirenderer.h"
#include "graphics\window.h"
#include "graphics\renderers\graphrenderer.h"
#include "graphics\renderers\guirenderer.h"
#include "core\processing\fourier.h"
#include "core\deeplearning\trainingfield.h"
#include "defines.h"

namespace audioengineer {

	class Application
	{
	private:
		const double sampleRate;
		const uint bufferSize;
		const uint waveformSize;
		const uint timeSize;
		const uint freqSize;
		const uint windowSize;

		EventHandler* eventHandler;
		core::InputStream* istream;

		bool learning;
		bool classifying;
		core::TrainingField tf;

		core::Waveform* waveform;
		core::Spectrum* spectrum;
		core::Waveform* waveformRecord;
		core::Spectrogram* spectrogram;
		core::Spectrum* spectrumRecord;

		graphics::Panel* backgroundPanel;
		graphics::Panel* panel1;
		graphics::Panel* panel2;
		graphics::Panel* vowels;
		graphics::Button* vowel_a;
		graphics::Button* vowel_e;
		graphics::Button* vowel_i;
		graphics::Button* vowel_o;
		graphics::Button* vowel_u;
		graphics::Button* learn;
		graphics::Button* save;
		graphics::Button* load;
		graphics::Panel* waveformPanel;
		graphics::Panel* spectrumPanel;
		graphics::Panel* spectrogramPanel;
		graphics::ToggleButton* button1;
		graphics::ToggleButton* button2;
		graphics::ToggleButton* button3;
		graphics::ToggleButton* button4;
		graphics::TButtonGroup* tbg;
		graphics::Scene* mainScene;
		graphics::Scene* learningScene;

		graphics::GraphRenderer* graphRenderer;
		graphics::GuiRenderer* guiRenderer;

	public:
		Application();
		~Application();

		void run();

	private:
		void update();
		void render();

		void addTrainingData(const std::vector<float>& target);
		void train();

		void keyListener(KeyboardEvent& e);
		void errorListener(ErrorEvent& e);
		void guiListener(GuiEvent& e);
		void resizeListener(WinResizeEvent& e);
		void audioListener(AudioEvent& e);
	};
}