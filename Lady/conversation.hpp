//
//  conversation.hpp
//  Conversation
//
//  Created by Илья Чапленко on 18.11.17.
//  Copyright © 2017 Prowebik. All rights reserved.
//

#ifndef conversation_hpp
#define conversation_hpp

class Сonversation{// Functions for analyzing the dialogue
public:
    string change(string askSt){
        //Трансформируем строку в удобный формат
        askSt+=" ";
        while (askSt.find("  ") != string::npos){// Удаляем лишние пробелы
            askSt.erase(askSt.find("  "),1);
        }
        while (askSt.find("!") != string::npos){// Удаляем лишние пробелы
            askSt.erase(askSt.find("!"),1);
        }
        while (askSt.find("?") != string::npos){// Удаляем лишние пробелы
            askSt.erase(askSt.find("?"),1);
        }
        while (askSt.find(".") != string::npos){// Удаляем лишние пробелы
            askSt.erase(askSt.find("."),1);
        }
        return askSt;
    }
    bool isInDict(string word){
        //Есть ли в словаре
        ifstream FileR;
        bool output = false;
        string dictLine;
        FileR.open("dict.txt");
        while (!FileR.eof() && output == false) {
            FileR >> dictLine;
            while (dictLine.find(word+",") != string::npos){
                if (( dictLine.find(","+word+",") != string::npos or dictLine.find(":"+word+",") != string::npos) and ( dictLine.find(word+",") > dictLine.find("sclon:") ) and ( dictLine.find(word+",") <= dictLine.find("smisl:") )){
                    output = true;//Есть
                }
                dictLine.erase(dictLine.find(word+",") ,word.length()+1);
            }
        }
        FileR.close();
        return output;
    }
    void isNotUnd(string askSt){
        //Не все понятно
        string word;
        askSt = change(askSt);
        while (askSt.length() > 0){
            //Каждое слово проверяем в словаре
            word = askSt.substr(0, askSt.find(" "));
            if (!isInDict(word)){
                newword(word);
            }else{
                askSt.erase(0, askSt.find(" ")+1);
            }
        }
    }
    bool isAnsDif(string askSt){
        //Сложный ли вопрос
        Ffl x;
        
        if(askSt.find(" ") == string::npos){
            askSt.erase(askSt.length()-1,1);
            askSt+=" ";
        }
        if(!syn(askSt, "Ask")){
            return false;
        }else{
            return true;
        }
    }
    bool isItAsk(string askSt){
        //Вопрос ли это
        if (askSt.find("?") != string::npos){
            return true;
        }else{
            return false;
        }
    }
    void starting(string progName){
        //Начало диалога
        Ffl x;
        x.pname = progName;
        string helloAns[3]={"Привет!", "Здравствуй!", "О! Привет!"};
        string ansW = x.randSt(helloAns, 3);
        x.message(ansW);
        remember(ansW, x.pname);
    }
    void remember(string state, string progName){
        //Запоминаем фразу в памяти
        Ffl x;
        ofstream Fwrite;
        x.pname = progName;
        Fwrite.open("memory.txt", ios_base::app);
        Fwrite << x.pname + ": " + state << endl;
        Fwrite.close();
    }
    string chRech(string askSt){
        //Узнаем часть речи слова по вопросу
        string ansW = "";
        if (askSt == "кто?" || askSt == "что?"){
            ansW = "сущ$";
        }
        if (askSt == "чтоделать?" || askSt == "чтосделать?"){
            ansW = "глаг$";
        }
        if (askSt == "какой?" || askSt == "какая?" || askSt == "какое?"){
            ansW = "прилаг$";
        }
        if (askSt == "сколько?" || askSt == "который?"){
            ansW = "числ$";
        }
        if (askSt == "чтоделая?" || askSt == "чтосделав?"){
            ansW = "деепр$";
        }
        if (askSt == "как?"){
            ansW = "нар$";
        }
        if (askSt == "междомет?"){
            ansW = "междомет$";
        }
        return ansW;
    }
    /*
     Функция поиска:
     -кода +
     -вопроса
     -синонимов+
     -роли
     -чувств +
     -склонений
     -служебное ли(tech==1)
     если служебное, то не учитываем в разборе
     Функция добавления:
     -Вопрос+
     Спрашиваем
     -Синонимы+
     Спрашиваем
     -Роль+
     По вопросу
     -Чувства+
     По синонимам
     -Склонения
     По вопросу + роли
     -tech or not+
     По вопросу + роли
     $code:открыть$ask:чтосделать$syn:открой,прочитай,найди,покажи,отыщи$role:глаг$feel:10,10,10,10,10,10$sclon:открою,откроешь,откроют,откроет,откроем,$smisl:не закрыть$tech:0$
     $code:мы$syn:мы$role:местоим$feel:10,10,10,10,10,10$sclon:мы$tech:1;
     */
    string getSyn(string code){
        //return string of syns of this word by code
        ifstream FileR;
        string syn="";
        string dictLine;
        FileR.open("dict.txt");
        while (!FileR.eof()) {
            FileR >> dictLine;
            //$code:открыть$ask:чтосделать$syn:открой,прочитай,найди,покажи,отыщи$role:глаг$feel:10,10,10,10,10,10$sclon:открою,откроешь,откроют,откроет,откроем,$smisl:не закрыть$tech:0$
            if (dictLine.find("$code:"+code) != string::npos){
                syn=dictLine.substr(dictLine.find("$syn:")+5,dictLine.find("$role:")-dictLine.find("$syn:")-5);
            }
        }
        FileR.close();
        return syn;
    }
    string getCode(string word){
        //return string of code of this word from dict
        ifstream FileR;
        string code="0";
        string dictLine;
        FileR.open("dict.txt");
        while (!FileR.eof()) {
            FileR >> dictLine;
            while (dictLine.find(word+",") != string::npos){
                if (( dictLine.find(","+word+",") != string::npos or dictLine.find(":"+word+",") != string::npos) and ( dictLine.find(word+",") > dictLine.find("sclon:") ) and ( dictLine.find(word+",") <= dictLine.find("smisl:") )){
                    code=dictLine.substr(6,dictLine.find("$ask:")-6);
                }
                
                dictLine.erase(dictLine.find(word+",") ,word.length()+1);
            }
        }
        FileR.close();
        return code;
    }
    string getCodeByCode(string word){
        //return string of code of this word from dict
        ifstream FileR;
        string code="0";
        string dictLine;
        FileR.open("dict.txt");
        while (!FileR.eof()) {
            FileR >> dictLine;
            if (dictLine.find("$code:"+word) != string::npos){
                code=dictLine.substr(6,dictLine.find("$ask:")-6);
            }
        }
        FileR.close();
        return code;
    }
    string getLineByCode(string word){
        //return string line of this word from dict by code
        ifstream FileR;
        string line="";
        string dictLine;
        FileR.open("dict.txt");
        while (!FileR.eof()) {
            FileR >> dictLine;
            if (dictLine.find("$code:"+word) != string::npos){
                line=dictLine;
            }
        }
        FileR.close();
        return line;
    }
    string getSmislByCode(string code){
        string line = getLineByCode(code);
        return line.substr(line.find("$smisl:")+7,line.find("$tech")-line.find("$smisl:")-7);
    };
    int countOfSt(string stroka1, string stroka2){
        int k = 0;
        while (stroka1.find(stroka2) != string::npos){
            k++;
            stroka1.erase(stroka1.find(stroka2),stroka2.length());
        }
        return k;
    }
    void addSclon(string code, string sclon){
        //add this sclon to line with this code in dict
        string line = getLineByCode(code);
        //$code:открыть$ask:чтосделать$syn:открой,прочитай,найди,покажи,отыщи$role:глаг$feel:10,10,10,10,10,10$sclon:открою,откроешь,откроют,откроет,откроем,$smisl:не закрыть$tech:0$
        string lpart=line.substr(0, line.find("$smisl:"));
        string rpart=line.substr(line.find("$smisl:"), line.length()-line.find("$smisl:")+1);
        line = lpart+sclon+","+rpart;
        //cout << "ERROR " + line << endl;
        ifstream FileR;
        ofstream FileI;
        string dict="";
        string dictLine;
        FileR.open("dict.txt");
        while (!FileR.eof()) {
            FileR >> dictLine;
            if (dictLine.find("$code:"+code) != string::npos){
                dict+=line+"\n";
                //cout << code + " " + line << endl;
            }else{
                if (dict.find(dictLine) == string::npos){
                    dict+=dictLine+"\n";
                }
            }
        }
        FileR.close();
        if (countOfSt(dict, line)>1){
            dict.erase(dict.find(line), line.length()+1);
        }        FileI.open("dict.txt");
        while (dict.length()>0) {
            FileI << dict.substr(0,dict.find("\n")+1);
            dict.erase(0,dict.find("\n")+1);
        }
        FileI.close();
    }
    int countOfSymbol(string s, string symbol){
        int count=0;
        for(int i=0;i<s.length();i++){
            if (s.substr(i,1)==symbol){
                count++;
            }
        }
        return count;
    }
    string getFeels(string smisl,string code){
        //return string line of feelings of this word by words smisl
        string feels="";
        string word;//dyn variable for get word in smisl line
        vector<string> feelLine;
        //иметь,приятный,чувство,
        while(smisl.length()>0){
            word = smisl.substr(0,smisl.find(","));//get every word of smisl line
            //cout << "ERROR 1 "+word << endl;
            smisl.erase(0,smisl.find(",")+1);
            if(isInDict(word)){
                string line=getLineByCode(getCode(word));
                //cout << "ERROR 2 "+line << endl;
                //$code:открыть$ask:чтосделать$syn:открой,прочитай,найди,покажи,отыщи$role:глаг$feel:10,10,10,10,10,10$sclon:открою,откроешь,откроют,откроет,откроем,$smisl:не закрыть$tech:0$
                feelLine.insert(feelLine.end(), line.substr(line.find("feel:")+5,line.find("$sclon:")-line.find("feel:")-5));
            }else{
                    //we iterate over all feelings for this word
                    ifstream FileR;
                    string state;
                    FileR.open("dict.txt");
                    while (!FileR.eof()) {
                        FileR >> state;
                        if(( state.find(","+word+",") != string::npos or state.find(":"+word+",") != string::npos) and ( state.find(word+",") > state.find("syn:") ) and ( state.find(word+",") <= state.find("role:") )){
                            //if this word in this syns line
                            //cout << "ERROR 3 "+state << endl;
                            feelLine.insert(feelLine.end(), state.substr(state.find("feel:")+5,state.find("$sclon:")-state.find("feel:")-5));//got feelings line for this word
                        }
                    }
                    FileR.close();
            }
        }
        string feelings="";
        int feelInt;
        for (int j = 0; j<6;j++){
            feelInt=0;
            if (feelLine.size()<1){
                feelInt=10;
                feels+=to_string(feelInt)+",";
            }else{
                for (int i=0;i<feelLine.size();i++){
                    feelings=feelLine[i];
                    feelInt+=stoi(feelings.substr(0,feelings.find(",")));
                    feelings.erase(0,feelings.find(",")+1);
                    feelLine[i]=feelings;
                }
                feels+=to_string(feelInt/feelLine.size())+",";
            }
            //cout << "ERROR 4 "+to_string(feelLine.size()) << endl;
            
        }
        return feels.substr(0,feels.length()-1);
    }
    void newword(string word){
        //Запоминаем слово
        Ffl x;
        string ansW=" ";
        string code;//code
        string ask;//ask
        string syn = word+",";//syn
        string tech;
        string role;//role
        string feel = "10,10,10,10,10,10";//default values
        string sclon = word+",";//default
        string smisl = "NONE";//default
        vector<string> mestoim(8);
        mestoim.push_back("я");
        mestoim.push_back("ты");
        mestoim.push_back("мы");
        mestoim.push_back("вы");
        mestoim.push_back("он");
        mestoim.push_back("она");
        mestoim.push_back("оно");
        mestoim.push_back("они");//я, ты, мы, вы, он (она, оно, они)
        x.message("Введите начальную форму слова: "+word);
        getline(cin, code);
        if (getCodeByCode(code)=="0"){
            //if we don't know this word at all
            x.message("Введите вопрос к слову: "+code);
            getline(cin, ask);
            //asking synms:
            x.message("введите синонимы к слову: "+code);
            x.message("enter = конец ввода");
            while (ansW != ""){
                getline(cin, ansW);
                syn+=ansW+",";
            }
            syn.erase(syn.length()-1,1);
            while (ask.find(" ") != string::npos){// Удаляем пробелы
                ask.erase(ask.find(" "),1);
            }
            if (find(mestoim.begin(), mestoim.end(), word) != mestoim.end()){
                role = "мест$";
            }else{
            role = chRech(ask);
            }
            if (role!="междомет$"){
                tech="0";
            }else{
                tech="1";
            }
            if (code != word){
                sclon+=code+",";
            }
            //get smisl
            if (tech!="1"){
                x.message("Введите смысл слова: "+code);
                getline(cin, smisl);
                bool doWeHaveNo = false;
                if (smisl.find("не ") != string::npos){
                    doWeHaveNo = true;
                }
                smisl+=" ";
                while (smisl.find(" ") != string::npos){// Удаляем пробелы
                    smisl.insert(smisl.find(" ")+1, ",");
                    smisl.erase(smisl.find(" "),1);
                }
                feel=getFeels(smisl,code);
                if (doWeHaveNo){
                    feel=inversion(feel);
                }
            }
            ofstream Fwrite;
            Fwrite.open("dict.txt", ios_base::app);
            Fwrite << "$code:"+code+"$ask:"+ask+"$syn:"+syn+"$role:"+role+"feel:"+feel+"$sclon:"+sclon+"$smisl:"+smisl+"$tech:"+tech+"$"<< endl;
            Fwrite.close();
        }else{
            //if we know this word, but don't know this sclon
            addSclon(code,word);
        }
        
        cout << endl;
    }
    bool syn(string stroka, string slovo){
        //is slovo in stroka
        stroka+=" ";
        string word;
        while (stroka.length() > 0){
            word = stroka.substr(0,stroka.find(" "));
            stroka.erase(0,stroka.find(" ")+1);
            if (getSyn(slovo).find(word+",") != string::npos){
                return true;
            }
        }
        return false;
    }
    string inversion(string feelsLine){
        //10,10,10,10,10,10
        feelsLine+=",";
        string ansW="";
        while (feelsLine.length()>0){
            ansW+=to_string(100-stoi(feelsLine.substr(0,feelsLine.find(","))))+",";
            feelsLine.erase(0,feelsLine.find(",")+1);
        }
        ansW=ansW.substr(0,ansW.length()-1);
        return ansW;
    }
    string getChRech(string line){
        //return chRech("сущ","междомет"...) by dictLine
        return line.substr(line.find("$role:")+6,line.find("$feel:")-line.find("$role:")-6);
    }
    string getPodl(string stroka){
        //return string of podl of this input string
        //return 0 if have not
        string slovo="";
        stroka+=" ";
        string askLine=" где когда куда откуда почему зачем как какой какая какие какое кому кем кого что ";
        while (stroka.length()>0){
            slovo = stroka.substr(0,stroka.find(" "));
            if (((getCode(slovo)==slovo and getChRech(getLineByCode(getCode(slovo)))=="сущ") or getChRech(getLineByCode(getCode(slovo)))=="мест") and askLine.find(" "+getCode(slovo)+" ")==string::npos){
                return slovo;
            }
            stroka.erase(0, stroka.find(" ")+1);
        }
        return "";
    }
    bool doWeHaveVerb(string stroka){
        stroka+=" ";
        string slovo="";
        while(stroka.length()>0){
            slovo=stroka.substr(0,stroka.find(" "));
            if(getChRech(getLineByCode(getCode(slovo)))=="глаг"){
                return true;
            }
            stroka.erase(0,stroka.find(" ")+1);
        }
        return false;
    }

   
};

