#include "application.h"

namespace audioengineer {

	Application::Application()
		:
		sampleRate(8000),
		waveformSize(1 << 10),
		bufferSize(1 << 11),
		timeSize(1 << 14),
		freqSize(1 << 8),
		windowSize(1 << 9)
	{
		using namespace std;
		using namespace graphics;
		using namespace core;

#if 1
		Window::create(1368, 912, "AudioEngineer");
#else
		Window::create(0, 0, "Test", WIN_FULLSCREEN);
#endif

		eventHandler = EventHandler::getDefaultHandler();

		eventHandler->addEventListener(&Application::keyListener, this);
		eventHandler->addEventListener(&Application::errorListener, this);
		eventHandler->addEventListener(&Application::guiListener, this);
		eventHandler->addEventListener(&Application::resizeListener, this);
		eventHandler->addEventListener(&Application::audioListener, this);

		waveform = new Waveform(sampleRate, waveformSize);
		spectrum = new Spectrum(sampleRate, waveformSize);
		waveformRecord = new Waveform(sampleRate, timeSize);
		spectrogram = nullptr;
		spectrumRecord = new Spectrum(sampleRate, timeSize);

		core::FFT.useContext(true);
		core::FFT.prepareContext(waveform->getSize());

		PortAudio::initialize();
		istream = new InputStream(0, 1, 256, sampleRate, bufferSize);

		cout << "Sample rate: " << sampleRate << " Hz" << endl;
		cout << "Inputstream buffer size: " << bufferSize << " samples (" << (double)bufferSize / sampleRate << " seconds)" << endl;
		cout << "Waveform size: " << waveformSize << " samples (" << (double)waveformSize / sampleRate << " seconds)" << endl;
		cout << "Record waveform size: " << timeSize << " samples (" << (double)timeSize / sampleRate << " seconds)" << endl;
		cout << "Spectrogram window size: " << windowSize << " samples (" << (double)windowSize / sampleRate << " seconds)" << endl;

		graphRenderer = new GraphRenderer();
		guiRenderer = new GuiRenderer(Window::getWidth(), Window::getHeight());
		
		graphRenderer->setScreenSize(Window::getWidth(), Window::getHeight());
		guiRenderer->setScreenSize(Window::getWidth(), Window::getHeight());

		Texture::loadTexture("background", "res/images/gui/background.png");
		Texture::loadTexture("dashboard_panel", "res/images/gui/dashboard_panel.png");
		Texture::loadTexture("monitor_on", "res/images/gui/monitor_on.png");
		Texture::loadTexture("monitor_off", "res/images/gui/monitor_off.png");
		Texture::loadTexture("mic_off", "res/images/gui/mic_off.png");
		Texture::loadTexture("mic_on", "res/images/gui/mic_on.png");
		Texture::loadTexture("training_off", "res/images/gui/training_off.png");
		Texture::loadTexture("training_on", "res/images/gui/training_on.png");
		Texture::loadTexture("classify_off", "res/images/gui/classify_off.png");
		Texture::loadTexture("classify_on", "res/images/gui/classify_on.png");
		Texture::loadTexture("vowels", "res/images/gui/vowels.png");
		Texture::loadTexture("vowel_a", "res/images/gui/vowel_a.png");
		Texture::loadTexture("vowel_e", "res/images/gui/vowel_e.png");
		Texture::loadTexture("vowel_i", "res/images/gui/vowel_i.png");
		Texture::loadTexture("vowel_o", "res/images/gui/vowel_o.png");
		Texture::loadTexture("vowel_u", "res/images/gui/vowel_u.png");
		Texture::loadTexture("vowel_a_sel", "res/images/gui/vowel_a_sel.png");
		Texture::loadTexture("vowel_e_sel", "res/images/gui/vowel_e_sel.png");
		Texture::loadTexture("vowel_i_sel", "res/images/gui/vowel_i_sel.png");
		Texture::loadTexture("vowel_o_sel", "res/images/gui/vowel_o_sel.png");
		Texture::loadTexture("vowel_u_sel", "res/images/gui/vowel_u_sel.png");
		Texture::loadTexture("learn_on", "res/images/gui/learn_on.png");
		Texture::loadTexture("learn_off", "res/images/gui/learn_off.png");
		Texture::loadTexture("save", "res/images/gui/save.png");
		Texture::loadTexture("load", "res/images/gui/load.png");

		graphRenderer->setWaveformTextureSize(1328, 332);
		graphRenderer->setSpectrumTextureSize(1328, 332);
		graphRenderer->setSpectrogramTextureSize(1328, 332);

		backgroundPanel = new Panel(Texture::getTexture("background"));
		panel1 = new Panel(Texture::getTexture("dashboard_panel"));
		panel2 = new Panel(Texture::getTexture("dashboard_panel"));
		
		waveformPanel = new Panel(graphRenderer->getWaveformTexture());
		spectrumPanel = new Panel(graphRenderer->getSpectrumTexture());
		vowels = new Panel(Texture::getTexture("vowels"));
		spectrogramPanel = new Panel(graphRenderer->getSpectrogramTexture());
		button1 = new ToggleButton(Texture::getTexture("monitor_off"), Texture::getTexture("monitor_on"));
		button2 = new ToggleButton(Texture::getTexture("mic_off"), Texture::getTexture("mic_on"));
		button3 = new ToggleButton(Texture::getTexture("training_off"), Texture::getTexture("training_on"));
		button4 = new ToggleButton(Texture::getTexture("classify_off"), Texture::getTexture("classify_on"));
		vowel_a = new Button(Texture::getTexture("vowel_a"));
		vowel_e = new Button(Texture::getTexture("vowel_e"));
		vowel_i = new Button(Texture::getTexture("vowel_i"));
		vowel_o = new Button(Texture::getTexture("vowel_o"));
		vowel_u = new Button(Texture::getTexture("vowel_u"));
		learn = new Button(Texture::getTexture("learn_off"));
		save = new Button(Texture::getTexture("save"));
		load = new Button(Texture::getTexture("load"));
		tbg = new TButtonGroup(eventHandler, { button1, button2, button3 });
		button1->scale(0.5f, 0.5f);
		button2->scale(0.5f, 0.5f);
		button3->scale(0.5f, 0.5f);
		button4->scale(0.5f, 0.5f);
		learn->scale(0.5f, 0.5f);
		save->scale(0.5f, 0.5f);
		load->scale(0.5f, 0.5f);
		panel1->setPosition(18, 162);
		panel2->setPosition(18, 508);
		vowels->setPosition(258, 166);
		vowel_a->setPosition(156, 200);
		vowel_e->setPosition(266, 200);
		vowel_i->setPosition(376, 200);
		vowel_o->setPosition(486, 200);
		vowel_u->setPosition(596, 200);
		learn->setPosition(19, 100);
		save->setPosition(74, 100);
		load->setPosition(129, 100);
		waveformPanel->setPosition(19, 163);
		spectrumPanel->setPosition(19, 509);
		spectrogramPanel->setPosition(19, 509);
		button1->setPosition(1135, 100);
		button2->setPosition(1190, 100);
		button3->setPosition(1245, 100);
		button4->setPosition(1300, 100);
		vowel_a->setRelativeTo(vowels);
		vowel_e->setRelativeTo(vowels);
		vowel_i->setRelativeTo(vowels);
		vowel_o->setRelativeTo(vowels);
		vowel_u->setRelativeTo(vowels);

		mainScene = new Scene();
		mainScene->addComponent(backgroundPanel);
		mainScene->addComponent(panel1);
		mainScene->addComponent(panel2);
		mainScene->addComponent(button1);
		mainScene->addComponent(button2);
		mainScene->addComponent(button3);
		mainScene->addComponent(button4);
		mainScene->addComponent(learn);
		mainScene->addComponent(save);
		mainScene->addComponent(load);
		learningScene = new Scene();
		learningScene->addComponent(vowels);
		learningScene->addComponent(vowel_a);
		learningScene->addComponent(vowel_e);
		learningScene->addComponent(vowel_i);
		learningScene->addComponent(vowel_o);
		learningScene->addComponent(vowel_u);
		learningScene->setVisible(false);
		learningScene->setEnabled(false);

		learning = false;
		classifying = false;
		Metaparameters params;
		params.hiddenLayerCountMax = 2;
		params.neuronCountMax = 40;
		tf = TrainingField(timeSize / 2, 5, 16, params);
		tf.addNetworkTrainer(NetworkTrainer(200, 5));
	}

