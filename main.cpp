// Compiler: GCC (g++) 15.2.0, MSYS2
// C++ standard: C++23

#include "weighted_random.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;


size_t parseGenerationCount(const string &line)
{
    istringstream input(line);

    long long value;

    if (!(input >> value))
        throw runtime_error("The first line must contain a natural number N.");

    if (value <= 0)
        throw runtime_error("N must be a natural number.");

    string extra;

    if (input >> extra)
        throw runtime_error("The first line must contain only one number.");

    if (static_cast<unsigned long long>(value) > numeric_limits<size_t>::max())
    {
        throw runtime_error("N is too large.");
    }

    return static_cast<size_t>(value);
}


vector<int> parseValues(const string &line)
{
    istringstream input(line);

    vector<int> values;
    int value;

    while (input >> value)
        values.push_back(value);

    if (!input.eof())
        throw runtime_error("The second line must contain integer values only.");

    if (values.empty())
        throw runtime_error("The sequence of input values cannot be empty.");

    return values;
}


vector<size_t> parseFrequencies(const string &line)
{
    istringstream input(line);

    vector<size_t> frequencies;
    long long frequency;

    while (input >> frequency)
    {
        if (frequency < 0)
            throw runtime_error("Frequencies cannot be negative.");

        frequencies.push_back(static_cast<size_t>(frequency));
    }

    if (!input.eof())
        throw runtime_error("The third line must contain integer frequencies only.");

    if (frequencies.empty())
        throw runtime_error("The sequence of frequencies cannot be empty.");

    return frequencies;
}


bool isBlank(const string &line)
{
    return all_of(
        line.begin(),
        line.end(),
        [](unsigned char ch)
        {
            return isspace(ch);
        }
    );
}


vector<WeightedValue> readInput(const string &fileName,size_t &generationCount)
{
    ifstream input(fileName);

    if (!input)
        throw runtime_error("Cannot open input file: " + fileName);

    string countLine;
    string valuesLine;
    string frequenciesLine;

    if (!getline(input, countLine))
        throw runtime_error("The input file does not contain N.");

    if (!getline(input, valuesLine))
        throw runtime_error("The input file does not contain the sequence of values.");

    if (!getline(input, frequenciesLine))
        throw runtime_error("The input file does not contain the sequence of frequencies.");

    string extraLine;

    while (getline(input, extraLine))
    {
        if (!isBlank(extraLine))
            throw runtime_error("The input file contains extra data.");
    }

    generationCount = parseGenerationCount(countLine);

    vector<int> values = parseValues(valuesLine);

    vector<size_t> frequencies = parseFrequencies(frequenciesLine);

    if (values.size() != frequencies.size())
    {
        throw runtime_error("The number of values and frequencies must be equal.");
    }

    vector<WeightedValue> result;

    result.reserve(values.size());

    for (size_t i = 0; i < values.size(); ++i)
    {
        result.push_back({values[i], frequencies[i]});
    }

    return result;
}


void printResult(const vector<WeightedValue> &source, size_t generationCount)
{
    WeightedRandom randomValue(source);

    unordered_map<int, size_t> obtained;

    obtained.reserve(source.size());

    for (size_t i = 0; i < generationCount; ++i)
    {
        ++obtained[randomValue()];
    }

    long double totalFrequency = accumulate(source.begin(), source.end(), 0.0L, []
       (long double result, const WeightedValue &item)
            {
                return result + item.frequency;
            }
        );

    cout << format("Number of generated values: {}\n\n",generationCount);

    cout << format("{:<12}{:<16}{:<18}{:<18}{:<18}{:<18}\n",
                "Value",
                "Given freq.",
                "Expected, %",
                "Obtained count",
                "Obtained, %",
                "Difference, %");

    cout << string(100, '-') << endl;

    double maxDifference = 0.0;
    int maxDifferenceValue = source.front().value;

    for (const auto &item : source)
    {
        size_t count = obtained[item.value];

        double expected = static_cast<double>(static_cast<long double>(item.frequency) /totalFrequency);

        double actual = static_cast<double>(count) / static_cast<double>(generationCount);

        double difference = abs(expected - actual);

        if (less<double>()(maxDifference, difference))
        {
            maxDifference = difference;
            maxDifferenceValue = item.value;
        }

        cout << format( "{:<12}{:<16}{:<18.6f}{:<18}{:<18.6f}{:<18.6f}\n",
                    item.value, item.frequency, expected * 100.0, count,
                    actual * 100.0, difference * 100.0);
    }

    cout << endl;

    cout << format("Maximum frequency difference: {:.6f}% " 
        "(for value {})\n",
                maxDifference * 100.0,
                maxDifferenceValue);
}


int main(int argc, char *argv[]) {
     try {
        if (argc > 2)
          throw runtime_error("Usage: lab3 [input_file]");
        string fileName = "input.txt";

        if (argc == 2)
            fileName = argv[1];

        size_t generationCount = 0;

        vector<WeightedValue> source = readInput(fileName, generationCount);

        printResult(source, generationCount);
    }
    catch (const exception &e)
    {
        cerr << "Error: "
             << e.what()
             << endl;

        return 1;
    }

    return 0;
}
