//
//  people.hpp
//  People
//
//  Created by Илья Чапленко on 18.11.17.
//  Copyright © 2017 Prowebik. All rights reserved.
//

#ifndef people_hpp
#define people_hpp

class People{
public:
    vector< vector<string> > humanity;
    People(string namesOfPeople){
        //taking string line of people, whom program knows
        vector<string> someone;
        string state;
        string name;
        ifstream FileR;
        while(namesOfPeople.length()>0){
            name=namesOfPeople.substr(0,namesOfPeople.find(","));
            FileR.open(name+".txt");
            while(!FileR.eof()){
                FileR >> state;
                someone.push_back(state);
            }
            FileR.close();
            namesOfPeople.erase(0,namesOfPeople.find(",")+1);
            humanity.push_back(someone);
            someone.clear();
        }
    }
    void addSomeone(){
        //create cell of memory about new man
        vector<string> someone;
        Ffl x;
        string name;
        x.message("Введите имя этого человека");
        getline(cin, name);
        someone.push_back(name);
        string age;
        x.message("Введите возраст этого человека");
        getline(cin, age);
        someone.push_back(age);
        string quietness;
        x.message("Введите то, что успокаивает этого человека(через запятую, можно оставить поле пустым)");
        getline(cin, quietness);
        quietness+=",";
        someone.push_back(quietness);
        string happiness;
        x.message("Введите то, что делает этого человека счастливым(через запятую, можно оставить поле пустым)");
        getline(cin, happiness);
        happiness+=",";
        someone.push_back(happiness);
        string anger;
        x.message("Введите то, что раздражает этого человека(через запятую, можно оставить поле пустым)");
        getline(cin, anger);
        anger+=",";
        someone.push_back(anger);
        string sadness;
        x.message("Введите то, что делает этого человека грустным(через запятую, можно оставить поле пустым)");
        getline(cin, sadness);
        sadness+=",";
        someone.push_back(sadness);
        string enjoyment;
        x.message("Введите то, что доставляет этому человеку наслаждение(через запятую, можно оставить поле пустым)");
        getline(cin, enjoyment);
        enjoyment+=",";
        someone.push_back(enjoyment);
        string pain;
        x.message("Введите то, что доставит этому человеку боль(через запятую, можно оставить поле пустым)");
        getline(cin, pain);
        pain+=",";
        someone.push_back(pain);
        string likeList;
        x.message("Введите то, что нравится этому человеку(через запятую, можно оставить поле пустым)");
        getline(cin, likeList);
        likeList+=",";
        someone.push_back(likeList);
        string knowList;
        x.message("Введите то, кого знает этот человек(через запятую, можно оставить поле пустым)");
        getline(cin, knowList);
        knowList+=",";
        someone.push_back(knowList);
        ofstream FileW;
        FileW.open(name+".txt");
        for(int i =0; i<someone.size();i++){
            FileW << someone[i] << endl;
        }
        FileW.close();
        humanity.push_back(someone);
    }
    void showAllInfAbout(string name){
        //cout all information about that people
        bool f=false;
        string info[10]={"имя:\n","возраст:\n","спокойствие:\n","счастье:\n","гнев:\n","грусть:\n","наслаждение:\n","боль:\n","любит:\n","знает:\n"};
        for(int i=0;i<humanity.size();i++){
            if (humanity[i][0]==name){
                f=true;
                for(int j=0;j<10;j++){
                    cout << info[j] << humanity[i][j] << endl;
                }
            }
        }
        if(!f){
            cout << "ERROR! Not found that man" << endl;
        }
    }
    void showAllPeople(){
        //cout list of people whom program knows
        for(int i=0;i<humanity.size();i++){
            cout << humanity[i][0] << endl;
        }
    }
    int size(){
        return humanity.size();
    }
    string lastMan(){
        return humanity[humanity.size()-1][0];
    }
};


#endif /* people_hpp */
