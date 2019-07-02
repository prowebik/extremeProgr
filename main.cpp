#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

/* СТРУКТУРЫ НАЧАЛО */

struct Subject{
    //фамилия и имя преподавателя
    string teacher;
    int teacherID;
    //название предмета
    string title;
    //количество занятий в неделю
    int numOfLessons;
    //осталось занятий в неделю
    int currentNumOfLessons;
    //лекционный зал
    bool isItLecture=false;
    //лабораторная
    bool isItLaboratory=false;
    //внутренний ли поток
    bool isItInner=true;
    //если уже назначили этот предмет, то true
    bool isAlreadySorted=false;
};

//общие для группы и комнаты данные
struct timetableTemplate{
public:
    int code;//код группы
    int num;//количество студентов
};

struct Group:timetableTemplate{
    //число предметов у конкретной группы
    int numOfSubjects;
    //массив предметов у конкретной группы
    Subject* subjects;
};

struct Room:timetableTemplate{
    //параметры для каждой комнаты
    
    //сколько студентов еще можно вместить
    int currentCapacity;
    Subject currentSubject;
};

enum Commands{
    //записать в файл
    writeIntoFile=1,
    //прочитать из файла
    readFromFile=2,
    //создать расписание на одну пару
    makeOneLessonTimetable=3,
    //создать расписание на всю неделю
    makeWholeTimetable=4,
    //сменить режим на расписание по преподавателям
    changeForTeacher=5,
    //сменить режим на расписание по группам
    changeForStudents=6,
    //сменить режим на расписание по кабинетам
    changeForRooms=7,
    //начать работу программы
    startProgram=404,
    //закончить работу программы
    stopProgram=9,
    //вывести список групп
    writeGroups=8,
};

enum CurrentMode{
    //режим вывода расписания для преподавателей
    teacher,
    //режим вывода расписания для студентов
    student,
    //режим вывода расписания для комнат
    room,
    all
};

/* СТРУКТУРЫ КОНЕЦ */




/*Timetable BEGIN*/

void printSubject(Subject x){
    cout << "Subj. ";
    cout << x.title;
    cout << endl;
    cout << " teacher:";
    cout << x.teacher;
    cout << endl;
    cout << " lessons:";
    cout << x.currentNumOfLessons;
    cout << "/";
    cout << x.numOfLessons;
    if(x.isItLecture){
        cout << endl;
        cout << " it is lecture";
    }
    if(x.isItLaboratory){
        cout << endl;
        cout << " it is laboratory";
    }
    if(x.isItInner){
        cout << endl;
        cout << " it is inner";
    }else{
        cout << endl;
        cout << " it is external";
    }
    cout << ";";
    cout << endl;
}

//расписание в конкретной комнате
class Timetable{
    //индекс последней группы
    int lastIndex;
    //массив групп
    Group* thisGroups;
    
    //увеличить количество групп
    void addLength(int newLength);
    
    
public:
    //конкретная комната
    Room thisRoom;
    //сколько групп в комнате
    int groupLength;
    //конструктор для создания массива, или пустого экземпляра
    Timetable();
    //конструктор для создания расписания конкретной комнаты
    Timetable(Room data);
    //добавить группу в расписание этой комнаты
    void add(Group data);
    //распечатать расписание этой комнаты
    void print();
    bool saveForRoom();
    bool doesItHasThisGroup(Group x){
        for(int g=0;g<groupLength;g++)
            if(thisGroups[g].code==x.code)
                return true;
        return false;
    }
    void refreshData(){
        //задаем начальные параметры
        lastIndex=0;
        groupLength=1;
        
        delete thisGroups;
        //создаем массив комнат, НО ОН ПОКА ЧТО ПУСТОЙ
        thisGroups=new Group[groupLength];
        thisRoom.currentCapacity=thisRoom.num;
    }
};


Timetable::Timetable(){}

Timetable::Timetable(Room data){
    //передаем комнату
    thisRoom = data;
    //задаем начальные параметры
    lastIndex=0;
    groupLength=1;
    //создаем массив комнат, НО ОН ПОКА ЧТО ПУСТОЙ
    thisGroups=new Group[groupLength];
}

