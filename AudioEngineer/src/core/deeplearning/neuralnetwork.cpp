#include "neuralnetwork.h"

namespace audioengineer {
	namespace core {

		NeuralNetwork::NeuralNetwork(const std::vector<uint>& topology, float learningRate)
		{
			m_topology = topology;
			m_learningRate = learningRate;
			uint layerCount = topology.size();
			m_layers.reserve(layerCount);
			m_joints.reserve(layerCount - 1);
			uint neuronCountMax = 0;
			for (uint p = 0; p < layerCount; p++) {
				m_layers.emplace_back(topology, p);
				if (p != layerCount - 1) {
					m_joints.emplace_back(topology[p], topology[p + 1]);
				}
				if (topology[p] > neuronCountMax) {
					neuronCountMax = topology[p];
				}
			}
			m_derSigma.resize(neuronCountMax);
		}

		const std::vector<uint>& NeuralNetwork::getTopology() const
		{
			return m_topology;
		}

		float NeuralNetwork::getLearningRate() const
		{
			return m_learningRate;
		}

		void NeuralNetwork::setLearningRate(float learningRate)
		{
			m_learningRate = learningRate;
		}

		void NeuralNetwork::feed(const vec& input)
		{
#ifdef _DEBUG
			if (input.size() != m_layers[0].getNeuronCount()) throw std::invalid_argument("Input dimension mismatch");
#endif
			vec& inputNeurons = m_layers[0].getNeurons();
			for (uint i = 0; i < input.size(); i++) {
				inputNeurons[i] = input[i];
			}
			for (uint k = 0; k < m_joints.size(); k++) {
				vec& inputNeurons = m_layers[k].getNeurons();
				vec& outputNeurons = m_layers[k + 1].getNeurons();
				vec& weights = m_joints[k].getWeights();
				vec& biases = m_joints[k].getBiases();
				uint I = m_joints[k].getOutputCount();
				uint J = m_joints[k].getInputCount();

				for (uint i = 0; i < I; i++) {
					outputNeurons[i] = biases[i];
					uint iJ = i * J;
					for (uint j = 0; j < J; j++) {
						outputNeurons[i] += weights[iJ + j] * inputNeurons[j];
					}
					outputNeurons[i] = sigmoid(outputNeurons[i]);
				}
			}
		}

		vec NeuralNetwork::operator()(const vec& input)
		{
#ifdef _DEBUG
			if (input.size() != m_layers[0].getNeuronCount()) throw std::invalid_argument("Input dimension mismatch");
#endif
			feed(input);
			return m_layers.back().getNeurons();
		}

		float NeuralNetwork::getMSE(const vec& input, const vec& target)
		{
#ifdef _DEBUG
			if (input.size() != m_layers[0].getNeuronCount()) throw std::invalid_argument("Input dimension mismatch");
			if (target.size() != m_layers.back().getNeuronCount()) throw std::invalid_argument("Output dimension mismatch");
#endif
			float result = 0.f;
			feed(input);
			Layer& layer = m_layers.back();
			vec& neurons = layer.getNeurons();
			for (uint q = 0; q < layer.getNeuronCount(); q++) {
				float error = neurons[q] - target[q];
				result += error * error;
			}
			return result / (float)layer.getNeuronCount();
		}

		float NeuralNetwork::getMSE()
		{
			float result = 0.f;
			Layer& layer = m_layers.back();
			vec& neurons = layer.getNeurons();
			for (uint q = 0; q < layer.getNeuronCount(); q++) {
				result += neurons[q] * neurons[q];
			}
			return result / (float)layer.getNeuronCount();
		}