	Application::~Application()
	{
		eventHandler->removeEventListener(&Application::keyListener, this);
		eventHandler->removeEventListener(&Application::errorListener, this);
		eventHandler->removeEventListener(&Application::guiListener, this);
		eventHandler->removeEventListener(&Application::resizeListener, this);
		eventHandler->removeEventListener(&Application::audioListener, this);

		delete graphRenderer;
		delete guiRenderer;

		delete backgroundPanel;
		delete panel1;
		delete panel2;
		delete waveformPanel;
		delete spectrumPanel;
		delete spectrogramPanel;
		delete button1;
		delete button2;
		delete button3;
		delete button4;
		delete vowel_a;
		delete vowel_e;
		delete vowel_i;
		delete vowel_o;
		delete vowel_u;
		delete learn;
		delete save;
		delete load;
		delete tbg;
		delete mainScene;
		delete learningScene;

		delete waveform;
		delete spectrum;
		delete waveformRecord;
		delete spectrogram;
		delete spectrumRecord;

		delete istream;
		core::PortAudio::terminate();

		graphics::Window::destroy();
	}

	void Application::run()
	{
		double previousTime = glfwGetTime();
		int frameCount = 0;
		while (graphics::Window::isOpen()) {
			double currentTime = glfwGetTime();
			frameCount++;
			if (currentTime - previousTime >= 1.0)
			{
				//std::cout << frameCount << " FPS" << std::endl;
				frameCount = 0;
				previousTime = currentTime;
			}
			update();
			render();
		}
	}