void Timetable::addLength(int newLength){
    //сюда будем копировать текущий список групп
    Group* dyn;
    dyn=new Group[newLength];
    //копируем каждую комнату
    for(int i=0;i<groupLength;i++){
        Group groupCopy = thisGroups[i];
        dyn[i]=groupCopy;
    }
    
    //очищаем массив
    delete thisGroups;
    //обновляем количество групп
    groupLength=newLength;
    //создаем новый список групп
    thisGroups=new Group[groupLength];
    //возвращаем список групп
    for(int i=0;i<groupLength;i++)
        thisGroups[i]=dyn[i];
}

void Timetable::add(Group data){
    //выделяем память для группы
    Group dyn=data;
    if(lastIndex<groupLength){
        //если у нас есть еще место в списке групп
        //добавляем группу
        thisGroups[lastIndex]=dyn;
        lastIndex++;
    }else{
        //если нет места в списке групп, увеличиваем
        //количество элементов в списке в два раза
        int newLength = 2*groupLength;
        //изменяем длину списка
        addLength(newLength);
        //добавляем новый элемент
        thisGroups[lastIndex]=dyn;
        lastIndex++;
    }
}

void Timetable::print(){
    
    if(lastIndex>0){
        cout << "Room number ";
        cout << thisRoom.code;
        
        cout << " has ";
        
        cout << thisRoom.currentCapacity;
        cout << "/";
        cout << thisRoom.num;
        cout << " free chairs.";
        //если добавляли какие-то группы в эту комнату
        cout << endl;
        cout << "This room has groups: ";
        //печатаем все группы в этой комнате
        for(int j=0;j<lastIndex;j++){
            cout << endl;
            cout << thisGroups[j].code;
            cout << "(";
            cout << thisGroups[j].num;
            cout << " students)";
            if(j<lastIndex-1)
                cout <<", ";
            else
                cout <<".";
        }
        cout << endl;
        printSubject(thisRoom.currentSubject);
        cout <<endl;
    }
}

bool Timetable::saveForRoom(){
    ofstream fOut;
    
    fOut.open("timetableForRooms.txt", ios_base::app);
    
    
        if(lastIndex>0){
            fOut << "Room number ";
            
            fOut << thisRoom.code;
            
            
            fOut << " has ";
            
            fOut << thisRoom.currentCapacity;
            
            fOut << "/";
            fOut << thisRoom.num;
            fOut << " free chairs.";
            fOut << endl;
            fOut << "This room has groups: ";
            //печатаем все группы в этой комнате
            for(int j=0;j<lastIndex;j++){
                fOut << endl;
                fOut << thisGroups[j].code;
                fOut << "(";
                fOut << thisGroups[j].num;
                fOut << " students)";
                if(j<lastIndex-1)
                    fOut <<", ";
                else
                    fOut <<".";
            }
            fOut << endl;
            Subject x = thisRoom.currentSubject;
            fOut << "Subj. ";
            fOut << x.title;
            fOut << endl;
            fOut << " teacher:";
            fOut << x.teacher;
            fOut << endl;
            fOut << " lessons:";
            fOut << x.currentNumOfLessons;
            fOut << "/";
            fOut << x.numOfLessons;
            if(x.isItLecture){
                fOut << endl;
                fOut << " it is lecture";
            }
            if(x.isItLaboratory){
                fOut << endl;
                fOut << " it is laboratory";
            }
            if(x.isItInner){
                fOut << endl;
                fOut << " it is inner";
            }else{
                fOut << endl;
                fOut << " it is external";
            }
            fOut << ";";
            fOut << endl;
            fOut <<endl;
        }

    
    fOut.close();
    
    
    
    
    return true;
}

/*Timetable END*/

/* ПЕРЕМЕННЫЕ НАЧАЛО */

//инициализация констант
const int numGroups=30;
//число кабинетов
const int numRooms=40;
//обязательно больше 9
//число записей предметов
//(именно в расписании, т.е. связка препод+предмет)
const int numOfSubjects=80;
//для кого сейчас выводим расписание
CurrentMode currentMode=all;
//какая сейчас пара по счету
int currLsnNum=0;
//какая сейчас неделя по счету
int currWeekNum=0;
int currDayOfWeek=1;

//объявление массивов данных
//само расписание
Timetable timetable[numRooms];
//массив групп студентов
Group groups[numGroups];
//массив записей предметов
Subject subjs[numOfSubjects];

