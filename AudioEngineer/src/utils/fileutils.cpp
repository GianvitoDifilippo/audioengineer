#include "fileutils.h"

namespace audioengineer {
	namespace utils {

		std::string FileUtils::read_file(const std::string& filename)
		{
			FILE* file = fopen(filename.c_str(), "r");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return "";
			}
			fseek(file, 0, SEEK_END);
			long length = ftell(file);
			char* text = new char[length + 1];
			memset(text, 0, sizeof(char) * (length + 1));
			rewind(file);
			fread(text, sizeof(char), length, file);
			fclose(file);
			std::string result(text);
			delete[] text;
			return result;
		}

		BYTE* FileUtils::read_image(const std::string& filename, uint* width, uint* height, uint* channels)
		{
			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			FIBITMAP* dib = nullptr;
			fif = FreeImage_GetFileType(filename.c_str());
			if (fif == FIF_UNKNOWN)
				fif = FreeImage_GetFIFFromFilename(filename.c_str());
			if (fif == FIF_UNKNOWN) {
				std::cout << "Could not import image: unknown extension. " << filename << std::endl;
				return nullptr;
			}

			if (FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_Load(fif, filename.c_str());
			if (!dib) {
				std::cout << "Could not import image. " << filename << std::endl;
				return nullptr;
			}

			*width = FreeImage_GetWidth(dib);
			*height = FreeImage_GetHeight(dib);
			*channels = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib) / sizeof(BYTE);
			return FreeImage_GetBits(dib);
		}