	void Application::update()
	{
		if (button1->isPressed()) {
			istream->streamToWaveform(*waveform);
			core::FFT(*waveform, *spectrum);
		}
	}

	void Application::render()
	{
		graphics::Window::clear();

		if (button1->isPressed()) {
			graphRenderer->render(*waveform);
			graphRenderer->render(*spectrum);
		}
		if (button2->isPressed()) {
			graphRenderer->render(*waveformRecord);
			if (spectrogram) {
				graphRenderer->render(*spectrogram, false);
				delete spectrogram;
				spectrogram = nullptr;
			}
		}
		if (button3->isPressed()) {
			graphRenderer->render(*waveformRecord);
			if (spectrogram) {
				graphRenderer->render(*spectrogram, false);
				delete spectrogram;
				spectrogram = nullptr;
			}
		}
		if (button4->isPressed()) {
			graphRenderer->render(*waveformRecord);
			if (spectrogram) {
				graphRenderer->render(*spectrogram, false);
				delete spectrogram;
				spectrogram = nullptr;
			}
		}
		guiRenderer->render(mainScene);
		guiRenderer->render(learningScene);

		graphics::Window::update();
	}

	void Application::keyListener(KeyboardEvent& e)
	{
		if (e.getAction() == GLFW_PRESS) {
			if (e.getButton() == GLFW_KEY_ESCAPE) {
				eventHandler->submitEvent(WinCloseEvent());
			}
		}
		if (learning) {
			if (e.getAction() == GLFW_RELEASE) {
				if (e.getButton() == GLFW_KEY_A) {
					vowel_a->click();
				}
				else if (e.getButton() == GLFW_KEY_E) {
					vowel_e->click();
				}
				else if (e.getButton() == GLFW_KEY_I) {
					vowel_i->click();
				}
				else if (e.getButton() == GLFW_KEY_O) {
					vowel_o->click();
				}
				else if (e.getButton() == GLFW_KEY_U) {
					vowel_u->click();
				}
			}
		}
	}