//массив имен преподавателей
string names[14]={"A.",
    "B.",
    "P.",
    "G.",
    "S.",
    "E.",
    "I.",
    "Z.",
    "O.",
    "L.",
    "Z.",
    "K.",
    "C.",
    "X.",
},

//массив фамилий преподавателей
surnames[24]={"Jobs",
    "Bolgarian",
    "Putin",
    "Rasputin",
    "Jackson",
    "Banderos",
    "Musk",
    "Geytz",
    "McEvoy",
    "Ostankov",
    "Petrov",
    "Sidorov",
    "Ostrov",
    "Sosnin",
    "Sorokin",
    "Arabia",
    "Obama",
    "Merkel",
    "Federov",
    "Maramov",
    "Baranov",
    "Volkov",
    "Sidorov",
    "Simono",
};

//если смогли распределить студентов
bool madeADecision=true;


//массив преподавателей
string teachers[numOfSubjects];

/* ПЕРЕМЕННЫЕ КОНЕЦ */


void readData();

bool isAlreadyHaveThisTeacher(string name){
    for(int i=0;i<numOfSubjects;i++)
        if(teachers[i]==name)
            return true;
    return false;
}

//процедура заполнения массивов данных начальными значениями
void generateData(){
    //список предметов
    string subs[24]={"Math",
                    "Programming",
                    "Art",
                    "Physics",
                    "Linear algebra",
                    "Boolean algebra",
                    "Extreme programming",
                    "Physical culture",
                    "Economy",
                    "Philosofy",
                    "3D modelling",
                    "Computer architecture",
                    "WEB-design",
                    "Neural networks",
                    "Operating Systems",
                    "Game design",
                    "Gameplay programming",
                    "English language",
                    "Russian language",
                    "History",
                    "Chemistry",
                    "Biology",
                    "Marketing",
                    "Management"
    };
    
    for (int i=0; i<numOfSubjects; i++) {
        //surname name
        //ex.:
        //"Jobs S."
        do{
            teachers[i]=surnames[rand()%15]+" "+names[rand()%14];
        }while(!isAlreadyHaveThisTeacher(teachers[i]));
    }
    //количество преподавателей каждого предмета
    int subN[24]={0};
    // итератор для первого прохода по массиву предметов
    int iteratorSubs=0;
    
    //заполним данными предметы
    for (int i=0; i<numOfSubjects; i++) {
        //временная переменная
        Subject dyn;
        //добавили к записи предмета преподавателя
        dyn.teacher=teachers[i];
        dyn.teacherID=i;
        //выбрали сам предмет
        int key =rand() % 24;
        //если хотя бы один предмет не встречается ни одного раза
        while(iteratorSubs<24){
            if(subN[iteratorSubs]==0 && key != iteratorSubs){
                key =rand() % 24;
            }
            if(subN[iteratorSubs]==0 && key == iteratorSubs){
                subN[iteratorSubs]++;
                iteratorSubs++;
                dyn.title=subs[key];
            }
        }
        //если все предметы уже как минимум один раз встретились
        if(iteratorSubs>=24)
            dyn.title=subs[key];
        dyn.numOfLessons=rand() % 6+2;
        dyn.currentNumOfLessons=dyn.numOfLessons;
        int isItLecture=rand()%100;
        if (isItLecture%2==0){
            dyn.isItLecture=true;
        }
        if(isItLecture%3==0){
            dyn.isItLaboratory=true;
            dyn.isItLecture=false;
        }
        if(key == 3 ||
           key ==7||
           key ==9||
           key ==16||
           key ==17||
           key ==18)
            dyn.isItInner=false;
        
        subjs[i]=dyn;
    }
    
    //сначала заполним данными список групп
    for (int i=0; i<numGroups; i++){
        Group dynGroup;
        dynGroup.code=i;
        dynGroup.num=rand() % 30 +10;
        int dynNumSubs=rand() % 24+6 ;
        dynGroup.numOfSubjects=dynNumSubs;
        dynGroup.subjects=new Subject[dynGroup.numOfSubjects];
        Subject masKeys[dynGroup.numOfSubjects];
        for(int j=0;j<dynGroup.numOfSubjects;j++){
            int key = rand()%numOfSubjects;
            bool weCanAdd;
            do{
                weCanAdd = true;
                key = rand()%numOfSubjects;
                for(int k=0;k<j;k++){
                    if(masKeys[k].title==subjs[key].title &&
                       masKeys[k].isItLecture==subjs[key].isItLecture)
                    {
                        weCanAdd=false;
                    }
                }
            }while(!weCanAdd);
            masKeys[j]=subjs[key];
            dynGroup.subjects[j]=subjs[key];
        }
        groups[i]=dynGroup;
    }
    
    //теперь заполним комнаты в расписании
    for (int i=0; i<numRooms; i++){
        //создадим абстрактную комнату
        Room dynRoom;
        //дадим ей номер
        dynRoom.code=100+i*10;
        //сколько вмещает студентов
        dynRoom.num=(rand() % 100)*2 +20;
        dynRoom.currentCapacity=dynRoom.num;
        
        //создадим расписание этой комнаты
        Timetable dynTimetable(dynRoom);
        //добавим это расписание комнаты в общее расписание
        timetable[i] =dynTimetable;
    }
}

