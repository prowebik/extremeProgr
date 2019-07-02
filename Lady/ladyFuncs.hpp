//
//  ladyFuncs.hpp
//  LadyFuncs
//
//  Created by Илья Чапленко on 18.11.17.
//  Copyright © 2017 Prowebik. All rights reserved.
//

#ifndef ladyFuncs_hpp
#define ladyFuncs_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <algorithm>// find
#include <cctype>
#include <locale>


using namespace std;

class Ffl // Functions for Lady
{
public: // Public Functions
    string pname;
    Ffl(){
        ifstream FileR;
        string state;
        FileR.open("pname.txt");
        while (!FileR.eof()) {
            FileR >> state; //Get Program Name
            pname = state;
        }
        FileR.close();
    }
    void message(string st) // A function that outputs a value to the console
    {
        cout << pname + ": " + st << endl;
    }
    string randSt(string *arr, int lgh) { //Рандомное значение из строкового массива
        srand(time(0)); //Автоматическая рандомизация
        return arr[rand() % lgh];
    }
    int maxIntVect(vector<int> arr){
        int maxi=arr[0];
        for (int i=0;i<arr.size();i++){
            if (arr[i]>maxi){
                maxi=arr[i];
            }
        }
        return maxi;
    }
    void rename(){
        ofstream Fwrite;
        string state;
        string prepareAnswers[3] = { "Выполняю", "Приступаю", "Занимаюсь" };
        Fwrite.open("pname.txt", ios_base::trunc);
        message("Введи новое имя:");
        getline(cin, state);
        message(randSt(prepareAnswers, 3));
        Fwrite << state << endl;
        Fwrite.close();
    }
};


#endif /* ladyFuncs_hpp */
