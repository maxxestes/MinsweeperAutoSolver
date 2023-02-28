#include<iostream>
#include<cstdlib>
#include <vector>
using namespace std;

class board {
    int currentBoard[16][16] = {0};
    int seed;
    int bombCount;
public:
    board()
    {
        seed = time(0);
        srand(seed);
        bombCount = 40;
        makeBoard();
    }
    /*
    Takes a point and returns surrounding points. Needed because
    arrays do not check bounds on access and will throw.
    Param: point to find all points surrounding.
    return: list of points surrounding given point.
    */
    vector<pair<int, int>> getSurroundingElements(pair<int, int> point) {
        vector<pair<int, int>> surroundingPoints;
        bool downBound = false;
        bool upBound = false;
        bool leftBound = false;
        bool rightBound = false;
        int xPoint = point.first;
        int yPoint = point.second;

        if (xPoint + 1 >= 16) {
            rightBound = true;
        }
        if (xPoint - 1 < 0) {
            leftBound = true;
        }
        if (yPoint - 1 < 0) {
            downBound = true;
        }
        if (yPoint + 1 >= 16) {
            upBound = true;
        }

        if (!rightBound) {
            surroundingPoints.push_back(make_pair(xPoint + 1, yPoint));
            if (!downBound) {
                surroundingPoints.push_back(make_pair(xPoint + 1, yPoint - 1));
            }
            if (!upBound) {
                surroundingPoints.push_back(make_pair(xPoint + 1, yPoint + 1));
            } 
        }
        if (!leftBound) {
            surroundingPoints.push_back(make_pair(xPoint - 1, yPoint));
            if (!downBound) {
                surroundingPoints.push_back(make_pair(xPoint - 1, yPoint - 1));
            }
            if (!upBound) {
                surroundingPoints.push_back(make_pair(xPoint - 1, yPoint + 1));
            } 
        }
        if (!downBound) {
            surroundingPoints.push_back(make_pair(xPoint, yPoint - 1));
        }
        if (!upBound) {
            surroundingPoints.push_back(make_pair(xPoint, yPoint + 1));
        }
        return surroundingPoints;      
    }
    /*
    Uses six areas distributed evenly on the board to insert bombs.
    Gets a random point in the area and puts a bomb in as long as
    it does not have one already. After the bomb is placed, the surrounding
    points are incremented by one.
    */
    void makeBoard() {
        pair<pair<int, int>, pair<int, int>> pivotPoints[9]; //{{min x, max x}, {min y, max y}}
        pivotPoints[0] = {{0,5}, {8,16}}; //top left
        pivotPoints[1] = {{10,16}, {8,16}}; //top right
        pivotPoints[2] = {{0,5}, {0,8}}; //bottom left
        pivotPoints[3] = {{10,16}, {0,8}}; //bottom right
        pivotPoints[4] = {{5,10}, {8,16}}; //top middle
        pivotPoints[5] = {{5,10}, {0,8}}; //bottom middle

        int inputBombCount = bombCount;
        int i = 0;

        
        
        while (inputBombCount > 0) {
            int randomX = rand() % (pivotPoints[i].first.second - pivotPoints[i].first.first);
            int randomY = rand() % (pivotPoints[i].second.second - pivotPoints[i].second.first);

            while (currentBoard[randomX + pivotPoints[i].first.first][randomY + pivotPoints[i].second.first] == -1) {
                randomX = rand() % (pivotPoints[i].first.second - pivotPoints[i].first.first);
                randomY = rand() % (pivotPoints[i].second.second - pivotPoints[i].second.first);
            }

            int xPoint = randomX + pivotPoints[i].first.first;
            int yPoint = randomY + pivotPoints[i].second.first;
            currentBoard[xPoint][yPoint] = -1;

            vector<pair<int, int>> surroundingPoints = getSurroundingElements(make_pair(xPoint, yPoint));
            for (pair<int, int> point : surroundingPoints) {
                if (currentBoard[point.first][point.second] != -1) {
                    currentBoard[point.first][point.second] += 1;
                }
            }
            inputBombCount--;

            if (i < 5) {
                i++;
            }
            else {
                i = 0;
            }
        } 
    }