//функция проверки идентичности двух предметов
bool isItTheSameSubect(Subject first, Subject second){
    if(first.title==second.title&&
       first.isItInner==second.isItInner&&
       (first.isItLaboratory==second.isItLaboratory||
        first.isItLecture==second.isItLecture)&&
       first.teacher==second.teacher&&
       first.currentNumOfLessons==second.currentNumOfLessons&&
       first.teacherID==second.teacherID)
        return true;
    return false;
}

//функция проверки вхождения предмета 'that' в массив 'arr', длины n
bool doesItHasThesame(Subject arr[], int n, Subject that){
    for(int i=0;i<n;i++){
        if(isItTheSameSubect(arr[i], that))
            return true;
    }
    return false;
}

/*
 Если в комнате есть место
    если тут никто не сидит, и у группы есть нераспределенный предмет
 
    если тут кто-то сидит, и у группы есть этот нераспределенный предмет
 */

//процедура составления расписания на одну пару
void sopostavlenie(){
    int k=0, save=k;
    for (int i=0; i<numGroups; i++){
        while(k<numRooms && madeADecision){
            if (timetable[k].thisRoom.currentCapacity-groups[i].num>=0){
                if(timetable[k].thisRoom.currentCapacity==timetable[k].thisRoom.num){
                    //первая группа в этой аудитории устанавливает
                    //предмет, который будут тут изучать
                    
                    bool done=false;
                    for(int l=0;l<groups[i].numOfSubjects;l++){
                        
                        for(int il=0;il<numOfSubjects;il++){
                            if(!groups[i].subjects[l].isAlreadySorted &&
                                groups[i].subjects[l].currentNumOfLessons>0){
                                timetable[k].thisRoom.currentSubject=groups[i].subjects[l];
                                if(groups[i].subjects[l].currentNumOfLessons<=0)
                                    groups[i].subjects[l].isAlreadySorted=true;
                                done = true;
                                //закидываем эту группу в комнату k
                               
                                groups[i].subjects[l].currentNumOfLessons--;
                            timetable[k].thisRoom.currentSubject.currentNumOfLessons=groups[i].subjects[l].currentNumOfLessons;
                                timetable[k].add(groups[i]);
                                timetable[k].thisRoom.currentCapacity-=groups[i].num;
                                break;
                            }
                            
                            
                        }
                        if(done)
                            break;
                        
                    }
                    if(!done)
                    {
                        cout << "Group ";
                        cout << groups[i].code;
                        cout << " has 0 lessons on that pair";
                        cout << endl;
                        save=k;
                        k++;
                        break;
                    }
                    
                    break;
                }else{
                    
                    if(doesItHasThesame(groups[i].subjects, groups[i].numOfSubjects, timetable[k].thisRoom.currentSubject)){
                        //закидываем эту группу в комнату k
                        timetable[k].add(groups[i]);
                        timetable[k].thisRoom.currentCapacity-=groups[i].num;
                        for(int l=0;l<groups[i].numOfSubjects;l++)
                            if (isItTheSameSubect(groups[i].subjects[l],timetable[k].thisRoom.currentSubject)){
                                groups[i].subjects[l].currentNumOfLessons--;
                            }
                        break;
                    }else{
                        save=k;
                        k++;
                    }
                }
                
                
            }else{
                save=k;
                k++;
            }
        }
        
//        if (k==numRooms){
//            cout << "Failed to make a decision";
//            madeADecision=false;
//            cout << endl;
//            currLsnNum--;
//            break;
//        }
        
        k=save;
    }
    
}

