#ifndef TEST_HPP
#define TEST_HPP
#include "BPNeuralNetwork.hpp"
#include "Table.hpp"
void Test1();
void Test2();
void Test3();
void AIChoose(BPNeuralNetwork& network, Table& table, int player);
int AIVsRandom(BPNeuralNetwork& network);
void Train(bool& X_first, int& player, int index, BPNeuralNetwork& network);
void TrainOneIteration(BPNeuralNetwork& network, Table& table,
                       vector<State>& states, int& player);
void MemoryTest();
Sequence RandomData(int player);
void Test4();

#endif // TEST_HPP
