#include "Table.hpp"
#include <random>
Table::Table()
    : matrix(3, 3)
{
    for (int i = 0; i < matrix.Rows(); i++)
    {
        for (int j = 0; j < matrix.Cols(); j++)
        {
            matrix(i, j) = 0;
        }
    }
}

Table::Table(const Table& other)
    : matrix{other.matrix}
{
}

Table::Table(Table&& other)
    : matrix{other.matrix}
{
}

Table::Table(std::initializer_list<int> args)
    : matrix(3, 3, args)
{
}

void Table::SetX(int row, int col)
{
    if (matrix(row, col) == 0)
    {
        matrix(row, col) = 1;
    }
    else
    {
        throw InvalidMoveException();
    }
}

void Table::SetO(int row, int col)
{
    if (matrix(row, col) == 0)
    {
        matrix(row, col) = -1;
    }
    else
    {
        throw InvalidMoveException();
    }
}

void Table::Reset()
{
    for (int i = 0; i < matrix.Rows(); i++)
    {
        for (int j = 0; j < matrix.Cols(); j++)
        {
            matrix(i, j) = 0;
        }
    }
}

bool Table::DoesXWin()
{
    return CheckWinner(3);
}

bool Table::DoesOWin()
{
    return CheckWinner(-3);
}

bool Table::IsFull()
{
    for (int i = 0; i < matrix.Rows(); i++)
    {
        for (int j = 0; j < matrix.Cols(); j++)
        {
            if (matrix(i, j) == 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool Table::HasX()
{
    for (int i = 0; i < matrix.Rows(); i++)
    {
        for (int j = 0; j < matrix.Cols(); j++)
        {
            if (matrix(i, j) == 1)
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<Table> Table::Choices(int value)
{
    std::vector<Table> result;
    for (int i = 0; i < matrix.Rows(); i++)
    {
        for (int j = 0; j < matrix.Cols(); j++)
        {
            if (matrix(i, j) == 0)
            {
                Table table(*this);
                table.matrix(i, j) = value;
                result.push_back(table);
            }
        }
    }
    return result;
}

void Table::RandomSet(int value)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> choices;
    int length = matrix.Rows() * matrix.Cols();
    for (int i = 0; i < length; i++)
    {
        if (matrix(i) == 0)
        {
            choices.push_back(i);
        }
    }
    std::uniform_int_distribution<> dis(0,
                                        static_cast<int>(choices.size() - 1));
    size_t index = static_cast<size_t>(dis(gen));
    matrix(choices.at(index)) = value;
}

void Table::ToVector(Matrix<double>& input)
{
    for (int i = 0; i < 9; i++)
    {
        input(i) = matrix(i);
    }
}

std::ostream& operator<<(std::ostream& stream, Table& table)
{
    for (int i = 0; i < table.matrix.Rows(); i++)
    {
        for (int j = 0; j < table.matrix.Cols(); j++)
        {
            if (table.matrix(i, j) == 1)
            {
                stream << "X";
            }
            else if (table.matrix(i, j) == -1)
            {
                stream << "O";
            }
            else
            {
                stream << " ";
            }
            stream << " ";
        }
        stream << std::endl;
    }
    return stream;
}

bool Table::CheckWinner(int value)
{
    for (int i = 0; i < matrix.Rows(); i++)
    {
        if (matrix.SumOfRow(i) == value)
        {
            return true;
        }
    }

    for (int j = 0; j < matrix.Cols(); j++)
    {
        if (matrix.SumOfColumn(j) == value)
        {
            return true;
        }
    }
    int sum = 0;
    for (int i = 0; i < matrix.Rows(); i++)
    {
        sum = sum + matrix(i, i);
    }
    if (sum == value)
    {
        return true;
    }
    else
    {
        sum = 0;
        for (int i = 0; i < matrix.Rows(); i++)
        {
            sum = sum + matrix(i, matrix.Cols() - i - 1);
        }
        if (sum == value)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

InvalidMoveException::InvalidMoveException()
{
}

State::State(Table table)
    : table{table}
    , score{0.0}
{
}

State::State(Table table, double score)
    : table{table}
    , score{score}
{
}

Sequence::Sequence(std::vector<State> states, double learningRate)
    : states{states}
    , learningRate{learningRate}
{
}

void Sequence::Caculate()
{
    if (states.at(states.size() - 1).table.DoesXWin())
    {
        Calculate(1.0, 0.0);
    }
    else if (states.at(states.size() - 1).table.DoesOWin())
    {
        Calculate(1.0, 0.0);
    }
    else
    {
        Calculate(0.4, 0.6);
    }
}

void Sequence::Calculate(double score1, double score2)
{
    double score = score1;
    states.at(states.size() - 1).score = score;
    for (int i = static_cast<int>(states.size() - 1 - 2); i >= 0; i -= 2)
    {
        score = 0.5 + learningRate * (score - 0.5);
        states.at(static_cast<size_t>(i)).score = score;
    }
    score = score2;
    states.at(states.size() - 1 - 1).score = score;

    for (int i = static_cast<int>(states.size() - 1 - 1 - 2); i >= 0; i -= 2)
    {
        score = 0.5 + learningRate * (score - 0.5);
        states.at(static_cast<size_t>(i)).score = score;
    }
}

bool Sequence::DoesXGoFirst()
{
    return states.at(0).table.HasX();
}

std::ostream& operator<<(std::ostream& stream, Sequence& sequence)
{
    for (size_t i = 0; i < sequence.states.size(); i++)
    {
        Matrix<int>& matrix = sequence.states.at(i).table.matrix;
        int length = matrix.Rows() * matrix.Cols();
        stream << "( ";
        for (int j = 0; j < length; j++)
        {
            if (matrix(j) == 1)
            {
                stream << 'X';
            }
            else if (matrix(j) == -1)
            {
                stream << 'O';
            }
            else
            {
                stream << ' ';
            }
            stream << ' ';
        }
        stream << ") -> ";
        stream << sequence.states.at(i).score << std::endl;
    }
    return stream;
}
