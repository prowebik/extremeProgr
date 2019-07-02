//
//  lady.cpp
//  Lady
//
//  Created by Илья Чапленко on 18.11.17.
//  Copyright © 2017 Prowebik. All rights reserved.
//

#include "ladyFuncs.hpp"
#include "conversation.hpp"
#include "people.hpp"

//Ответы пользователю, Ответы пользователя, вопросы программы, вопросы пользователя, оскорбления,
//Характер нужен для разнообразия ответа. Для смены эмоциональной окраски ответа.
int character[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int lineCount = 0;//Количество элементов словаря

void updateSelfFeels(int updateFeel, string calling){
    vector<string> feelsOut(0);
    feelsOut.push_back("Спокойствие");
    feelsOut.push_back("Радость");
    feelsOut.push_back("Гнев");
    feelsOut.push_back("Грусть");
    feelsOut.push_back("Наслаждение");
    feelsOut.push_back("Боль");
    for (int i=0;i<feelsOut.size()-1;i++){
        if (feelsOut[i]==calling){
            character[5+i+1]=(character[i+5+1]+updateFeel)/2;
        }
    }
}

void CharacterGet(){
    //Подгружаем характер из памяти
    //Подгружаем словарь из памяти
    //Будем использовать словарь кодовых слов, в качестве параметра будем передавать множество форм слова(синонимы)
    //["спокойствие", "радость", "гнев", "грусть", "наслаждение", "боль"]
    ifstream FileR;
    string dictLine;
    FileR.open("dict.txt");
    while (!FileR.eof()) {//Считаем количество строк
        FileR >> dictLine;
        lineCount++;
    }
    FileR.close();
    int i = 0;

    string state;
    FileR.open("character.txt");
    i=0;
    while (!FileR.eof()) {
        FileR >> state;
        character[i]+=atoi(state.c_str()); //Прибавляем численное значение к ячейке характера
        i++;
    }
    FileR.close();
}

void CharacterAdd(){
    //Запоминаем характер
    ofstream FileW;
    FileW.open("character.txt", ios_base::trunc);
    for (int i=0;i<12;i++){
        FileW << character[i] << endl;
    }
    FileW.close();
}

string peopleWhomProgramKnow(){
    ifstream FileR;
    string answ;
    FileR.open("peoplewhomprogramknow.txt");
    FileR >> answ;
    FileR.close();
    return answ;
}

string ifWeHaveAnyName(string inpS){
    //return word if input string has that word
    //return "" if not
    string slovo;
    Сonversation brain;//Разговор
    inpS=" "+inpS+" ";
    string peopleLine=peopleWhomProgramKnow();
    while(peopleLine.length()>0){
        slovo=peopleLine.substr(0,peopleLine.find(","));
        if(brain.syn(inpS, slovo)){
            return slovo;
        }
        peopleLine.erase(0,peopleLine.find(",")+1);
    }
    return "";
}

int main(){
    setlocale(LC_ALL, "Russian");
    Ffl lady;//Функции
    People someone(peopleWhomProgramKnow());//people whom program knows
    string peopleLine=peopleWhomProgramKnow();
    Сonversation brain;//Разговор
    Сonversation *iterConv;//Need to get acces to subclasses of "Conversation"
    Feels *iterFeel;
    iterConv = iterFeel = new Feels;//getting access to subclass "Feels" of "Conversation"
    
    ifstream FileR;
    ofstream FileW;
    string state;//Для получения строки из файла
    
    CharacterGet();//Подгружаем характер из памяти
    
    //Подгружаем имя из памяти
    FileR.open("pname.txt");
    while (!FileR.eof()) {
        FileR >> state; //Get Program Name
        lady.pname = state;
    }
    FileR.close();
    
    string inpS; // ask line
    string ansW; // ans line, need to save the diologue
    string prepareAnswers[3] = { "Выполняю", "Приступаю", "Занимаюсь" }; // in progress
    string positivAnswers[4] = { "Выполнила", "Готово", "Сделано", "Закончила" }; // done
    string bayAnswers[6] = { "Пока", "До встречи", "Желаю удачи", "Всего хорошего", "Еще увидимся", "Давай, пока"}; // good bay
    bool exitV = false; //Закрыть ли программу
    bool isStarted = false; //Начался ли диалог
    bool isNeedAns = false;
    bool isitAskLine=false;//есть ли вопрос в строка
    string obj="";//object in input string
    lady.message("Program is ready. Ver. 0.0.8. By Prowebik. For help write /help"); //Системная информация
    /*
     КОМАНДЫ ЛЕДИ:
     -Повтори
        Программа выводит последнее сообщение пользователя
     -Пока
        Закрыть программу
     -Exit
        Дополнительный способ закрыть программу
     -Переименовать
        Меняем имя программе
     -/help
        Вызвать справку по командам
     */
    string decodeF;//need for get three highiest feels of string
    string a;
    int maxIntFeel=10;
    while (!exitV){
        getline(cin, inpS);
        isNeedAns = true;
        //update selfFeelings
        decodeF=iterFeel -> decodeFeel(inpS);
        maxIntFeel=iterFeel -> getMaxFeelInt(inpS);
        updateSelfFeels(maxIntFeel,decodeF.substr(0,decodeF.find(" ")));
        
        if(brain.isItAsk(inpS)){
            inpS.erase(inpS.length()-1,1);
            isNeedAns=true;
            isitAskLine=true;
        }
        
        if(brain.getPodl(inpS).length()>0 and brain.doWeHaveVerb(inpS)){
            //searching obj in input string
            //if obj if found then remember it
            obj = brain.getPodl(inpS);
        }
        if(ifWeHaveAnyName(inpS).length()>0 and brain.syn(inpS, "знать") and brain.syn(inpS, "что") and brain.getPodl(inpS)=="ты" and isNeedAns) {
            //if user talking about anyone whom program knows
            //then program will show all information about this man
            someone.showAllInfAbout(ifWeHaveAnyName(inpS));
            ansW = lady.randSt(positivAnswers, 4);
            lady.message(ansW);
            brain.remember(ansW, lady.pname);
            isNeedAns=false;
            isitAskLine=false;
        }
        if((brain.syn(inpS, "запомнить") or brain.syn(inpS, "добавить")) and brain.syn(inpS, "человек")){
            someone.addSomeone();
            ansW = lady.randSt(positivAnswers, 4);
            lady.message(ansW);
            brain.remember(ansW, lady.pname);
            character[0]++;//ответили пользователю
            character[3]++;//пользователь спросил
            FileW.open("peoplewhomprogramknow.txt");
            FileW << (peopleLine+someone.lastMan()+",");
            FileW.close();
        }
        //Начался ли разговор
        if (!isStarted && brain.syn(inpS, "Hello")){
            //cout << "ERROR+"<<1 << endl;
            cout << "Program wanna talk about: " << iterFeel -> getStateProgram() << endl;
            ofstream Fwrite;
            Fwrite.open("memory.txt", ios_base::app);
            Fwrite << "\nNEW Dialogue" << endl;
            Fwrite.close();
            brain.remember(inpS, "prowebik");
            //cout << "ERROR+"<<2 << endl;
            brain.starting(lady.pname);
            isStarted = true;
            character[0]++;//ответили пользователю
        }else{
            brain.remember(inpS, "prowebik");
        }
        if((((brain.syn(inpS, "что") or brain.syn(inpS, "кто")) and brain.syn(inpS, "знать")) and brain.getPodl(inpS)=="ты") and isNeedAns){
            //return list of mans who knows by program
            someone.showAllPeople();
            ansW = lady.randSt(positivAnswers, 4);
            lady.message(ansW);
            brain.remember(ansW, lady.pname);
            isNeedAns=false;
            isitAskLine=false;
            character[0]++;//ответили пользователю
            character[3]++;//пользователь спросил
        }
        //FUNCS
        //1 REPEAT
        if (brain.syn(inpS, "повтори")) {
            //cout << "ERROR+"<<3 << endl;
            ansW = inpS;
            lady.message(ansW);
            brain.remember(ansW, lady.pname);
        }
        //2 RENAME
        if (brain.syn(inpS, "переименовать")){
            //cout << "ERROR+"<<4 << endl;
            lady.rename();
            //Подгружаем имя из памяти
            FileR.open("pname.txt");
            while (!FileR.eof()) {
                FileR >> state; //Get Program Name
                lady.pname = state;
            }
            FileR.close();
            //cout << "ERROR+"<<5 << endl;
            ansW = lady.randSt(positivAnswers, 4);
            lady.message(ansW);
            brain.remember(ansW, lady.pname);
        }
        //3 EXIT
        if(inpS == "exit"){
            exitV = true;
        }
        //4 HELP
        if (inpS == "/help"){
            //cout << "ERROR+"<<6 << endl;
            FileR.open("help.txt");
            while (!FileR.eof()) {
                FileR >> state;
                cout << state + " ";
                if (state.substr(state.length()-1,1) == ";" || state.substr(state.length()-1,1) == ":"){
                    cout << endl;
                }
            }
            FileR.close();
            character[3]++;//пользователь спросил
            character[0]++;//ответили пользователю
        }
        //6 Знает ли слово!!!
        if (brain.syn(inpS, "Word") && brain.syn(inpS, "Know")) {
            //cout << "ERROR+"<<7 << endl;
            string outWord = inpS.substr(inpS.find("слово ")+11,inpS.length()-5-inpS.find("слово "));
            //cout << "ERROR+"<<8 << endl;
            outWord = brain.change(outWord);
            cout << outWord << endl;
            if (brain.isInDict(outWord)){
                lady.message("Да");
            }else{
                lady.message("Нет");
            }
            isNeedAns=false;
        }
        if (isitAskLine && isNeedAns){ //Если это вопрос
            //cout << "ERROR+"<<9 << endl;
            if (!brain.isAnsDif(inpS)) {
                //Односложный ответ
                srand(time(0));
                //lady.message("Да или нет");
                string arr[2] = {"Да", "Нет"};
                ansW = arr[rand() % 2];
                lady.message(ansW);
                brain.remember(ansW, lady.pname);
            }else{
                //Сложный ответ
                ansW = "Не знаю";
                lady.message(ansW);
                brain.remember(ansW, lady.pname);
            }
            character[0]++;//ответили пользователю
            character[3]++;//пользователь спросил
        }
        //cout << "ERROR+"<<10 << endl;
        brain.isNotUnd(inpS);
        //Прощаемся
        if (brain.syn(inpS, "пока") && isNeedAns) {
            //cout << "ERROR+"<<11 << endl;
            ansW = lady.randSt(bayAnswers, 6);
            lady.message(ansW);
            brain.remember(ansW, lady.pname);
            exitV = true;
            character[0]++;//ответили пользователю
            //cout << "ERROR+"<<12 << endl;
            CharacterAdd();//Отгружаем характер в память
        }
        obj="";//clear obj string
    }
}