    auto getBoard() {
        return currentBoard;
    }

    pair<int, int> getStart() {
        for (int i = 7; i >= 0; i--) {
            for (int j = 7; j < 16; j++) {
                if (currentBoard[j][i] == 0) {
                    return make_pair(j, i);
                }
            }
        }
        return make_pair(0, 0);
    } 

    auto getNewInfo(pair<int, int> chosenTile) {
        vector<pair<pair<int, int>, int>> returnCoords;
        if (currentBoard[chosenTile.first][chosenTile.second] == -1) {
            vector<pair<pair<int, int>, int>> returnCoords;
            return returnCoords;
        }
        else if (currentBoard[chosenTile.first][chosenTile.second] == 0) {
            vector<pair<int, int>> zerosToCheck;
            vector<pair<int, int>> checkedZeros;
            zerosToCheck.push_back(chosenTile);
            pair newPair = make_pair(chosenTile, 0);
            returnCoords.push_back(newPair);
            while (zerosToCheck.size() > 0) {
                int xPoint = zerosToCheck.back().first;
                int yPoint = zerosToCheck.back().second;
                vector<pair<pair<int, int>, int>>::iterator it;
                vector<pair<int, int>>::iterator it2;
                vector<pair<int, int>>::iterator it3;

                vector<pair<int, int>> surroundingPoints = getSurroundingElements(make_pair(xPoint, yPoint));
                
                for (pair<int, int> point : surroundingPoints) {
                    for (it = returnCoords.begin() ; it != returnCoords.end(); ++it) {
                        if (it->first.first == point.first && it->first.second == point.second) {
                            break;
                        }
                    }
                    if (it == returnCoords.end()) {
                        newPair = make_pair(make_pair(point.first, point.second), currentBoard[point.first][point.second]);
                        returnCoords.push_back(newPair);
                    }
                    if (currentBoard[point.first][point.second] == 0) {
                        for (it2 = checkedZeros.begin() ; it2 != checkedZeros.end(); ++it2) {
                            if (it2->first == point.first && it2->second == point.second) {
                                break;
                            }
                        }
                        for (it3 = zerosToCheck.begin() ; it3 != zerosToCheck.end(); ++it3) {
                            if (it3->first == point.first && it3->second == point.second) {
                                break;
                            }
                        }
                        if (it2 == checkedZeros.end() && it3 == zerosToCheck.end()) {
                            zerosToCheck.insert(zerosToCheck.begin(), make_pair(point.first, point.second));
                        }
                    }
                    
                }
                checkedZeros.push_back(zerosToCheck.back());
                zerosToCheck.pop_back();
            }
            return returnCoords;
        }
        else {
            pair newPair = make_pair(chosenTile, currentBoard[chosenTile.first][chosenTile.second]);
            vector<pair<pair<int, int>, int>> returnCoords;
            returnCoords.push_back(newPair);
            return returnCoords;
        }
    }
};

