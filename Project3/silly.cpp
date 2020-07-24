// PROJECT IDENTIFIER: C0F4DFE8B340D81183C208F70F9D2D797908754D
//  silly.cpp
//  Project3
//
//  Created by Akash on 3/12/20.
//  Copyright © 2020 Akash Bajpai. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include "xcode_redirect.hpp"
#include <iostream>
#include <getopt.h>
#include "TableEntry.h"
#include <unordered_map>
#include <map>
#include <algorithm>

using namespace std;


struct Table {
    string tableName;
    vector<string>colNames;
    unordered_map<string, int> orderOfCols;
    vector<string> colTypes;
    vector<vector<TableEntry>> data;
    int numCols;
    bool indexExists;
    unordered_map<TableEntry, vector<int>> hashIndex;
    map<TableEntry, vector<int>> bstIndex;
    string indexType;
    string indexCol;
    Table(string tableN, int cols) : tableName(tableN), numCols(cols) {
        colNames.resize(cols);
        colTypes.resize(cols);
        indexExists = false;
    }
    Table() = default;
    
    class CompEqual {
    private:
        TableEntry &temp;
        int colIndex;
    public:
        CompEqual(TableEntry &t, int col) : temp(t), colIndex(col) {}
        
        bool operator() (const vector<TableEntry> &row) {
            return row[colIndex] == temp;
        }
    };
    
    class CompLess {
    private:
        TableEntry &temp;
        int colIndex;
    public:
        CompLess(TableEntry &t, int col) : temp(t), colIndex(col) {}
        
        bool operator() (const vector<TableEntry> &row) {
            return row[colIndex] < temp;
        }
    };
    
    class CompMore {
    private:
        TableEntry &temp;
        int colIndex;
    public:
        CompMore(TableEntry &t, int col) : temp(t), colIndex(col) {}
        
        bool operator() (const vector<TableEntry> &row) {
            return row[colIndex] > temp;
        }
    };
};

bool CompareEqual(size_t index, TableEntry &temp, vector<TableEntry> &tempRow) {
    return tempRow[index] == temp;
}
bool CompareLess(size_t index, TableEntry &temp, vector<TableEntry> &tempRow) {
    return tempRow[index] < temp;
}
bool CompareMore(size_t index, TableEntry &temp, vector<TableEntry> &tempRow) {
    return tempRow[index] > temp;
}


void getHelp() {
    cerr << "Nigthmare Nightmare Nightmare!!!\n" << flush;
    exit(1);
} //end of help

int readCommands(int argc, char *argv[]) {
    int choice = 0;
    int option_index = 0;
    int commands = 0;
    string outSpec;
    
    option long_options[] = {
        {"quiet",              no_argument, nullptr,  'q'},
        {"help",               no_argument, nullptr,  'h'},
        {nullptr,                        0, nullptr, '\0'}
    }; //end of options

    while((choice = getopt_long(argc, argv, "hq", long_options, &option_index)) != -1) {
        switch(choice) {
            case 'h':
                getHelp();
                exit(1);
            case 'q':
                commands = 1;
                break;
            default:
                cerr << "Error: invalid option" << flush;
                exit(1);
        } //end of switch
    } //end of while
    return commands;
} //end of readCommands

bool isNewTable(unordered_map<string, Table> &database, const string &newName) {
    auto it = database.find(newName);
    if(it == database.end()) {
        return true;
    }
    return false;
}


void create(unordered_map<string, Table> &database) {
    string tableName;
    int numCols;
    cin >> tableName >> numCols;
    Table newTable(tableName, numCols);
    
    if(isNewTable(database, tableName)) {
        database.insert({tableName, newTable});
        string col;
        for(int i = 0; i < numCols; ++i) {
            cin >> col;
            database[tableName].colTypes[i] = col;
        }
        for(int j = 0; j < numCols; ++j) {
            cin >> col;
            database[tableName].colNames[j] = col;
            database[tableName].orderOfCols.insert({database[tableName].colNames[j], j});
        }
    
        cout << "New table " << tableName << " with column(s) ";
        for(auto &x : database[tableName].colNames) {
            cout << x  << " ";
        }
        cout << "created\n";
    }
    else {
        string junk = "";
        getline(cin, junk);
        cout << "Error: Cannot create already existing table " << tableName << "\n";
    }
}//end of create

