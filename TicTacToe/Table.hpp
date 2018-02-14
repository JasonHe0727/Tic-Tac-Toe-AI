#ifndef TABLE_HPP
#define TABLE_HPP
#include "Matrix.hpp"
#include <initializer_list>
#include <iostream>
#include <vector>
class InvalidMoveException
{
public:
    InvalidMoveException();
};
class Table
{
public:
    Table();
    Table(const Table& other);
    Table& operator=(const Table& other) = default;
    Table(Table&& other);
    Table(std::initializer_list<int> args);
    void SetX(int row, int col);
    void SetO(int row, int col);
    void Reset();
    bool DoesXWin();
    bool DoesOWin();
    bool IsFull();
    bool HasX();

    std::vector<Table> Choices(int value);
    void RandomSet(int value);
    void ToVector(Matrix<double>& input);
    friend std::ostream& operator<<(std::ostream& stream, Table& table);

    Matrix<int> matrix;

private:
    bool CheckWinner(int value);
};

class State
{
public:
    Table table;
    double score;
    State(Table table);
    State(Table table, double score);
};

class Sequence
{
public:
    std::vector<State> states;
    double learningRate = 0.5;
    Sequence(std::vector<State> states, double learningRate);
    void Caculate();
    void Calculate(double score1, double score2);
    bool DoesXGoFirst();
    friend std::ostream& operator<<(std::ostream& stream, Sequence& sequence);
};
#endif // TABLE_HPP