//процедура вывода информации о всех группах
void printGroups(){
    for(int i=0;i<numGroups;i++){
        cout << "Group number ";
        cout << groups[i].code;
        
        cout << " has ";
        cout << groups[i].num;
        
        cout << " students.";
        
        cout <<endl;
        
        for(int j=0;j<groups[i].numOfSubjects;j++)
            printSubject(groups[i].subjects[j]);
        
        cout << endl;
    }
}

Room whereIsThisGroupSeats(Group x){
    Room dyn;
    for(int i=0;i<numRooms;i++)
        if(timetable[i].doesItHasThisGroup(x)){
            dyn =  timetable[i].thisRoom;
            
        }
    return dyn;
}

Room whereIsThisTeacherSeats(string name, int id){
    Room dyn;
    for(int i=0;i<numRooms;i++)
        if(timetable[i].thisRoom.currentSubject.teacher==name&&
           timetable[i].thisRoom.currentSubject.teacherID == id){
            dyn =  timetable[i].thisRoom;
            
        }
    return dyn;
}

bool saveForGroups(){
    ofstream fOut;
    
    for(int i=0;i<numGroups;i++){
        const string addres="GroupCode_"+to_string(groups[i].code);
        Room thisRoom =  whereIsThisGroupSeats(groups[i]);
        fOut.open(addres, ios_base::app);
        
        string days[6]={
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday"
        };
        
        
        fOut << "TIMETABLE FOR ";
        fOut << currWeekNum;
        fOut << " WEEK";
        fOut << endl;
        
        fOut << "TIMETABLE FOR ";
        fOut << days[currDayOfWeek];
        fOut << endl;
        
        if(currLsnNum<=5)
            fOut << "TIMETABLE FOR ";
        else
            fOut << "TIMETABLE FOR ADDITIVE ";
        fOut << currLsnNum;
        fOut << " LESSON:";
        fOut << endl;
        fOut << "Room: ";
        fOut << thisRoom.num;
        fOut << ";";
        fOut << endl;
        
        fOut << "Teacher ";
        fOut << thisRoom.currentSubject.teacher;
        fOut << ";";
        fOut << endl;
        
        Subject x = thisRoom.currentSubject;
        for(int j=0;j<groups[i].numOfSubjects;j++)
            for(int k=0;k<numRooms;k++)
                if(isItTheSameSubect(timetable[k].thisRoom.currentSubject, groups[i].subjects[j]))
                    x=groups[i].subjects[j];
        fOut << "Subject: ";
        fOut << x.title;
        fOut << endl;
        
        fOut << " lessons:";
        fOut << x.currentNumOfLessons;
        fOut << "/";
        fOut << x.numOfLessons;
        if(x.isItLecture){
            fOut << endl;
            fOut << " it is lecture";
        }
        if(x.isItLaboratory){
            fOut << endl;
            fOut << " it is laboratory";
        }
        if(x.isItInner){
            fOut << endl;
            fOut << " it is inner";
        }else{
            fOut << endl;
            fOut << " it is external";
        }
        fOut << ";";
        fOut << endl;

        fOut.close();
        
        cout << "Group number ";
        cout << groups[i].code;
        cout << " done!";
        cout << endl;
    }
    
    return true;
}