void remove(unordered_map<string, Table> &database, const string &name) {
    auto it = database.find(name);
    if(it != database.end()) {
        database.erase(name);
        cout << "Table " << name << " deleted\n";
    }
    else {
        string junk = "";
        getline(cin, junk);
        cout << "Error: " << name << " does not name a table in the database\n";
    }
} //end of remove

int findColIndex(unordered_map<string, Table> &database, const string &name, const string &colName) {
    //string colCondition,
    auto it = database[name].orderOfCols.find(colName);
    if(it != database[name].orderOfCols.end()) {
        return it->second;
    }
    return -1;
}

int findConditionIndex(unordered_map<string, Table> &database, const string &name, const string &colCondition) {
    for(int i = 0; i < (int)database[name].colNames.size(); ++i) {
        if(database[name].colNames[i] == colCondition) {
            return i;
        }
    }
        return -1;
}

void insert_index(unordered_map<string, Table> &database, const string &name, int i) {
    string colName = database[name].indexCol;
    int indexOfCol = findColIndex(database, name, colName);
    if(database[name].indexType == "hash") {
        database[name].hashIndex[database[name].data[i][indexOfCol]];
        database[name].hashIndex[database[name].data[i][indexOfCol]].push_back(i);
    }//end of indexType as Hash
    else if(database[name].indexType == "bst") {
            database[name].bstIndex[database[name].data[i][indexOfCol]];
            database[name].bstIndex[database[name].data[i][indexOfCol]].push_back(i);
    }//end of indexType as bst
}

void insert(unordered_map<string, Table> &database, string name) {
    auto it = database.find(name);
    string junk = "";
    int numRows = 0;
    if(it == database.end()) {
        cin >> numRows;
        getline(cin, junk);
        cout << "Error: " << name << " does not name a table in the database\n";
        for(int i = 0; i < numRows; ++i) {
            getline(cin, junk);
        }
        return;
    }
    cin >> numRows >> junk;
    int tempSize = (int) it->second.data.size();
    it->second.data.resize(tempSize + numRows);
    for(int i = tempSize; i < tempSize + numRows; ++i) {
        for(int j = 0; j < (int)it->second.numCols; ++j) {
            string currCol = it->second.colTypes[j];
            string colName = it->second.colNames[j];
            if(currCol == "bool") {
                string t1;
                cin >> t1;
                bool valT1 = false;
                if(t1 == "true") {
                    valT1 = true;
                }
                TableEntry newEntry(valT1);
                it->second.data[i].emplace_back(valT1);
                if(database[name].indexExists && database[name].indexCol == colName) {
                    insert_index(database, name, i);
                }
            }
            else if(currCol == "double") {
                double t2;
                cin >> t2;
                TableEntry newEntry(t2);
                it->second.data[i].emplace_back(newEntry);
                if(database[name].indexExists && database[name].indexCol == colName) {
                    insert_index(database, name, i);
                }
            }
            else if(currCol == "string") {
                string t3;
                cin >> t3;
                TableEntry newEntry(t3);
                it->second.data[i].emplace_back(newEntry);
                if(database[name].indexExists && database[name].indexCol == colName) {
                    insert_index(database, name, i);
                }
            }
            else if (currCol == "int") {
                int t4;
                cin >> t4;
                TableEntry newEntry(t4);
                it->second.data[i].emplace_back(newEntry);
                if(database[name].indexExists && database[name].indexCol == colName) {
                    insert_index(database, name, i);
                }
            }
        } //end of inner while loop
    }//end of outter while loop
    int endN = tempSize + numRows - 1;
    cout << "Added " << numRows << " rows to " << name << " from position " << tempSize << " to " << endN << "\n";
} //end of insert

bool isBool(unordered_map<string, Table> &database, int k, const string &name) {
    if(database[name].colTypes[k] == "bool") {
        return true;
    }
    return false;
}

