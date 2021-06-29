/*
Note this is brute force solution because I did not come up with a better solution.
Maybe there is some relation I am missing idk
*/


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


class Din
{
private:
    unsigned int lowestVal;
    unsigned int middleVal;
    unsigned int highestVal;
    char lowestChar;
    char middleChar;
    char highestChar;

public:
    Din(unsigned int lowestVal, unsigned int middleVal, unsigned int highestVal)
        : lowestVal(lowestVal), middleVal(middleVal), highestVal(highestVal)
    {
        lowestChar = 'A';
        middleChar = 'B';
        highestChar = 'C';
    }

    void sort()
    {
        if (lowestVal > highestVal) {
            unsigned int temp = lowestVal;
            lowestVal = highestVal;
            highestVal = temp;

            char tempChar = lowestChar;
            lowestChar = highestChar;
            highestChar = tempChar;
        }

        if (middleVal > highestVal) {
            unsigned int temp = middleVal;
            middleVal = highestVal;
            highestVal = temp;

            char tempChar = middleChar;
            middleChar = highestChar;
            highestChar = tempChar;
        }

        if (lowestVal > middleVal) {
            unsigned int temp = lowestVal;
            lowestVal = middleVal;
            middleVal = temp;

            char tempChar = lowestChar;
            lowestChar = middleChar;
            middleChar = tempChar;
        }
    }

    bool operator==(const Din& other) const
    {
        return (this->highestVal == other.highestVal &&
                this->middleVal == other.middleVal &&
                this->lowestVal == other.lowestVal);
    }

    bool gameOver()
    {
        return !lowestVal || !middleVal || !highestVal;
    }

    bool isLegalMove(unsigned from, unsigned to)
    {
        if (from > 2 || to > 2) {
            return false;
        }

        unsigned int allValues[3] = {lowestVal, middleVal, highestVal};
        if (allValues[from] < allValues[to]) {
            return false;
        }

        return true;
    }

    void makeMove(unsigned from, unsigned to)
    {
        if (!isLegalMove(from, to)) {
            return;
        }

        unsigned int allValues[3] = {lowestVal, middleVal, highestVal};

        allValues[from] -= allValues[to];
        allValues[to] <<= 1;

        lowestVal = allValues[0];
        middleVal = allValues[1];
        highestVal = allValues[2];

        sort();
    }

    void reverseMove(char from, char to)
    {
        if (to == highestChar) {
            highestVal >>= 1;
            if (from == middleChar) {
                middleVal += highestVal;
            }
            else {
                lowestVal += highestVal;
            }
        }
        else if (to == middleChar) {
            middleVal >>= 1;
            if (from == lowestChar) {
                lowestVal += middleVal;
            }
            else {
                highestVal += middleVal;
            }
        }
        else {
            lowestVal >>= 1;
            if (from == middleChar) {
                middleVal += lowestVal;
            }
            else {
                highestVal += lowestVal;
            }
        }

        sort();
    }

    unsigned int getLowestVal() const
    {
        return lowestVal;
    }

    unsigned int getMiddleVal() const
    {
        return middleVal;
    }

    unsigned int getHighestVal() const
    {
        return highestVal;
    }

    char getChar(unsigned int index) const
    {
        if (index == 0) {
            return lowestChar;
        }
        else {
            return (index == 1 ? middleChar : highestChar);
        }
    }

};

//Hashing function for a Din
namespace std
{
    template <>
    struct hash<Din>
    {
        std::size_t operator()(const Din& d) const
        {
        using std::size_t;
        using std::hash;
        using std::string;

        return ((hash<unsigned int>()(d.getLowestVal())
                ^ (hash<unsigned int>()(d.getMiddleVal()) << 1)) >> 1)
                ^ (hash<unsigned int>()(d.getHighestVal()) << 1);
        }
    };
}


const unsigned numberOfMoves = 3;
const unsigned fromMoves[numberOfMoves] = {2, 2, 1};
const unsigned toMoves[numberOfMoves] = {1, 0, 0};

/*
Simple backtracking function using map in order to remember where it has been.
Without map it is bottomles recursion.
*/
bool solve(std::unordered_map<Din, unsigned int>& umap, Din din, std::vector<std::string>& result)
{
    if (din.gameOver()) {
        return true;
    }

    std::unordered_map<Din, unsigned int>::const_iterator gotMyDin = umap.find(din);
    if (gotMyDin == umap.end()) {
        std::pair<Din, unsigned int> firstDin (din, 0);
        umap.insert(firstDin);
    }

    gotMyDin = umap.find(din);
    for (unsigned int i = gotMyDin->second; i < numberOfMoves; ++i) {
        unsigned int newValue = gotMyDin->second + 1;
        umap.emplace(din, newValue).first->second = newValue;

        if (din.isLegalMove(fromMoves[i], toMoves[i])) {
            char from = din.getChar(fromMoves[i]);
            char to = din.getChar(toMoves[i]);

            din.makeMove(fromMoves[i], toMoves[i]);

            if (solve(umap, din, result)) {
                std::string move;
                move.push_back(from);
                move += "->";
                move.push_back(to);
                move.push_back('\n');
                result.push_back(move);

                return true;
            }
            else {
                din.reverseMove(from, to);
            }
        }
    }

    return false;
}

/*
Note that I need hash function and operator== in order to work correctly with unordered_map and Din class

Note the program thinks the input is correct:
    0 < A < B < C < 100000
It will not work properly otherwise

Still has bugs I cannot figure out but works most of the time :D
*/
int main()
{
    unsigned a, b, c;
    std::cin >> a;
    std::cin >> b;
    std::cin >> c;
    Din startingDin(a, b, c);


    std::unordered_map<Din, unsigned int> umap;
    std::pair<Din, unsigned int> firstDin (startingDin, 0);
    umap.insert(firstDin);

    std::vector<std::string> result;
    if (!solve(umap, startingDin, result)) {
        std::cout << "NOT POSSIBLE";
    }
    else {
        for (int i = result.size() - 1; i >= 0; --i) {
            std::cout << result[i];
        }
    }


    return 0;
}