	void Application::errorListener(ErrorEvent& e)
	{
		std::cout << e.getDescription() << std::endl;
	}

	void Application::guiListener(GuiEvent& e)
	{
		if (e.getComponent() == button1) {
			if (e.getDescription() == TBUTTON_ON) {
				graphRenderer->prepareWaveformAxis(waveform->getSize());
				graphRenderer->prepareSpectrumAxis(spectrum->getSize(), false);
				istream->start();
				mainScene->addComponent(waveformPanel);
				mainScene->addComponent(spectrumPanel);
			}
			else if (e.getDescription() == TBUTTON_OFF) {
				mainScene->removeComponent(waveformPanel);
				mainScene->removeComponent(spectrumPanel);
				istream->stop();
			}
		}
		else if (e.getComponent() == button2) {
			if (e.getDescription() == TBUTTON_ON) {
				waveformRecord->clear();
				graphRenderer->prepareWaveformAxis(waveformRecord->getSize());
				mainScene->addComponent(waveformPanel);
				mainScene->addComponent(spectrogramPanel);
				istream->recordToWaveform(*waveformRecord);
				istream->start();
			}
			else if (e.getDescription() == TBUTTON_OFF) {
				mainScene->removeComponent(waveformPanel);
				mainScene->removeComponent(spectrogramPanel);
				istream->stop();
			}
		}
		else if (e.getComponent() == button3) {
			if (e.getDescription() == TBUTTON_ON) {
				learning = true;
				waveformRecord->clear();
				graphRenderer->prepareWaveformAxis(waveformRecord->getSize());
				mainScene->addComponent(waveformPanel);
				mainScene->addComponent(spectrogramPanel);
				istream->recordToWaveform(*waveformRecord);
				istream->start();
			}
			else if (e.getDescription() == TBUTTON_OFF) {
				learning = false;
				mainScene->removeComponent(waveformPanel);
				mainScene->removeComponent(spectrogramPanel);
				istream->stop();
			}
		}
		else if (e.getComponent() == button4) {
			if (e.getDescription() == TBUTTON_ON) {
				classifying = true;
				waveformRecord->clear();
				graphRenderer->prepareWaveformAxis(waveformRecord->getSize());
				mainScene->addComponent(waveformPanel);
				mainScene->addComponent(spectrogramPanel);
				istream->recordToWaveform(*waveformRecord);
				istream->start();
			}
			else if (e.getDescription() == TBUTTON_OFF) {
				classifying = false;
				mainScene->removeComponent(waveformPanel);
				mainScene->removeComponent(spectrogramPanel);
				istream->stop();
			}
		}
		else if (e.getComponent() == vowel_a) {
			button3->setPressed(false);
			button4->setPressed(false);
			mainScene->setEnabled(true);
			learningScene->setEnabled(false);
			learningScene->setVisible(false);
			addTrainingData({ 1.f, 0.f, 0.f, 0.f, 0.f });
		}
		else if (e.getComponent() == vowel_e) {
			button3->setPressed(false);
			button4->setPressed(false);
			mainScene->setEnabled(true);
			learningScene->setEnabled(false);
			learningScene->setVisible(false);
			addTrainingData({ 0.f, 1.f, 0.f, 0.f, 0.f });
		}
		else if (e.getComponent() == vowel_i) {
			button3->setPressed(false);
			button4->setPressed(false);
			mainScene->setEnabled(true);
			learningScene->setEnabled(false);
			learningScene->setVisible(false);
			addTrainingData({ 0.f, 0.f, 1.f, 0.f, 0.f });
		}
		else if (e.getComponent() == vowel_o) {
			button3->setPressed(false);
			button4->setPressed(false);
			mainScene->setEnabled(true);
			learningScene->setEnabled(false);
			learningScene->setVisible(false);
			addTrainingData({ 0.f, 0.f, 0.f, 1.f, 0.f });
		}
		else if (e.getComponent() == vowel_u) {
			button3->setPressed(false);
			button4->setPressed(false);
			mainScene->setEnabled(true);
			learningScene->setEnabled(false);
			learningScene->setVisible(false);
			addTrainingData({ 0.f, 0.f, 0.f, 0.f, 1.f });
		}
		else if (e.getComponent() == learn) {
			train();
		}
		else if (e.getComponent() == save) {
			utils::FileUtils::write_trainingfield(tf, "trainingfield/tf");
		}
		else if (e.getComponent() == load) {
			tf = utils::FileUtils::read_trainingfield("trainingfield/tf");
		}
		if (classifying) {
			vowel_a->setTexture(graphics::Texture::getTexture("vowel_a"));
			vowel_e->setTexture(graphics::Texture::getTexture("vowel_e"));
			vowel_i->setTexture(graphics::Texture::getTexture("vowel_i"));
			vowel_o->setTexture(graphics::Texture::getTexture("vowel_o"));
			vowel_u->setTexture(graphics::Texture::getTexture("vowel_u"));
		}
	}

