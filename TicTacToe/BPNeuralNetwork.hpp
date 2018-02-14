#ifndef BPNEURALNETWORK_HPP
#define BPNEURALNETWORK_HPP
#include "Matrix.hpp"
#include <vector>
using std::vector;

class BPNeuralNetwork
{
public:
    Matrix<double> input;
    vector<Matrix<double>> hidden;
    Matrix<double> output;

    vector<Matrix<double>> weights;
    vector<Matrix<double>> biases;
    vector<Matrix<double>> deltas;

    double learningRate = 0.5;
    BPNeuralNetwork(int numOfInput, vector<int> numOfHidden, int numOfOutput);
    void FeedForward();
    void Backpropagation(const Matrix<double>& targets);

private:
    void BackpropagationToOutputLayer(const Matrix<double>& targets);
    void BackpropagationToHiddenLayers();
    void UpdateWeights();

    void RandomizeMatrix(Matrix<double>& matrix);
};

template <typename T>
T& First(vector<T>& vec)
{
    return vec.at(0);
}
template <typename T>
T& Last(vector<T>& vec)
{
    return vec.at(vec.size() - 1);
}

Matrix<double> Activte(const Matrix<double>& matrix);
void Activte(Matrix<double>& result, const Matrix<double>& matrix);
Matrix<double> DerivativeActive(const Matrix<double>& matrix);
#endif // BPNEURALNETWORK_HPP
