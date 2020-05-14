#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <FreeImage.h>
#include "..\core\data\waveform.h"
#include "..\defines.h"
#include "..\core\deeplearning\trainingfield.h"

namespace audioengineer {
	namespace utils {

		class FileUtils
		{
		public:
			static std::string read_file(const std::string& filename);
			static BYTE* read_image(const std::string& filename, uint* width, uint* height, uint* channels);

			static void write_waveform(const core::Waveform& waveform, const std::string& filename);
			static core::Waveform read_waveform(const std::string& filename);

			static void write_layer(const core::Layer& layer, const std::string& filename);
			static void write_joint(const core::Joint& joint, const std::string& filename);
			static void write_neuralnetwork(const core::NeuralNetwork& neuralnetwork, const std::string& filename);
			static void write_networktrainer(const core::NetworkTrainer& networktrainer, const std::string& filename);
			static void write_trainingfield(const core::TrainingField& trainingfield, const std::string& filename);
			static core::Layer read_layer(const std::string& filename);
			static core::Joint read_joint(const std::string& filename);
			static core::NeuralNetwork read_neuralnetwork(const std::string& filename);
			static core::NetworkTrainer read_networktrainer(const std::string& filename);
			static core::TrainingField read_trainingfield(const std::string& filename);

		};
	}
}