bool saveForTeacher(){
    ofstream fOut;
    
    for(int i=0;i<numOfSubjects;i++){
        const string addres="Teacher_id_"+ to_string(i)+"_"+teachers[i];
        Room thisRoom =  whereIsThisTeacherSeats(teachers[i], i);
        if(thisRoom.currentSubject.teacher!=teachers[i]){
            cout << "Teacher ";
            cout << teachers[i];
            cout << " have 0 groups!";
            cout << endl;
            continue;
        }
        fOut.open(addres, ios_base::app);
        
        string days[6]={
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday"
        };
        
        
        fOut << "TIMETABLE FOR ";
        fOut << currWeekNum;
        fOut << " WEEK";
        fOut << endl;
        
        
        fOut << "TIMETABLE FOR ";
        fOut << days[currDayOfWeek];
        fOut << endl;
        
        if(currLsnNum<=5)
            fOut << "TIMETABLE FOR ";
        else
            fOut << "TIMETABLE FOR ADDITIVE ";
        fOut << currLsnNum;
        fOut << " LESSON:";
        fOut << endl;
        fOut << "Room: ";
        fOut << thisRoom.num;
        fOut << ";";
        fOut << endl;
        
        
        Subject x = thisRoom.currentSubject;
        fOut << "Subject: ";
        fOut << x.title;
        if(x.isItLecture){
            fOut << endl;
            fOut << "it is lecture";
        }
        if(x.isItLaboratory){
            fOut << endl;
            fOut << "it is laboratory";
        }
        if(x.isItInner){
            fOut << endl;
            fOut << "it is inner";
        }else{
            fOut << endl;
            fOut << "it is external";
        }
        
        for(int j=0;j<numGroups;j++){
            if(doesItHasThesame(groups[j].subjects,groups[j].numOfSubjects, x)){
                fOut << endl;
                fOut << "GroupCode_";
                fOut << to_string(groups[j].code);
            }
        }
        
        fOut << ";";
        fOut << endl;
        
        fOut.close();
        
        cout << "Teacher ";
        cout << teachers[i];
        cout << " done!";
        cout << endl;
    }
    
    return true;
}

bool saveForStudent(){
    
    return saveForGroups();
}

bool saveTimetableIntoFile(){
    bool rtrn=false;
    ofstream fOut;
    switch (currentMode) {
        case teacher:
            fOut.open("timetableForTeachers.txt", ios_base::app);
            break;
        case room:
            fOut.open("timetableForRooms.txt", ios_base::app);
            break;
        case student:
            fOut.open("timetableForStudents.txt", ios_base::app);
            break;
        case all:
            fOut.open("timetableForRooms.txt", ios_base::app);
            break;
            
        default:
            return false;
            break;
    }
    
    string days[6]={
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
    };
    
    
    fOut << "TIMETABLE FOR ";
    fOut << currWeekNum;
    fOut << " WEEK";
    fOut << endl;
        
    
    fOut << "TIMETABLE FOR ";
    fOut << days[currDayOfWeek];
    fOut << endl;
    
    if(currLsnNum<=5)
        fOut << "TIMETABLE FOR ";
    else
        fOut << "TIMETABLE FOR ADDITIVE ";
    fOut << currLsnNum;
    fOut << " LESSON:";
    fOut << endl;
    fOut.close();
    
    
    switch (currentMode) {
        case teacher:
            if( saveForTeacher()){
                
                
                rtrn =  true;
            }
            break;
        case student:
            if(saveForStudent()){
                rtrn=  true;
            }
            break;
        case room:
            for(int i=0;i<numRooms;i++){
                if(!timetable[i].saveForRoom())
                    return false;
                cout << "Room ";
                cout << i;
                cout << " done!";
                cout << endl;
            }
            
            rtrn=  true;
            break;
        case all:
            saveForTeacher();
            saveForStudent();
            for(int i=0;i<numRooms;i++){
                if(!timetable[i].saveForRoom())
                    return false;
                cout << "Room ";
                cout << i;
                cout << " done!";
                cout << endl;
            }
            
            
            rtrn= true;
            break;
            
        default:
            rtrn=  false;
            break;
    }
    
    for(int i=0;i<numRooms;i++)
        if(timetable[i].groupLength>1)
            timetable[i].refreshData();
    return rtrn;
}