bool conditonSatisfied(unordered_map<string, Table> &database, const string &name, const string &condition, const string &dataType, int j, char op, int val2, double val3, const string &val4, bool val1Bool) {
    int indexOfCondition = findColIndex(database, name, condition);
    if(dataType == "bool") {
        TableEntry compVal(val1Bool);
        if(op == '=') {
            return CompareEqual(indexOfCondition, compVal, database[name].data[j]);
        }
        else if(op == '<') {
            return CompareLess(indexOfCondition, compVal, database[name].data[j]);
        }
        else if(op == '>') {
            return CompareMore(indexOfCondition, compVal, database[name].data[j]);
        }
    }
    else if(dataType == "int") {
        TableEntry compVal(val2);
        if(op == '=') {
            return CompareEqual(indexOfCondition, compVal, database[name].data[j]);
        }
        else if(op == '<') {
            return CompareLess(indexOfCondition, compVal, database[name].data[j]);
        }
        else if(op == '>') {
            return CompareMore(indexOfCondition, compVal, database[name].data[j]);
        }
        
    }
    else if(dataType == "double") {
        TableEntry compVal(val3);
        if(op == '=') {
            return CompareEqual(indexOfCondition, compVal, database[name].data[j]);
        }
        else if(op == '<') {
            return CompareLess(indexOfCondition, compVal, database[name].data[j]);
        }
        else if(op == '>') {
            return CompareMore(indexOfCondition, compVal, database[name].data[j]);
        }
    }
    else if(dataType == "string") {
        TableEntry compVal(val4);
        if(op == '=') {
            return CompareEqual(indexOfCondition, compVal, database[name].data[j]);
        }
        else if(op == '<') {
            return CompareLess(indexOfCondition, compVal, database[name].data[j]);
        }
        else if(op == '>') {
            return CompareMore(indexOfCondition, compVal, database[name].data[j]);
        }
    }
    return false;
}
void whatToPrint(unordered_map<string, Table> &database, string name, int j, int temp) {
    if(isBool(database, temp, name)) {
        if(database[name].data[j][temp] == true) {
            cout << "true ";        }
        else {
            cout << "false ";
        }
    }
    else {
        cout << database[name].data[j][temp] << " ";
    }
}