class Feels: public Сonversation{
    //subclass of Conversation
public:
    string decodeFeel(string stroka){
        //return three most popular feels of stroka
        Ffl x;
        string slovo="";
        string feelsLine="";//line of feelings of this word 0,0,0,0,0,0
        string code="";//code of slovo
        stroka+=" ";
        vector<string> feelsOut(0);
        feelsOut.push_back("Спокойствие");
        feelsOut.push_back("Радость");
        feelsOut.push_back("Гнев");
        feelsOut.push_back("Грусть");
        feelsOut.push_back("Наслаждение");
        feelsOut.push_back("Боль");
        string decodeOut="";
        int dynFeel=0;
        vector<int> feels(6,0);
        while (stroka.length()>0){
            slovo=stroka.substr(0,stroka.find(" "));
            code = getCodeByCode(slovo);
            stroka.erase(0, stroka.find(" ")+1);
            if (isInDict(slovo)){
                feelsLine=findFeelsByWord(slovo)+",";
                for (int i=0;i<6;i++){
                    dynFeel=feels[i]+stoi(feelsLine.substr(0,feelsLine.find(",")));
                    feels[i]=dynFeel;
                    feelsLine.erase(0,feelsLine.find(",")+1);
                }
            };
        }
        for (int j=0;j<3;j++){
            for (int i=0;i<feels.size();i++){
                if (feels[i]==x.maxIntVect(feels)){
                    decodeOut+=feelsOut[i]+" ";
                    feels.erase(feels.begin()+i);
                    feelsOut.erase(feelsOut.begin()+i);
                    break;
                }
            }
        }
        return decodeOut;
    }
    int getMaxFeelInt(string stroka){
        //return int count of max feeling of that stroka
        Ffl x;
        string slovo="";
        string feelsLine="";//line of feelings of this word 0,0,0,0,0,0
        string code="";//code of slovo
        stroka+=" ";
        vector<string> feelsOut(0);
        feelsOut.push_back("Спокойствие");
        feelsOut.push_back("Радость");
        feelsOut.push_back("Гнев");
        feelsOut.push_back("Грусть");
        feelsOut.push_back("Наслаждение");
        feelsOut.push_back("Боль");
        string decodeOut="";
        int dynFeel=0;
        vector<int> feels(6,0);
        while (stroka.length()>0){
            slovo=stroka.substr(0,stroka.find(" "));
            code = getCodeByCode(slovo);
            stroka.erase(0, stroka.find(" ")+1);
            if (isInDict(slovo)){
                feelsLine=findFeelsByWord(slovo)+",";
                for (int i=0;i<6;i++){
                    dynFeel=feels[i]+stoi(feelsLine.substr(0,feelsLine.find(",")));
                    feels[i]=dynFeel;
                    feelsLine.erase(0,feelsLine.find(",")+1);
                }
            };
        }
        return x.maxIntVect(feels);
    }
    string findFeelsByWord(string word){
        //return string of feels of this word from dict
        ifstream FileR;
        string state="";//dictLine
        FileR.open("dict.txt");
        while (!FileR.eof()) {
            FileR >> state;
            if (isInDict(word)){
                if (state.find("$code:"+getCode(word)) != string::npos){
                    return state.substr(state.find("$feel:")+6,state.find("$sclon:")-state.find("$role:")-6);
                }
            }
        }
        FileR.close();
        return "";
    }
    string getStateProgram(){
        //return max current feel (word)
        ifstream FileR;
        string state="";//dictLine
        vector<string> feelsOut(0);
        feelsOut.push_back("Спокойствие");
        feelsOut.push_back("Радость");
        feelsOut.push_back("Гнев");
        feelsOut.push_back("Грусть");
        feelsOut.push_back("Наслаждение");
        feelsOut.push_back("Боль");
        FileR.open("character.txt");
        int i=0, max=0, k=0, min=101;
        while (!FileR.eof()) {
            FileR >> state;
            i++;
            if (stoi(state)>max and i>8 and i!=11){
                max=stoi(state);
                if(((100-min)-max)<0){
                    k=i-7;
                }
            }else{
                if (stoi(state)<min and i>6 and i!=12 and i!=9 and i!=10){
                    min=stoi(state);
                    if (((100-min)-max)>0){
                        k=i-7;
                    }
                }
            }
        }
        FileR.close();
        return feelsOut[k];
    }
};

#endif /* conversation_hpp */