Commands menuCommandCheck(){
    Commands rtrn;
    
    int fl = 0;//выбор команды пользователем
    
    switch (currentMode) {
        case room:
            cout << "Current mode is for rooms.";
            cout << endl;
            break;
        case teacher:
            cout << "Current mode is for teachers.";
            cout << endl;
            break;
        case student:
            cout << "Current mode is for students.";
            cout << endl;
            break;
        case all:
            cout << "Current mode is for all.";
            cout << endl;
            break;
            
        default:
            break;
    }
    
    cout << "Write current timetable into file -- 1";
    cout << endl;
    cout << "Read timetable from file -- 2";
    cout << endl;
    cout << "Make timetable for 1 lesson -- 3";
    cout << endl;
    cout << "Make timetable for a 3 weeks -- 4";
    cout << endl;
    cout << "Change mode to teachers  -- 5";
    cout << endl;
    cout << "Change mode to groups -- 6";
    cout << endl;
    cout << "Change mode to rooms -- 7";
    cout << endl;
    cout << "Print groups list -- 8";
    cout << endl;
    cout << "Exit -- 9";
    cout << endl;
    
    cin >> fl;
    switch (fl) {
        case writeIntoFile:
            rtrn=writeIntoFile;
            break;
        case readFromFile:
            rtrn=readFromFile;
            break;
        case makeOneLessonTimetable:
            rtrn=makeOneLessonTimetable;
            break;
        case makeWholeTimetable:
            rtrn=makeWholeTimetable;
            break;
        case changeForTeacher:
            rtrn=changeForTeacher;
            break;
        case changeForStudents:
            rtrn=changeForStudents;
            break;
        case changeForRooms:
            rtrn=changeForRooms;
            break;
        case stopProgram:
            rtrn=stopProgram;
            break;
        case writeGroups:
            rtrn=writeGroups;
            break;
            
        default:
            cout << "Command not found! Try Again!";
            cout << endl;
            rtrn=startProgram;
            break;
    }
    
    return rtrn;
}

//процедура управления программой(текстовый интерфейс)
void manager(Commands cmd){
    
    cout << "Greetings! What do you want to select?";
    cout << endl;
    
    while(cmd!=stopProgram){
        
        cmd = menuCommandCheck();
        
        cout << endl;
        cout << "Working on it...";
        cout << endl;
        cout << endl;
        
        switch (cmd) {
            case writeIntoFile:
                if(!saveTimetableIntoFile()){
                    cout << "Cannot do that!";
                    cout << endl;
                }else{
                    cout << "Done!";
                    cout << endl;
                }
                break;
            case readFromFile:
                cout << "Not ready!";
                cout << endl;
                break;
            case makeOneLessonTimetable:
                sopostavlenie();
                currLsnNum++;
                break;
            case makeWholeTimetable:
                for(int w=1;w<5;w++){
                    currWeekNum=w;
                    if(w<3){
                        for(int d=0;d<5;d++){
                            currDayOfWeek=d;
                            for(int i=1;i<6;i++){
                                sopostavlenie();
                                currLsnNum=i;
                                saveTimetableIntoFile();
                            }
                            
                            
                        }
                    }else{
                        for(int d=0;d<5;d++){
                            currDayOfWeek=d;
                            for(int i=6;i<8;i++){
                                sopostavlenie();
                                currLsnNum=i;
                                saveTimetableIntoFile();
                            }
                            
                            
                        }
                    }
                    
                }
                break;
            case changeForTeacher:
                currentMode = teacher;
                break;
            case changeForStudents:
                currentMode = student;
                break;
            case changeForRooms:
                currentMode = room;
                break;
            case stopProgram:
                system(EXIT_SUCCESS);
                break;
            case writeGroups:
                printGroups();
                break;
                
            default:
                break;
        }
        
        
    }
    
}