void print_index(unordered_map<string, Table> &database, const string &name, vector<string> &colsToPrint, const string &conditionalCol, char op, bool val1Bool, int val2, double val3, const string &val4, bool qMode) {
    string dataType = database[name].colTypes[findConditionIndex(database, name, conditionalCol)];;

    int numPrints = 0;
    if(!qMode) {
        for(int i = 0; i < (int)colsToPrint.size(); ++i) {
            cout << colsToPrint[i] << " ";
        }
        cout << "\n";
    }
    if(database[name].indexType == "bst")
    {
        auto start = database[name].bstIndex.begin();
        auto end = database[name].bstIndex.end();
        if(dataType == "bool") {
            TableEntry newEntry(val1Bool);
            if(op == '=') {
                start = database[name].bstIndex.lower_bound(newEntry);
                end = database[name].bstIndex.upper_bound(newEntry);
            }
            else if(op == '<') {
                end = database[name].bstIndex.lower_bound(newEntry);
            }
            else if(op == '>') {
                start = database[name].bstIndex.upper_bound(newEntry);
            }
        }
        else if(dataType == "int") {
            TableEntry newEntry(val2);
            if(op == '=') {
                start = database[name].bstIndex.lower_bound(newEntry);
                end = database[name].bstIndex.upper_bound(newEntry);
            }
            else if(op == '<') {
                end = database[name].bstIndex.lower_bound(newEntry);
            }
            else if(op == '>') {
                start = database[name].bstIndex.upper_bound(newEntry);
            }
        }
        else if(dataType == "double") {
            TableEntry newEntry(val3);
            if(op == '=') {
                start = database[name].bstIndex.lower_bound(newEntry);
                end = database[name].bstIndex.upper_bound(newEntry);
            }
            else if(op == '<') {
                end = database[name].bstIndex.lower_bound(newEntry);
            }
            else if(op == '>') {
                start = database[name].bstIndex.upper_bound(newEntry);
            }
        }
        else if (dataType == "string") {
            TableEntry newEntry(val4);
            if(op == '=') {
                start = database[name].bstIndex.lower_bound(newEntry);
                end = database[name].bstIndex.upper_bound(newEntry);
            }
            else if(op == '<') {
                end = database[name].bstIndex.lower_bound(newEntry);
            }
            else if(op == '>') {
                start = database[name].bstIndex.upper_bound(newEntry);
            }
        }
        
        for(auto it = start; it != end; ++it) {
            if(qMode) {
                numPrints += (int)it->second.size();
                continue;
            }
            for(int j = 0; j < (int)it->second.size(); ++j) {
                int row = it->second[j];
                    for(int i  = 0; i < (int)colsToPrint.size(); ++i) {
                        int col = database[name].orderOfCols[colsToPrint[i]];
                        //indexOfColsToPrint[i];
                        whatToPrint(database, name, row, col);
                    }//end of innermost loop
                cout << "\n";
                ++numPrints;
            }//end of 1st inner loop
        }//end of outter loop
    
    }
    
   else if(database[name].indexType == "hash") {
       auto start2 = database[name].hashIndex.begin();
       
    
       if(dataType == "bool") {
           TableEntry newEntry(val1Bool);
           start2 = database[name].hashIndex.find(newEntry);
       }
       else if(dataType == "int") {
           TableEntry newEntry(val2);
           start2  = database[name].hashIndex.find(newEntry);
       }
       else if(dataType == "double") {
           TableEntry newEntry(val3);
           start2  = database[name].hashIndex.find(newEntry);
       }
       else if (dataType == "string") {
           TableEntry newEntry(val4);
           start2  = database[name].hashIndex.find(newEntry);
       }
       
       
       if(start2 != database[name].hashIndex.end()) {
           for(int j = 0; j < (int)start2->second.size(); ++j) {
               if(qMode) {
                   numPrints = (int)start2->second.size();
                   break;
               }
               int row = start2->second[j];
               for(int i = 0; i < (int)colsToPrint.size(); ++i) {
                   int col = database[name].orderOfCols[colsToPrint[i]];
                   whatToPrint(database, name, row, col);
               }
               cout << "\n";
               ++numPrints;
            }//end of outter for loop
       }
    }//end of if hash
    cout << "Printed " << numPrints << " matching rows from " << name << "\n";
}//end of print index