	void Application::resizeListener(WinResizeEvent& e)
	{
		graphRenderer->setScreenSize(e.getWidth(), e.getHeight());
		guiRenderer->setScreenSize(e.getWidth(), e.getHeight());
	}

	void Application::audioListener(AudioEvent& e)
	{
		if (e.getDescription() == WAVEFORM_RECORDED) {
			istream->stop();
			utils::FileUtils::write_waveform(*waveformRecord, "test");
			core::Waveform a = utils::FileUtils::read_waveform("test");
			delete spectrogram;
			spectrogram = new core::Spectrogram(*waveformRecord, windowSize, windowSize / 16 * 15);
			if (learning) {
				core::FFT(*waveformRecord, *spectrumRecord);
				mainScene->setEnabled(false);
				learningScene->setVisible(true);
				learningScene->setEnabled(true);
			}
			if (classifying) {
				core::FFT(*waveformRecord, *spectrumRecord);
				core::NeuralNetwork nn = tf.getBestIndividual();
				mainScene->setEnabled(false);
				vec input = spectrumRecord->getMagnitude();
				float max = 0.f;
				for (uint i = 0; i < input.size(); i++) {
					if (input[i] > max) {
						max = input[i];
					}
				}
				max = 1.f / max;
				for (uint i = 0; i < input.size(); i++) {
					input[i] *= max;
				}
				vec output = nn(input);
				std::cout << "A: " << output[0] << std::endl;
				std::cout << "E: " << output[1] << std::endl;
				std::cout << "I: " << output[2] << std::endl;
				std::cout << "O: " << output[3] << std::endl;
				std::cout << "U: " << output[4] << std::endl;
				uint index = 0;
				max = output[0];
				for (uint i = 1; i < 5; i++) {
					if (output[i] > max) {
						max = output[i];
						index = i;
					}
				}
				mainScene->setEnabled(false);
				learningScene->setVisible(true);
				learningScene->setEnabled(true);
				switch (index) {
				case 0:
					vowel_a->setTexture(graphics::Texture::getTexture("vowel_a_sel"));
					break;
				case 1:
					vowel_e->setTexture(graphics::Texture::getTexture("vowel_e_sel"));
					break;
				case 2:
					vowel_i->setTexture(graphics::Texture::getTexture("vowel_i_sel"));
					break;
				case 3:
					vowel_o->setTexture(graphics::Texture::getTexture("vowel_o_sel"));
					break;
				case 4:
					vowel_u->setTexture(graphics::Texture::getTexture("vowel_u_sel"));
					break;
				}
			}
		}
	}

	void Application::train()
	{
		for (uint i = 0; i < 50; i++) {
			tf.simulate();
			std::cout << "Simulation " << i << std::endl;
		}
	}

	void Application::addTrainingData(const std::vector<float>& target)
	{
		vec input = spectrumRecord->getMagnitude();
		float max = 0.f;
		for (uint i = 0; i < input.size(); i++) {
			if (input[i] > max) {
				max = input[i];
			}
		}
		max = 1.f / max;
		for (uint i = 0; i < input.size(); i++) {
			input[i] *= max;
		}
		tf.addTrainingData(0, input, target);
	}
}