		void NeuralNetwork::computeStep(const vec& input, const vec& target)
		{
#ifdef _DEBUG
			if (input.size() != m_layers[0].getNeuronCount()) throw std::invalid_argument("Input dimension mismatch");
			if (target.size() != m_layers.back().getNeuronCount()) throw std::invalid_argument("Output dimension mismatch");
#endif
			feed(input);
			uint P = m_layers.size();
			for (uint p = 1; p < P; p++) {
				Layer& layer = m_layers[p];
				Layer& pr_layer = m_layers[p - 1];
				Joint& pr_joint = m_joints[p - 1];
				uint Q = layer.getNeuronCount();
				uint H = pr_layer.getNeuronCount();
				vec& neurons = layer.getNeurons();
				vec& pr_neurons = pr_layer.getNeurons();
				vec& weights = pr_joint.getWeights();
				uint I;
				uint J;

				for (uint q = 0; q < Q; q++) {
					m_derSigma[q] = neurons[q] * (1.f - neurons[q]);
				}

				for (uint k = 0; k < p - 1; k++) {
					Joint& joint = m_joints[k];
					I = joint.getOutputCount();
					J = joint.getInputCount();
					vec& derivativesW = layer.getDerivativesW(k);
					vec& derivativesB = layer.getDerivativesB(k);
					vec& pr_derivativesW = pr_layer.getDerivativesW(k);
					vec& pr_derivativesB = pr_layer.getDerivativesB(k);

					for (uint i = 0; i < I; i++) {
						uint iJ = i * J;
						for (uint j = 0; j < J; j++) {
							uint iJ_j = iJ + j;
							uint iJQ_jQ = iJ_j * Q;
							uint iJH_jH = iJ_j * H;
							for (uint q = 0; q < Q; q++) {
								derivativesW[iJQ_jQ + q] = 0.f;
								uint qH = q * H;
								for (uint h = 0; h < H; h++) {
									derivativesW[iJQ_jQ + q] += weights[qH + h] * pr_derivativesW[iJH_jH + h];
								}
								derivativesW[iJQ_jQ + q] *= m_derSigma[q];
							}
						}
						uint iQ = i * Q;
						uint iH = i * H;
						for (uint q = 0; q < Q; q++) {
							derivativesB[iQ + q] = 0.f;
							uint qH = q * H;
							for (uint h = 0; h < H; h++) {
								derivativesB[iQ + q] += weights[qH + h] * pr_derivativesB[iH + h];
							}
							derivativesB[iQ + q] *= m_derSigma[q];
						}
					}
				}

				I = pr_joint.getOutputCount();
				J = pr_joint.getInputCount();
				vec& derivativesW = layer.getDerivativesW(p - 1);
				vec& derivativesB = layer.getDerivativesB(p - 1);

				for (uint i = 0; i < I; i++) {
					uint iJQ = i * J * Q;
					for (uint j = 0; j < J; j++) {
						uint iJQ_jQ = iJQ + j * Q;
						for (uint q = 0; q < Q; q++) {
							derivativesW[iJQ_jQ + q] = i == q ? pr_neurons[j] * m_derSigma[q] : 0.f;
						}
					}
					uint iQ = i * Q;
					for (uint q = 0; q < Q; q++) {
						derivativesB[iQ + q] = i == q ? m_derSigma[q] : 0.f;
					}
				}
			}
			Layer& layer = m_layers.back();
			vec& neurons = layer.getNeurons();
			uint Q = layer.getNeuronCount();
			for (uint q = 0; q < Q; q++) {
				neurons[q] -= target[q];
			}
			for (uint k = 0; k < P - 1; k++) {
				Joint& joint = m_joints[k];
				uint I = joint.getOutputCount();
				uint J = joint.getInputCount();
				vec& weightsStep = joint.getWeightsStep();
				vec& biasesStep = joint.getBiasesStep();
				vec& derivativesW = layer.getDerivativesW(k);
				vec& derivativesB = layer.getDerivativesB(k);

				for (uint i = 0; i < I; i++) {
					uint iJ = i * J;
					for (uint j = 0; j < J; j++) {
						uint iJ_j = iJ + j;
						uint iJQ_jQ = iJ_j * Q;
						for (uint q = 0; q < Q; q++) {
							weightsStep[iJ_j] += neurons[q] * derivativesW[iJQ_jQ + q];
						}
					}
					uint iQ = i * Q;
					for (uint q = 0; q < Q; q++) {
						biasesStep[i] += neurons[q] * derivativesB[iQ + q];
					}
				}
			}
		}

		void NeuralNetwork::takeStep(uint trainingCount)
		{
			float learningFactor = m_learningRate / (float)trainingCount;
			for (unsigned k = 0; k < m_joints.size(); k++) {
				vec& weights = m_joints[k].getWeights();
				vec& biases = m_joints[k].getBiases();
				vec& weightsStep = m_joints[k].getWeightsStep();
				vec& biasesStep = m_joints[k].getBiasesStep();
				uint I = m_joints[k].getOutputCount();
				uint J = m_joints[k].getInputCount();

				for (uint i = 0; i < I; i++) {
					uint iJ = i * J;
					biases[i] -= biasesStep[i] * learningFactor;
					biasesStep[i] = 0.f;
					for (uint j = 0; j < J; j++) {
						weights[iJ + j] -= weightsStep[iJ + j];
						weightsStep[iJ + j] = 0.f;
					}
				}
			}
		}

		NeuralNetwork NeuralNetwork::breed(const Metaparameters& parameters, float fitness)
		{
			NeuralNetwork result = *this;
			float devFactor = 1.f / fitness;
			result.m_learningRate = utils::randFloat(parameters.learningRateMin, parameters.learningRateMax);
			for (unsigned k = 0; k < result.m_joints.size(); k++) {
				Joint& joint = m_joints[k];
				Joint& resultJoint = result.m_joints[k];
				uint I = joint.getOutputCount();
				uint J = joint.getInputCount();
				vec& weights = joint.getWeights();
				vec& biases = joint.getBiases();
				float weightAverage = 0.f;
				float biasAverage = 0.f;
				for (uint i = 0; i < I; i++) {
					uint iJ = i * J;
					for (uint j = 0; j < J; j++) {
						weightAverage += weights[iJ + j];
					}
					biasAverage += biases[i];
				}
				float weightStd = 0.f;
				float biasStd = 0.f;
				for (uint i = 0; i < I; i++) {
					uint iJ = i * J;
					for (uint j = 0; j < J; j++) {
						weightStd += (weights[iJ + j] - weightAverage) * (weights[iJ + j] - weightAverage);
					}
					biasStd += (biases[i] - biasAverage) * (biases[i] - biasAverage);
				}
				weightStd = sqrt(weightStd);
				biasStd = sqrt(biasStd);
				vec& resultWeights = resultJoint.getWeights();
				vec& resultBiases = resultJoint.getBiases();
				float weightSigma = devFactor * weightStd;
				float biasSigma = devFactor * biasStd;
				for (uint i = 0; i < I; i++) {
					uint iJ = i * J;
					for (uint j = 0; j < J; j++) {
						resultWeights[iJ + j] = weights[iJ + j] + utils::randNormal(0.f, weightSigma);
					}
					resultBiases[i] += biases[i] + utils::randNormal(0.f, biasSigma);
				}
			}
			return result;
		}
	}
}