void print(unordered_map<string, Table> &database, const string &name, bool qMode) {
    auto it = database.find(name);
    string junk = "";
    int numPrints = 0;
    bool indexUsed = false;
    if(it == database.end()) {
        getline(cin, junk);
        cout << "Error: " << name << " does not name a table in the database\n";
        return;
    }
    int numCols = 0;
    cin >> numCols;
    vector<string> colsToPrint(numCols);
    string cols = "";
        for(int i = 0; i < numCols; ++i) {
            cin >> cols;
            colsToPrint[i] = cols;
        }
        string cmd;
        cin >> cmd;
        if(cmd == "ALL") {
            if(!qMode) {
                for(int i = 0; i < (int)colsToPrint.size(); ++i) {
                    cout << colsToPrint[i] << " ";
                }
                cout << "\n";
            }
            else if(qMode) {
                numPrints = (int)database[name].data.size();
                cout << "Printed " << numPrints << " matching rows from " << name << "\n";
                return;
            }
            vector<int> indexOfColsToPrint(numCols);
            for(int j = 0; j < (int)indexOfColsToPrint.size(); ++j) { //find index of each column to be printed
                indexOfColsToPrint[j] = findColIndex(database, name, colsToPrint[j]);
                if(indexOfColsToPrint[j] == -1) {
                    getline(cin, junk);
                    cout << "Error: " << colsToPrint[j] << " does not name a column in " << name << "\n";
                    return;
                }
            }
            for(int j = 0; j < (int)database[name].data.size(); ++j) {
                for(int i = 0; i < (int)colsToPrint.size(); ++i) {
                    int col = indexOfColsToPrint[i];
                    whatToPrint(database, name, j, col);
                }//end of inner loop
                ++numPrints;
                cout << "\n";
            }//end of outter loop
                
        }//end of all if
        else {
            string conditionalCol = "";
            char op;
            cin >> conditionalCol >> op;
            int indexOfCondition = findColIndex(database, name, conditionalCol);
            if(indexOfCondition == -1) {
                getline(cin, junk);
                cout << "Error: " << conditionalCol << " does not name a column in " << name << "\n";
                return;
            }
            string val1 = "";
            int val2 = 0;
            double val3 = 0;
            string val4 = "";
            string dataType = "";
            bool val1Bool = false;
            dataType = database[name].colTypes[findConditionIndex(database, name, conditionalCol)];
            if(dataType == "bool") { //correctly input the 'value'
                cin >> val1;
                if(val1 == "true") {
                    val1Bool = true;
                }
            }
            else if(dataType == "int") {
                cin >> val2;
            }
            else if (dataType == "double") {
                cin >> val3;
            }
            else if (dataType == "string") {
                cin >> val4;
            }
            if(database[name].indexExists && database[name].indexType == "bst" && database[name].indexCol == conditionalCol) { //traverse with bst
                print_index(database, name, colsToPrint, conditionalCol, op, val1Bool, val2, val3, val4, qMode);
                indexUsed = true;
            }
            else if(database[name].indexExists && database[name].indexCol == conditionalCol && op == '=') { //traverse with hash for =
                print_index(database, name, colsToPrint, conditionalCol, op, val1Bool, val2, val3, val4, qMode);
                indexUsed = true;
            }
            else {
                vector<int> indexOfColsToPrint(numCols);
                for(int j = 0; j < (int)indexOfColsToPrint.size(); ++j) { //find index of each column to be printed
                    indexOfColsToPrint[j] = findColIndex(database, name, colsToPrint[j]);
                    if(indexOfColsToPrint[j] == -1) {
                        getline(cin, junk);
                        cout << "Error: " << colsToPrint[j] << " does not name a column in " << name << "\n";
                        return;
                    }
                }
                if(!qMode) {
                    for(int i = 0; i < (int)colsToPrint.size(); ++i) { //print out column headings
                        cout << colsToPrint[i] << " ";
                    }
                    cout << "\n";
                }
                for(int j = 0; j < (int)database[name].data.size(); ++j) {
                    if(conditonSatisfied(database, name, conditionalCol, dataType, j, op, val2, val3, val4, val1Bool)) {
                        for(int k = 0; k < (int)colsToPrint.size(); ++k) {
                            int col = indexOfColsToPrint[k];
                            if(!qMode) {
                                whatToPrint(database, name, j, col);
                            }
                        }
                    }//end of conditionSatisfied
                    else {
                        continue;
                    }
                    ++numPrints;
                    if(!qMode) {
                        cout << "\n";
                    }
                }//end of outter for loop
                
                
            }//end of if BST
                    
        }//end of else for WHERE clause
    if(!indexUsed) {
        cout << "Printed " << numPrints << " matching rows from " << name << "\n";
    }
}

void join_index(unordered_map<string, Table> &database, const string &tabl1, const string &tabl2, vector<string> &colsToPrint, vector<int> &indexOfCols, vector<int> &colOrigin, int indexofCondition1, bool qMode) {
    int numPrints = 0;
    for(int j = 0; j < (int)database[tabl1].data.size(); ++j) {
        int row1 = j;
        auto it = database[tabl2].hashIndex.find(database[tabl1].data[j][indexofCondition1]);
        if(it != database[tabl2].hashIndex.end()) {
            if(qMode) {
                numPrints+= (int)it->second.size();
                continue;
            }
            for(int k = 0; k < (int)it->second.size(); ++k) {
                int row2 = it->second[k];
                for(int l = 0; l < (int)colsToPrint.size(); ++l) {
                    int index = indexOfCols[l];
                    if(colOrigin[l] == 1) {
                        whatToPrint(database, tabl1, row1, index);
                    }
                    else if(colOrigin[l] == 2) {
                        whatToPrint(database, tabl2, row2, index);
                    }
                }//end of cols to print forloop
                ++numPrints;
                cout << "\n";
            }//end of vector row for loop
        }//end of hash if
    }//end of outter for loop, goes through rows of table 1
    cout << "Printed " << numPrints << " rows from joining " << tabl1 << " to " << tabl2 << "\n";
}//end join_index

