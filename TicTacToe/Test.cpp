#include "Test.hpp"
#include "BPNeuralNetwork.hpp"
#include "Table.hpp"
#include <iostream>
#include <random>
using namespace std;
void Test1()
{
    Table table({-1, 0, 1, -1, 1, 0, 1, -1, 0});
    cout << table;
    cout << "does x win? " << table.DoesXWin() << endl;
}

void Test2()
{
    Sequence sequence(
        vector<State>({State(Table({0, 0, 1, 0, 0, 0, 0, 0, 0})),
                       State(Table({-1, 0, 1, 0, 0, 0, 0, 0, 0})),
                       State(Table({-1, 0, 1, 0, 0, 0, 1, 0, 0})),
                       State(Table({-1, 0, 1, 0, -1, 0, 1, 0, 0})),
                       State(Table({-1, 0, 1, 0, -1, 0, 1, 0, 1})),
                       State(Table({-1, 0, 1, 0, -1, 0, 1, -1, 1})),
                       State(Table({-1, 0, 1, 0, -1, 1, 1, -1, 1}))}),
        0.5);
    sequence.Caculate();
    cout << sequence;
}

void Test3()
{
    bool X_first = true;
    int player = -1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 8);
    BPNeuralNetwork network(9, {9, 9, 9}, 1);
    int count = 0;
    int train_count = 10000;
    while (count < train_count)
    {
        count++;
        if (count % 1000 == 0)
        {
            cout << count << endl;
        }
        Train(X_first, player, dis(gen), network);
    }
    int x_win = 0;
    int draw = 0;
    int total = 10;
    for (int i = 0; i < total; i++)
    {
        int result = AIVsRandom(network);
        if (result == 1)
        {
            x_win++;
        }
        else if (result == 0)
        {
            draw++;
        }
    }

    cout << "AI wins: " << x_win << endl;
    cout << "Random wins: " << total - x_win - draw << endl;
    cout << "Draw: " << draw << endl;
}

int AIVsRandom(BPNeuralNetwork& network)
{
    int player = 1;
    Table table;
    table.RandomSet(-1);

    vector<State> states;
    states.push_back(table);
    while (true)
    {
        //        cout << "---------" << endl;
        //        cout << table;
        //        cout << "---------" << endl;
        if (table.DoesXWin() || table.DoesOWin() || table.IsFull())
        {
            Sequence sequence(states, 0.5);
            sequence.Caculate();
            cout << "  ";
            for (int i = 0; i < 9; i++)
            {
                cout << (i + 1) << " ";
            }
            cout << endl;
            cout << sequence;
            if (table.DoesXWin())
            {
                cout << "AI wins!" << endl;
                return 1;
            }
            else if (table.DoesOWin())
            {
                cout << "random wins" << endl;
                return -1;
            }
            else
            {
                cout << "draw" << endl;
                return 0;
            }
        }
        AIChoose(network, table, player);
        states.push_back(table);

        if (table.DoesXWin() || table.DoesOWin() || table.IsFull())
        {
            Sequence sequence(states, 0.5);
            sequence.Caculate();
            cout << "  ";
            for (int i = 0; i < 9; i++)
            {
                cout << (i + 1) << " ";
            }
            cout << endl;
            cout << sequence;
            if (table.DoesXWin())
            {
                cout << "AI wins!" << endl;
                return 1;
            }
            else if (table.DoesOWin())
            {
                cout << "random wins" << endl;
                return -1;
            }
            else
            {
                cout << "draw" << endl;
                return 0;
            }
        }
        else
        {
            table.RandomSet(-1);
            states.push_back(table);
        }
    }
}

void MemoryTest()
{
    Matrix<double> m = Matrix<double>::RowVector(10);
    cout << m << endl;
}

void AIChoose(BPNeuralNetwork& network, Table& table, int player)
{
    vector<Table> choices = table.Choices(player);
    size_t max_index = 0;
    double max_score = -1000;
    for (size_t i = 0; i < choices.size(); i++)
    {
        choices.at(i).ToVector(network.input);
        network.FeedForward();
        double score = network.output(0);
        if (score > max_score)
        {
            max_score = score;
            max_index = i;
        }
    }
    for (int i = 0; i < 9; i++)
    {
        table.matrix(i) = choices.at(max_index).matrix(i);
    }
}

void TrainOneIteration(BPNeuralNetwork& network, Table& table,
                       vector<State>& states, int& player)
{
    while (true)
    {
        if (table.DoesXWin() || table.DoesOWin() || table.IsFull())
        {
            Sequence sequence(states, 0.5);
            sequence.Caculate();
            //                cout <<
            //                "======================================" <<
            //                endl; cout << sequence;
            for (State& item : sequence.states)
            {
                item.table.ToVector(network.input);
                network.FeedForward();
                network.Backpropagation(Matrix<double>(1, 1, {item.score}));
            }
            return;
        }
        else
        {
            AIChoose(network, table, player);
            states.push_back(table);
            player = player * (-1);
        }
    }
}

void Train(bool& X_first, int& player, int index, BPNeuralNetwork& network)
{
    vector<State> states;
    Table table;
    if (X_first)
    {
        table.SetX(index / 3, index % 3);
        X_first = false;
        player = -1;
    }
    else
    {
        table.SetX(index / 3, index % 3);
        X_first = true;
        player = 1;
    }

    states.push_back(table);
    TrainOneIteration(network, table, states, player);
}

Sequence RandomData(int player)
{
    vector<State> states;
    Table table;
    table.RandomSet(player);
    while (true)
    {
        if (table.DoesXWin() || table.DoesOWin() || table.IsFull())
        {
            Sequence sequence(states, 0.5);
            sequence.Caculate();
            return sequence;
        }
        table.RandomSet(-player);
        states.push_back(table);

        if (table.DoesXWin() || table.DoesOWin() || table.IsFull())
        {
            Sequence sequence(states, 0.5);
            sequence.Caculate();
            return sequence;
        }

        table.RandomSet(player);
        states.push_back(table);
    }
}

void Test4()
{
    int player = -1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 8);
    BPNeuralNetwork network(9, {9, 9, 9}, 1);

    int count = 0;
    int train_count = 1000000;
    while (count < train_count)
    {
        count++;
        if (count % 1000 == 0)
        {
            cout << count << endl;
        }
        Sequence sequence = RandomData(player);
        for (State& item : sequence.states)
        {
            item.table.ToVector(network.input);
            network.FeedForward();
            network.Backpropagation(Matrix<double>(1, 1, {item.score}));
        }
    }

    int x_win = 0;
    int draw = 0;
    int total = 10000;
    for (int i = 0; i < total; i++)
    {
        int result = AIVsRandom(network);
        if (result == 1)
        {
            x_win++;
        }
        else if (result == 0)
        {
            draw++;
        }
    }

    cout << "AI wins: " << x_win << endl;
    cout << "Random wins: " << total - x_win - draw << endl;
    cout << "Draw: " << draw << endl;
}