		void FileUtils::write_waveform(const core::Waveform& waveform, const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "wb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return;
			}
			size_t size = waveform.getSize();
			double sampleRate = waveform.getSampleRate();
			fwrite(&size, sizeof(size_t), 1, file);
			fwrite(&sampleRate, sizeof(double), 1, file);
			fwrite(waveform.getSamples(), sizeof(float), size, file);
			fclose(file);
		}

		core::Waveform FileUtils::read_waveform(const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "rb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return core::Waveform(0.0, 0);
			}
			size_t size;
			double sampleRate;
			fread(&size, sizeof(size_t), 1, file);
			fread(&sampleRate, sizeof(double), 1, file);
			core::Waveform result(sampleRate, size);
			fread(result.getSamples(), sizeof(float), size, file);
			fclose(file);
			return result;
		}

		void FileUtils::write_layer(const core::Layer& layer, const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "wb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return;
			}
			const vec& neurons = layer.m_neurons;
			const std::vector<vec>& derivativesW = layer.m_derivativesW;
			const std::vector<vec>& derivativesB = layer.m_derivativesB;

			size_t size = neurons.size();
			fwrite(&size, sizeof(size_t), 1, file);
			fwrite(&neurons[0], sizeof(float), size, file);

			size = derivativesW.size();
			fwrite(&size, sizeof(size_t), 1, file);
			for (uint i = 0; i < derivativesW.size(); i++) {
				const vec& derivatives = derivativesW[i];
				size = derivatives.size();
				fwrite(&size, sizeof(size_t), 1, file);
				fwrite(&derivatives[0], sizeof(float), size, file);
			}

			size = derivativesB.size();
			fwrite(&size, sizeof(size_t), 1, file);
			for (uint i = 0; i < derivativesB.size(); i++) {
				const vec& derivatives = derivativesB[i];
				size = derivatives.size();
				fwrite(&size, sizeof(size_t), 1, file);
				fwrite(&derivatives[0], sizeof(float), size, file);
			}
			fclose(file);
		}

		core::Layer FileUtils::read_layer(const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "rb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return core::Layer();
			}
			size_t size;

			fread(&size, sizeof(size_t), 1, file);
			vec neurons(size);
			fread(&neurons[0], sizeof(float), size, file);
			fread(&size, sizeof(size_t), 1, file);
			std::vector<vec> derivativesW(size);
			for (uint i = 0; i < size; i++) {
				size_t sizeW;
				fread(&sizeW, sizeof(size_t), 1, file);
				vec& derivatives = derivativesW[i];
				derivatives.resize(sizeW);
				fread(&derivatives[0], sizeof(float), sizeW, file);
			}
			fread(&size, sizeof(size_t), 1, file);
			std::vector<vec> derivativesB(size);
			for (uint i = 0; i < size; i++) {
				size_t sizeB;
				fread(&sizeB, sizeof(size_t), 1, file);
				vec& derivatives = derivativesB[i];
				derivatives.resize(sizeB);
				fread(&derivatives[0], sizeof(float), sizeB, file);
			}
			fclose(file);

			core::Layer result;
			result.m_neurons = std::move(neurons);
			result.m_derivativesW = std::move(derivativesW);
			result.m_derivativesB = std::move(derivativesB);
			return result;
		}

		void FileUtils::write_joint(const core::Joint& joint, const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "wb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return;
			}
			uint inputCount = joint.m_inputCount;
			uint outputCount = joint.m_outputCount;
			const vec& weights = joint.m_weights;
			const vec& biases = joint.m_biases;
			const vec& weightsStep = joint.m_weightsStep;
			const vec& biasesStep = joint.m_biasesStep;

			fwrite(&inputCount, sizeof(uint), 1, file);
			fwrite(&outputCount, sizeof(uint), 1, file);
			size_t size = weights.size();
			fwrite(&size, sizeof(size_t), 1, file);
			fwrite(&weights[0], sizeof(float), size, file);
			size = biases.size();
			fwrite(&size, sizeof(size_t), 1, file);
			fwrite(&biases[0], sizeof(float), size, file);
			size = weightsStep.size();
			fwrite(&size, sizeof(size_t), 1, file);
			fwrite(&weightsStep[0], sizeof(float), size, file);
			size = biasesStep.size();
			fwrite(&size, sizeof(size_t), 1, file);
			fwrite(&biasesStep[0], sizeof(float), size, file);
			
			fclose(file);
		}

		core::Joint FileUtils::read_joint(const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "rb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return core::Joint();
			}
			uint inputCount;
			uint outputCount;
			size_t size;
			fread(&inputCount, sizeof(uint), 1, file);
			fread(&outputCount, sizeof(uint), 1, file);
			fread(&size, sizeof(size_t), 1, file);
			vec weights(size);
			fread(&weights[0], sizeof(float), size, file);
			fread(&size, sizeof(size_t), 1, file);
			vec biases(size);
			fread(&biases[0], sizeof(float), size, file);
			fread(&size, sizeof(size_t), 1, file);
			vec weightsStep(size);
			fread(&weightsStep[0], sizeof(float), size, file);
			fread(&size, sizeof(size_t), 1, file);
			vec biasesStep(size);
			fread(&biasesStep[0], sizeof(float), size, file);
			fclose(file);

			core::Joint result;
			result.m_inputCount = inputCount;
			result.m_outputCount = outputCount;
			result.m_weights = std::move(weights);
			result.m_biases = std::move(biases);
			result.m_weightsStep = std::move(weightsStep);
			result.m_biasesStep = std::move(biasesStep);
			return result;
		}

		void FileUtils::write_neuralnetwork(const core::NeuralNetwork& neuralnetwork, const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "wb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return;
			}
			const std::vector<uint>& topology = neuralnetwork.m_topology;
			const vec& derSigma = neuralnetwork.m_derSigma;
			size_t size = topology.size();
			
			fwrite(&size, sizeof(size_t), 1, file);
			fwrite(&topology[0], sizeof(uint), size, file);
			
			size = neuralnetwork.m_layers.size();
			fwrite(&size, sizeof(size_t), 1, file);
			for (uint i = 0; i < size; i++) {
				write_layer(neuralnetwork.m_layers[i], filename + "_layer" + std::to_string(i));
			}
			size = neuralnetwork.m_joints.size();
			fwrite(&size, sizeof(size_t), 1, file);
			for (uint i = 0; i < size; i++) {
				write_joint(neuralnetwork.m_joints[i], filename + "_joint" + std::to_string(i));
			}
			size = derSigma.size();
			fwrite(&size, sizeof(size_t), 1, file);
			fwrite(&derSigma[0], sizeof(float), size, file);
			fwrite(&neuralnetwork.m_learningRate, sizeof(float), 1, file);

			fclose(file);
		}

		core::NeuralNetwork FileUtils::read_neuralnetwork(const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "rb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return core::NeuralNetwork();
			}
			float learningRate;
			std::vector<core::Layer> layers;
			std::vector<core::Joint> joints;
			size_t size;

			fread(&size, sizeof(size_t), 1, file);
			std::vector<uint> topology(size);
			fread(&topology[0], sizeof(uint), size, file);
			fread(&size, sizeof(size_t), 1, file);
			layers.reserve(size);
			for (uint i = 0; i < size; i++) {
				layers.emplace_back(read_layer(filename + "_layer" + std::to_string(i)));
			}
			fread(&size, sizeof(size_t), 1, file);
			joints.reserve(size);
			for (uint i = 0; i < size; i++) {
				joints.emplace_back(read_joint(filename + "_joint" + std::to_string(i)));
			}
			fread(&size, sizeof(size_t), 1, file);
			vec derSigma(size);
			fread(&derSigma[0], sizeof(float), size, file);
			fread(&learningRate, sizeof(float), 1, file);
			fclose(file);
			
			core::NeuralNetwork result;
			result.m_learningRate = learningRate;
			result.m_topology = std::move(topology);
			result.m_layers = std::move(layers);
			result.m_joints = std::move(joints);
			result.m_derSigma = derSigma;
			return result;
		}

		void FileUtils::write_networktrainer(const core::NetworkTrainer& networktrainer, const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "wb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return;
			}
			uint iterations = networktrainer.m_iterations;
			uint batchSize = networktrainer.m_batchSize;
			size_t size = networktrainer.m_trainingData.size();

			fwrite(&iterations, sizeof(uint), 1, file);
			fwrite(&batchSize, sizeof(uint), 1, file);
			fwrite(&size, sizeof(size_t), 1, file);
			for (uint i = 0; i < size; i++) {
				size_t size1 = networktrainer.m_trainingData[i].first->size();
				fwrite(&size1, sizeof(size_t), 1, file);
				fwrite(&(networktrainer.m_trainingData[i].first->front()), sizeof(float), size1, file);

				size_t size2 = networktrainer.m_trainingData[i].second->size();
				fwrite(&size2, sizeof(size_t), 1, file);
				fwrite(&(networktrainer.m_trainingData[i].second->front()), sizeof(float), size2, file);
			}
			
			fclose(file);
		}

		core::NetworkTrainer FileUtils::read_networktrainer(const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "rb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return core::NetworkTrainer();
			}
			uint iterations;
			uint batchSize;
			size_t size;

			fread(&iterations, sizeof(uint), 1, file);
			fread(&batchSize, sizeof(uint), 1, file);
			fread(&size, sizeof(size_t), 1, file);
			std::vector<std::pair<vec*, vec*>> trainingData(size);

			for (uint i = 0; i < size; i++) {
				size_t size1;
				fread(&size1, sizeof(size_t), 1, file);
				vec first(size1);
				fread(&first[0], sizeof(float), size1, file);

				size_t size2;
				fread(&size2, sizeof(size_t), 1, file);
				vec second(size2);
				fread(&second[0], sizeof(float), size2, file);

				trainingData[i] = std::make_pair(new vec(std::move(first)), new vec(std::move(second)));
			}

			fclose(file);
			core::NetworkTrainer result;
			result.m_iterations = iterations;
			result.m_batchSize = batchSize;
			result.m_trainingData = std::move(trainingData);
			return result;
		}

		void FileUtils::write_trainingfield(const core::TrainingField& trainingfield, const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "wb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return;
			}
			uint populationSize = trainingfield.m_populationSize;
			float survivalRate = trainingfield.m_survivalRate;
			uint inputCount = trainingfield.m_inputCount;
			uint outputCount = trainingfield.m_outputCount;
			float allTimeBestFitness = trainingfield.m_allTimeBestFitness;
			const core::PopulationStats& stats = trainingfield.m_stats;
			const core::Metaparameters& parameters = trainingfield.m_parameters;
			const std::vector<core::NetworkTrainer>& trainers = trainingfield.m_trainers;
			const std::vector<core::NeuralNetwork>& population = trainingfield.m_population;
			const core::NeuralNetwork& best = trainingfield.m_best;
			const core::NeuralNetwork& allTimeBest = trainingfield.m_allTimeBest;

			fwrite(&populationSize, sizeof(uint), 1, file);
			fwrite(&survivalRate, sizeof(float), 1, file);
			fwrite(&inputCount, sizeof(uint), 1, file);
			fwrite(&outputCount, sizeof(uint), 1, file);
			fwrite(&allTimeBestFitness, sizeof(float), 1, file);
			fwrite(&stats, sizeof(core::PopulationStats), 1, file);
			fwrite(&parameters, sizeof(core::Metaparameters), 1, file);
			size_t size = trainers.size();
			fwrite(&size, sizeof(size_t), 1, file);
			for (uint i = 0; i < size; i++) {
				write_networktrainer(trainers[i], filename + "_trainer" + std::to_string(i));
			}
			size = population.size();
			fwrite(&size, sizeof(size_t), 1, file);
			for (uint i = 0; i < size; i++) {
				write_neuralnetwork(population[i], filename + "_net" + std::to_string(i));
			}
			write_neuralnetwork(best, filename + "_netBest");
			write_neuralnetwork(allTimeBest, filename + "_netAllTimeBest");
			fclose(file);
		}

		core::TrainingField FileUtils::read_trainingfield(const std::string& filename)
		{
			FILE* file = fopen(("res/" + filename + ".txt").c_str(), "rb");
			if (!file) {
				std::cout << "Could not open file: " << filename << std::endl;
				return core::TrainingField();
			}
			uint populationSize;
			float survivalRate;
			uint inputCount;
			uint outputCount;
			float allTimeBestFitness;
			core::PopulationStats stats;
			core::Metaparameters parameters;
			size_t size;

			fread(&populationSize, sizeof(uint), 1, file);
			fread(&survivalRate, sizeof(float), 1, file);
			fread(&inputCount, sizeof(uint), 1, file);
			fread(&outputCount, sizeof(uint), 1, file);
			fread(&allTimeBestFitness, sizeof(float), 1, file);
			fread(&stats, sizeof(core::PopulationStats), 1, file);
			fread(&parameters, sizeof(core::Metaparameters), 1, file);
			fread(&size, sizeof(size_t), 1, file);
			std::vector<core::NetworkTrainer> trainers;
			trainers.reserve(size);
			for (uint i = 0; i < size; i++) {
				trainers.emplace_back(read_networktrainer(filename + "_trainer" + std::to_string(i)));
			}
			fread(&size, sizeof(size_t), 1, file);
			std::vector<core::NeuralNetwork> population;
			population.reserve(size);
			for (uint i = 0; i < size; i++) {
				population.emplace_back(read_neuralnetwork(filename + "_net" + std::to_string(i)));
			}
			core::NeuralNetwork best = read_neuralnetwork(filename + "_netBest");
			core::NeuralNetwork allTimeBest = read_neuralnetwork(filename + "_netAllTimeBest");

			fclose(file);
			core::TrainingField result;
			result.m_populationSize = populationSize;
			result.m_survivalRate = survivalRate;
			result.m_inputCount = inputCount;
			result.m_outputCount = outputCount;
			result.m_allTimeBestFitness = allTimeBestFitness;
			result.m_stats = stats;
			result.m_parameters = parameters;
			result.m_trainers = std::move(trainers);
			result.m_population = std::move(population);
			result.m_best = std::move(best);
			result.m_allTimeBest = std::move(allTimeBest);
			return result;
		}

	}
}