void joinQMode(unordered_map<string, Table> &database, const string &tabl1, const string &tabl2, int indexOfCondition1, int indexOfCondition2) {
    unordered_map<TableEntry, vector<int>> tempHash;
    for(int i = 0; i < (int)database[tabl2].data.size(); ++i) {
        tempHash[database[tabl2].data[i][indexOfCondition2]];
        tempHash[database[tabl2].data[i][indexOfCondition2]].push_back(i);
    }
    int numPrints = 0;
    for(int j = 0; j < (int)database[tabl1].data.size(); ++j) {
        auto it = tempHash.find(database[tabl1].data[j][indexOfCondition1]);
        if(it != tempHash.end()) {
            numPrints+= (int)it->second.size();
        }
    }
    cout << "Printed " << numPrints << " rows from joining " << tabl1 << " to " << tabl2 << "\n";
    tempHash.clear();
}

void join(unordered_map<string, Table> &database, bool qMode) {
    string junk = ""; //taking in all the input needed for the function
    string tabl1 = "";
    string tabl2 = "";
    cin >> tabl1 >> junk >> tabl2;
    auto it = database.find(tabl1);
    if(it == database.end()) {
        getline(cin, junk);
        cout << "Error: " << tabl1 << " does not name a table in the database\n";
        return;
    }
    it = database.find(tabl2);
    if(it == database.end()) {
        getline(cin, junk);
        cout << "Error: " << tabl2 << " does not name a table in the database\n";
        return;
    }
    
    string conditionalCol1 = "";
    string conditionalCol2 = "";
    cin >> junk >> conditionalCol1 >> junk >> conditionalCol2;
    int numCols = 0;
    cin >> junk >> junk >> numCols;
    vector<string> colsToPrint(numCols);
    vector<int> colOrigin(numCols);
    string col = "";
    int tableNum = 0;
    int i = 0;
    while(i < numCols) { //storing the names of columns to print and their table origin
        cin >> col;
        colsToPrint[i] = col;
        cin >> tableNum;
        colOrigin[i] = tableNum;
        ++i;
    }
    int indexofCondition1 = findColIndex(database, tabl1, conditionalCol1);
    if(indexofCondition1 == -1) {
        getline(cin, junk);
        cout << "Error: " << conditionalCol1 << " does not name a column in " << tabl1 << "\n";
        return;
    }
    int indexofCondition2 = findColIndex(database, tabl2, conditionalCol2);
    if(indexofCondition2 == -1) {
        getline(cin, junk);
        cout << "Error: " << conditionalCol2 << " does not name a column in " << tabl2 << "\n";
        return;
    }
    
    vector<int> indexOfCols(numCols, 0);
    for(int i  = 0; i < (int)indexOfCols.size(); ++i) { //determines index of each column in its respective table
        if(colOrigin[i] == 1) {
            indexOfCols[i] = findColIndex(database, tabl1, colsToPrint[i]);
            if(indexOfCols[i] == -1) {
                getline(cin, junk);
                cout << "Error: " << colsToPrint[i] << " does not name a column in " << tabl1 << "\n";
                return;
            }
        }
        else if(colOrigin[i] == 2) {
            indexOfCols[i] = findColIndex(database, tabl2, colsToPrint[i]);
            if(indexOfCols[i] == -1){
                getline(cin, junk);
                cout << "Error: " << colsToPrint[i] << " does not name a column in " << tabl2 << "\n";
                return;
            }
        }
    }
    if(!qMode) {
        for(int x = 0; x < (int)colsToPrint.size(); ++x) {//prints column headings
            cout << colsToPrint[x] << " ";
        }
        cout <<"\n";
    }
    
    if(database[tabl2].indexExists && database[tabl2].indexCol == conditionalCol2 && database[tabl2].indexType == "hash"){
        join_index(database, tabl1, tabl2, colsToPrint, indexOfCols, colOrigin, indexofCondition1, qMode);
        return;
    }
    int numPrints = 0;
    if(qMode) {
            joinQMode(database, tabl1, tabl2, indexofCondition1, indexofCondition2);
            return;
    }//if quiet mode
        
    unordered_map<TableEntry, vector<int>> tempHash;
    for(int i = 0; i < (int)database[tabl2].data.size(); ++i) {
        tempHash[database[tabl2].data[i][indexofCondition2]];
        tempHash[database[tabl2].data[i][indexofCondition2]].push_back(i);
    }
        
        for(int j = 0; j < (int)database[tabl1].data.size(); ++j) {
            int row1 = j;
            auto it = tempHash.find(database[tabl1].data[j][indexofCondition1]);
            if(it != tempHash.end()) {
                for(int k = 0; k < (int)it->second.size(); ++k) {
                    int row2 = it->second[k];
                    for(int l = 0; l < (int)colsToPrint.size(); ++l) {
                        int index = indexOfCols[l];
                        if(colOrigin[l] == 1) {
                            whatToPrint(database, tabl1, row1, index);
                        }
                        else if(colOrigin[l] == 2){
                            whatToPrint(database, tabl2, row2, index);
                        }
                    }//end of cols to print forloop
                    ++numPrints;
                    cout << "\n";
                }//end of vector row for loop
            }//end of hash if
        }//end of outter for loop, goes through rows of table 1
    cout << "Printed " << numPrints << " rows from joining " << tabl1 << " to " << tabl2 << "\n";
}//end of join