void readData(){
    //список предметов
    string subs[24]={"Math",
        "Programming",
        "Art",
        "Physics",
        "Linear algebra",
        "Boolean algebra",
        "Extreme programming",
        "Physical culture",
        "Economy",
        "Philosofy",
        "3D modelling",
        "Computer architecture",
        "WEB-design",
        "Neural networks",
        "Operating Systems",
        "Game design",
        "Gameplay programming",
        "English language",
        "Russian language",
        "History",
        "Chemistry",
        "Biology",
        "Marketing",
        "Management"
    };
    
    for (int i=0; i<numOfSubjects; i++) {
        //surname name
        //ex.:
        //"Jobs S."
        do{
            teachers[i]=surnames[rand()%15]+" "+names[rand()%14];
        }while(!isAlreadyHaveThisTeacher(teachers[i]));
    }
    //количество преподавателей каждого предмета
    int subN[24]={0};
    // итератор для первого прохода по массиву предметов
    int iteratorSubs=0;
    
    //заполним данными предметы
    for (int i=0; i<numOfSubjects; i++) {
        //временная переменная
        Subject dyn;
        //добавили к записи предмета преподавателя
        dyn.teacher=teachers[i];
        dyn.teacherID=i;
        //выбрали сам предмет
        int key =rand() % 24;
        //если хотя бы один предмет не встречается ни одного раза
        while(iteratorSubs<24){
            if(subN[iteratorSubs]==0 && key != iteratorSubs){
                key =rand() % 24;
            }
            if(subN[iteratorSubs]==0 && key == iteratorSubs){
                subN[iteratorSubs]++;
                iteratorSubs++;
                dyn.title=subs[key];
            }
        }
        //если все предметы уже как минимум один раз встретились
        if(iteratorSubs>=24)
            dyn.title=subs[key];
        
        subjs[i]=dyn;
    }
    
    
    ifstream file;
    //num of groups
    file.open("data.txt");
    
    string line;
    getline(file, line);
    int n=stoi(line);
    
    for (int i=0; i<n; i++){
        Group dynGroup;
        //code of group
        getline(file, line);
        cout << "GROUP ";
        cout << line;
        cout << " IS READY";
        cout << endl;
        dynGroup.code=stoi(line);
        //num of students
        getline(file, line);
        dynGroup.num=stoi(line);
        //numofSubs
        getline(file, line);
        int dynNumSubs=stoi(line);
        dynGroup.numOfSubjects=dynNumSubs;
        dynGroup.subjects=new Subject[dynGroup.numOfSubjects];
        Subject masKeys[dynGroup.numOfSubjects];
        for(int j=0;j<dynGroup.numOfSubjects;j++){
            //title
            getline(file, line);
            for(int k=0;k<numOfSubjects;k++)
                if(line==("Subj. "+ subjs[k].title)){
                    dynGroup.subjects[j]=subjs[k];
                    //num of lessons
                    getline(file, line);
                    dynGroup.subjects[j].numOfLessons=stoi(line);
                    dynGroup.subjects[j].currentNumOfLessons=dynGroup.subjects[j].numOfLessons;

                    //type
                    getline(file, line);
                    if(line=="inner"){
                        //inner
                        dynGroup.subjects[k].isItInner=true;
                    }else {
                        if (line=="external"){
                        //external
                        dynGroup.subjects[k].isItInner=false;
                        }else{
                            //laboratory/lecture
                            if (line== "laboratory"){
                                //external
                                dynGroup.subjects[k].isItLaboratory=true;
                            }else if (line=="lecture"){
                                //external
                                dynGroup.subjects[k].isItLecture=true;
                            }
                            
                            getline(file, line);
                            if(line =="inner"){
                                //inner
                                dynGroup.subjects[k].isItInner=true;
                            }else if (line =="external"){
                                //external
                                dynGroup.subjects[k].isItInner=false;
                            }
                        }
                    }
                }
        }
        groups[i]=dynGroup;
    }
    
    
    file.close();
    
    //теперь заполним комнаты в расписании
    for (int i=0; i<numRooms; i++){
        //создадим абстрактную комнату
        Room dynRoom;
        //дадим ей номер
        dynRoom.code=100+i*10;
        //сколько вмещает студентов
        dynRoom.num=(rand() % 100)*2 +20;
        dynRoom.currentCapacity=dynRoom.num;
        
        //создадим расписание этой комнаты
        Timetable dynTimetable(dynRoom);
        //добавим это расписание комнаты в общее расписание
        timetable[i] =dynTimetable;
    }
}

int main(int argc, const char * argv[]) {
    srand(time(0));
    ofstream fout;
    fout.open("timetableForTeachers.txt", ios_base::trunc);
    
    fout.close();
    fout.open("timetableForStudents.txt", ios_base::trunc);
    
    fout.close();
    fout.open("timetableForRooms.txt", ios_base::trunc);
    
    fout.close();
    
    /*кабинеты при выводе не совпадают?
        из-за очистки данных при записи в файл( refreshData() )
     
     проверить additive пары, не правильно считает currentNumOfLessons*/
    
    cout << "Generate data -- 0";
    cout << endl;
    cout << "Load data from the file -- 1";
    cout << endl;
    int answ=0;
    cin >> answ;
    if(answ==1)
        readData();
    else
        generateData();
    
    manager(startProgram);
    
    return 0;
}