class solver {
    board boardToSolve;
    pair<int,int> startingPoint;
    int knowledgeBoard[16][16];
    vector<pair<pair<int, int>, int>> knownCoordPairArr;
    vector<pair<int, int>> coordsToClick;
    int bombsLeft;
public:
    solver(pair<int,int> startingPoint, board board) {
        bombsLeft = 40;
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                knowledgeBoard[i][j] = -2;
            }
        }
        boardToSolve = board;
        this->startingPoint = startingPoint;
        addBoardInfo(boardToSolve.getNewInfo(startingPoint));
        markBombs();
    }

    auto getBoard() {
        return knowledgeBoard;
    }

    void solveBoard() {
        while (coordsToClick.size() > 0) {
            leftClick();
            markBombs();
        }
        vector<pair<pair<int, int>, int>> lastDitchArr = knownCoordPairArr;
        lastDitch(lastDitchArr);
        markBombs();
        if (!coordsToClick.empty()) {
            solveBoard();
        }
    }

    void lastDitch(vector<pair<pair<int, int>, int>> lastDitchArr) {
        vector<pair<pair<int, int>, pair<int, int>>> newPointsToTry;
        while (lastDitchArr.size() > 0) {
            vector<pair<int, int>> surroundingPoints = boardToSolve.getSurroundingElements(lastDitchArr.back().first);
            for (pair<int, int> point : surroundingPoints) {
                if (knowledgeBoard[point.first][point.second] > 0) {
                    newPointsToTry.push_back(make_pair(lastDitchArr.back().first, make_pair(point.first, point.second)));
                }
            }
            lastDitchArr.pop_back();
        }
        while (newPointsToTry.size() > 0) {
            vector<pair<int, int>> surroundHigh;
            vector<pair<int, int>> surroundLow;
            int valueAtPointHigh = knowledgeBoard[newPointsToTry.back().first.first][newPointsToTry.back().first.second];
            int valueAtPointLow = knowledgeBoard[newPointsToTry.back().second.first][newPointsToTry.back().second.second];
            int highBombs = 0;
            int lowBombs = 0;


            vector<pair<int, int>> surroundingPointsHigh = boardToSolve.getSurroundingElements(newPointsToTry.back().first);
            vector<pair<int, int>> surroundingPointsLow = boardToSolve.getSurroundingElements(newPointsToTry.back().second);
            for (pair<int, int> point : surroundingPointsHigh) {
                if (knowledgeBoard[point.first][point.second] == -2) {
                    surroundHigh.push_back(make_pair(point.first, point.second));
                }
                else if (knowledgeBoard[point.first][point.second] == -1) {
                    highBombs++;
                }
            }

            for (pair<int, int> point : surroundingPointsLow) {
                if (knowledgeBoard[point.first][point.second] == -2) {
                    surroundLow.push_back(make_pair(point.first, point.second));
                }
                else if (knowledgeBoard[point.first][point.second] == -1) {
                    lowBombs++;
                }
            }

            if ((valueAtPointHigh - highBombs == 2 && valueAtPointLow - lowBombs == 1)
                || valueAtPointHigh - highBombs == 1 && valueAtPointLow - lowBombs == 2) {
                bool erasedI = false;
                for (auto i = surroundHigh.begin(); i != surroundHigh.end();) {
                    erasedI = false;
                    for (auto j = surroundLow.begin(); j != surroundLow.end();) {
                        if (i->first == j->first && i->second == j->second) {
                            surroundLow.erase(j);
                            surroundHigh.erase(i);
                            erasedI = true;
                            break;
                        }
                        else {
                            ++j;
                        }
                    }
                    if (i == surroundHigh.end()) {
                        break;
                    }
                    else if (!erasedI){
                        ++i;
                    }
                }
                if (surroundHigh.size() == 1 && surroundLow.empty()) {
                    knowledgeBoard[surroundHigh[0].first][surroundHigh[0].second] = -1;
                }
                else if (surroundLow.size() == 1 && surroundHigh.empty()) {
                    knowledgeBoard[surroundLow[0].first][surroundLow[0].second] = -1;
                }
                else if (surroundLow.size() == 1 && surroundHigh.size() == 1) {
                    if (valueAtPointHigh - highBombs == 2) {
                        knowledgeBoard[surroundHigh[0].first][surroundHigh[0].second] = -1;
                        coordsToClick.push_back(surroundLow[0]);
                    }
                    else if (valueAtPointLow - lowBombs == 2) {
                        knowledgeBoard[surroundLow[0].first][surroundLow[0].second] = -1;
                        coordsToClick.push_back(surroundHigh[0]);
                    }
                }
            }
            else if (valueAtPointHigh - highBombs == 1 && valueAtPointLow - lowBombs == 1) {
                bool erasedI = false;
                for (auto i = surroundHigh.begin(); i != surroundHigh.end();) {
                    erasedI = false;
                    for (auto j = surroundLow.begin(); j != surroundLow.end();) {
                        if (i->first == j->first && i->second == j->second) {
                            surroundLow.erase(j);
                            surroundHigh.erase(i);
                            erasedI = true;
                            break;
                        }
                        else {
                            ++j;
                        }
                    }
                    if (i == surroundHigh.end()) {
                        break;
                    }
                    else if (!erasedI) {
                        ++i;
                    }
                }

                if (surroundHigh.size() >= 1 && surroundLow.empty()) {
                    for (auto newCoords : surroundHigh) {
                        coordsToClick.push_back(newCoords);
                    }
                }
                else if (surroundLow.size() >= 1 && surroundHigh.empty()) {
                    for (auto newCoords : surroundLow) {
                        coordsToClick.push_back(newCoords);
                    }
                }
            }
            newPointsToTry.pop_back();
        }
    }

    bool addBoardInfo(vector<pair<pair<int, int>, int>> coordsToAdd) {
        if (coordsToAdd.size() == 0) {
            return false;
        }
        while (coordsToAdd.size() > 0) {
            knowledgeBoard[coordsToAdd.back().first.first][coordsToAdd.back().first.second] = coordsToAdd.back().second;
            if (coordsToAdd.back().second != 0) {
                knownCoordPairArr.push_back(coordsToAdd.back());
            }
            coordsToAdd.pop_back();
        }
        return true;
    }

    void leftClick() {
       vector<pair<pair<int, int>, int>> newInfo = boardToSolve.getNewInfo(coordsToClick.back());
       coordsToClick.pop_back();
       addBoardInfo(newInfo);
    }

    void markBombs() {
        for (auto iter = knownCoordPairArr.begin(); iter != knownCoordPairArr.end();) {
            vector<pair<int, int>> unknownSpaceCoord;
            vector<pair<int, int>> posSpacesWithoutBombs;
            int numUnknownSurroundings = 0;
            int xPoint = iter->first.first;
            int yPoint = iter->first.second;

            vector<pair<int, int>> surroundingPoints = boardToSolve.getSurroundingElements(iter->first);
            for (pair<int, int> point : surroundingPoints) {
                if (knowledgeBoard[point.first][point.second] == -2 || knowledgeBoard[point.first][point.second] == -1) {
                    unknownSpaceCoord.push_back(make_pair(point.first, point.second));
                    numUnknownSurroundings += 1;
                    if (knowledgeBoard[point.first][point.second] == -2) {
                        posSpacesWithoutBombs.push_back(make_pair(point.first, point.second));
                    }
                }
            }

            if (numUnknownSurroundings == iter->second) {
                for (int i = 0; i < unknownSpaceCoord.size(); i++) {
                    if (knowledgeBoard[unknownSpaceCoord[i].first][unknownSpaceCoord[i].second] == -2) {
                        knowledgeBoard[unknownSpaceCoord[i].first][unknownSpaceCoord[i].second] = -1;
                        bombsLeft--;
                    }
                }
                knownCoordPairArr.erase(iter);
            }
            else if (numUnknownSurroundings - posSpacesWithoutBombs.size() == iter->second) {
                for (int i = 0; i < posSpacesWithoutBombs.size(); i++) {
                    coordsToClick.push_back(posSpacesWithoutBombs[i]);
                }
                knownCoordPairArr.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }
};


int main() {
    board boardClass = board();
    auto currentBoard = boardClass.getBoard();
    for (int i = 15; i >= 0; i--) {
        for (int j = 0; j < 16; j++) {
            printf("%*i", 3,  currentBoard[j][i]);
        }
        cout<<"\n";
    }
    solver solverClass = solver(boardClass.getStart(), boardClass);
    cout<<"\n";
    solverClass.solveBoard();
    auto solvedBoard = solverClass.getBoard();
    for (int i = 15; i >= 0; i--) {
        for (int j = 0; j < 16; j++) {
            //cout<<currentBoard[i][j]<<" ";
            printf("%*i", 3,  solvedBoard[j][i]);
        }
        cout<<"\n";
    }
}