void updateIndex(unordered_map<string, Table> &database, const string &name) {
    if(database[name].indexType == "hash") {
        database[name].hashIndex.clear();
    }
    else {
        database[name].bstIndex.clear();
    }
    
    string colName = database[name].indexCol;
    int indexOfCol = findColIndex(database, name, colName);
    
    if(database[name].indexType == "hash") {
        //std::hash<TableEntry> table_hash;
        for(int i = 0; i < (int)database[name].data.size(); ++i) {
            //table_hash(database[name].data[i][indexOfCol]);
            database[name].hashIndex[database[name].data[i][indexOfCol]];
            database[name].hashIndex[database[name].data[i][indexOfCol]].push_back(i);
        }
    }//end of indexType as Hash
    else if(database[name].indexType == "bst") {
        for(int i = 0; i < (int)database[name].data.size(); ++i) {
            database[name].bstIndex[database[name].data[i][indexOfCol]];
            database[name].bstIndex[database[name].data[i][indexOfCol]].push_back(i);
        }
    }//end of indexType as bst
    
}

void delete_from(unordered_map<string, Table> &database, const string &name) {
    auto it = database.find(name);
    string junk = "";
    if(it == database.end()) {
        getline(cin, junk);
        cout << "Error: " << name << " does not name a table in the database\n";
        return;
    }
    string conditionalCol = "";
    char op;
    cin >> junk >> conditionalCol >> op; //get necessary input
    int indexOfcondition = findColIndex(database, name, conditionalCol); //determine the index of the conditional column in the table
    if(indexOfcondition == -1) {
        getline(cin, junk);
        cout << "Error: " << conditionalCol << " does not name a column in " << name << "\n";
        return;
    }
    string val1 = "";
    bool val1Bool = false;
    int val2 = 0;
    double val3 = 0;
    string val4 = "";
    string dataType = database[name].colTypes[indexOfcondition]; //determine data type of comparison value
    
    
    int currRows = (int)database[name].data.size();
    int size = 0;
    if(dataType == "bool") { //correctly read in the value for comparison
        cin >> val1;
        if(val1 == "true") {
            val1Bool = true;
        }
        TableEntry newEntry(val1Bool);
        switch (op) {
            case '=': {
                Table::CompEqual comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
                break;
            case '<': {
                Table::CompLess comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
                break;
            case '>': {
                Table::CompMore comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
            default:
                break;
        }
    }
    else if(dataType == "int") {
        cin >> val2;
        TableEntry newEntry(val2);
        switch (op) {
            case '=': {
                Table::CompEqual comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
                break;
            case '<': {
                Table::CompLess comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
                break;
            case '>': {
                Table::CompMore comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
            default:
                break;
        }
    }
    else if(dataType == "double") {
        cin >> val3;
        TableEntry newEntry(val3);
        switch (op) {
            case '=': {
                Table::CompEqual comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
                break;
            case '<': {
                Table::CompLess comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
                break;
            case '>': {
                Table::CompMore comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
            default:
                break;
        }
    }
    else if(dataType == "string") {
        cin >> val4;
        TableEntry newEntry(val4);
        switch (op) {
            case '=': {
                Table::CompEqual comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
                break;
            case '<': {
                Table::CompLess comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
                break;
            case '>': {
                Table::CompMore comp(newEntry, indexOfcondition);
                auto it = remove_if(database[name].data.begin(), database[name].data.end(), comp);
                database[name].data.erase(it, database[name].data.end());
                size = currRows - (int)database[name].data.size();
                cout << "Deleted " << size << " rows from " << name << "\n";
            }
            default:
                break;
        }
    }
    
    if(database[name].indexExists && size > 0) {
        updateIndex(database, name);
    }
    
}//end of delete_from

void generate(unordered_map<string, Table> &database, const string &name) {
    auto it = database.find(name);
    string junk = "";
    if(it == database.end()) {
        getline(cin, junk);
        cout << "Error: " << name << " does not name a table in the database\n";
        return;
    }
    if(database[name].indexExists) {
        if(database[name].indexType == "hash") {
            database[name].hashIndex.clear();
        }
        else {
            database[name].bstIndex.clear();
        }
        database[name].indexExists = false;
    }
    string index= "";
    string colName = "";
    cin >> index >> junk >> junk >> colName;
    database[name].indexType = index;
    database[name].indexCol = colName;
    int indexOfCol = findColIndex(database, name, colName);
    if(indexOfCol == -1) {
        getline(cin, junk);
        cout << "Error: " << colName << " does not name a column in " << name << "\n";
        return;
    }
    if(index == "hash") {
        //std::hash<TableEntry> table_hash;
        for(int i = 0; i < (int)database[name].data.size(); ++i) {
            //table_hash(database[name].data[i][indexOfCol]);
            database[name].hashIndex[database[name].data[i][indexOfCol]];
            database[name].hashIndex[database[name].data[i][indexOfCol]].push_back(i);
        }
    }//end of indexType as Hash
    else if(index == "bst") {
        for(int i = 0; i < (int)database[name].data.size(); ++i) {
            database[name].bstIndex[database[name].data[i][indexOfCol]];
            database[name].bstIndex[database[name].data[i][indexOfCol]].push_back(i);
        }
    }//end of indexType as bst
    
    cout << "Created " << index << " index for table " << name << " on column " << colName << "\n";
    database[name].indexExists = true;
}


 
int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    int givenCommands = readCommands(argc, argv);
    bool quietMode = false;
    if(givenCommands == 1) {
        quietMode = true;
    }
    
    
    string cmd;
    string junk;
    unordered_map<string, Table> database;
    do {
        cout << "% ";
        cin >> cmd;
        
        if(cmd[0] == 'C') {
            create(database);
        }
        
        else if(cmd[0] == 'R') {
            cin >> cmd;
            remove(database, cmd);
        }
        
        else if(cmd[0] == 'I') {
            cin >> junk >> cmd;
            insert(database, cmd);
        }
        
        else if(cmd[0] == 'D') {
            cin >> junk >> cmd;
            delete_from(database, cmd);
            
        }
        else if(cmd[0] == 'P') {
            cin >> junk >> cmd;
            print(database, cmd, quietMode);
        }
    
        else if(cmd[0] == 'J') {
            join(database, quietMode);
        }
        
        else if(cmd[0] == 'G') {
            cin >> junk >> cmd;
            generate(database, cmd);
        }
        
        else if(cmd[0] == '#') {  //Comments ignored
            getline(cin, junk);
            continue;
        }
        
        else if(cmd != "QUIT") {
            getline(cin, junk);
            cout << "Error: unrecognized command" << endl;
        }

        //process command
    } while(cmd != "QUIT");
    
    if(cmd == "QUIT") {
        //delete everything
        cout << "Thanks for being silly!" << endl;
        return 0;
    }
 
}

