//By Prowebik

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <locale.h>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <ctime>
using namespace std;

class ffl // Functions for Lady
{
public: // Public Functions
	void message(string st) // A function that outputs a value to the console
	{
		int i = st.length();
		for (i; i >= 0; i--) {
			cout << st.substr(0, 1);
			st.erase(0, 1);
//          Sleep(20);
		}
		cout << endl;
	}
	string randSt(string *arr, int lgh) { //Рандомное значение из строкового массива
		srand(time(0)); //Автоматическая рандомизация
		lgh--;
		return arr[rand() % lgh];;
	}
	void readbook() {
		//PREPARE
		string prepareAnswers[3] = { "Выполняю", "Приступаю", "Занимаюсь" };
		string bookname;
		message("Назови имя файла");
		cin >> bookname;
		message(randSt(prepareAnswers, 3));
		//PART l
		ifstream FileR;
		string Stroka; //Read string of file
		string slovar; //Словарь
		FileR.open("words.txt");
		while (!FileR.eof()) {
			FileR >> Stroka;
			slovar += Stroka + "\n";
		}
		FileR.close();
		//PART ll
		bookname += ".txt";
		string symbols = ",.?;()-=_+!*1234567890{}[]\|/`~#$№^:abcdefghijklmnopqrstuvwxyz–";
		const char * bname = bookname.c_str();
		FileR.open(bname); //Открываем файл
		while (!FileR.eof()) {
			FileR >> Stroka;
//            transform(Stroka.begin(), Stroka.end(), Stroka.begin(), tolower);
			Stroka += " ";
			for (int i = 0; i < Stroka.length(); i++) {
				if (symbols.find(Stroka.substr(i, 1)) != string::npos) {
					Stroka.erase(i, 1);
					i--;
				}
			}
			while (Stroka != " ") {
				if (slovar.find(Stroka.substr(0, Stroka.find(" "))) != string::npos) {//Если нашли слово в словаре
					Stroka.erase(0, Stroka.find(" "));
				}
				else {//Если слово не найдено в словаре
					slovar += Stroka.substr(0, Stroka.find(" ")) + "\n";
					Stroka.erase(0, Stroka.find(" "));
				}
			}
		}
		FileR.close();
		
		//PART lll
		slovar.erase(slovar.length() - 1, 1);
		ofstream FileW;
		FileW.open("words.txt", ios_base::trunc);
		FileW << slovar;
		FileW.close();
		cout << endl;
	}
	bool syn(string Stroka, string slovo) {//Правило синонима
		ifstream FileR;
		string slovarsyn; //Список синонимов
		Stroka = " " + Stroka + " ";
		bool output = false;
		FileR.open("syn.txt");
		while (!FileR.eof()) {
			FileR >> slovarsyn;
			while (slovarsyn.find(",") != string::npos && output == false && slovarsyn.find(slovo + ":") != string::npos) {
				if (Stroka.find(" " + slovarsyn.substr(slovarsyn.find(":") + 1, slovarsyn.find(",") - slovarsyn.find(":") - 1) + " ") != string::npos) {
					output = true;
				}
				else {
					slovarsyn.erase(slovarsyn.find(":") + 1, slovarsyn.find(",") - slovarsyn.find(":"));
				}
			}
		}
		FileR.close();
		return output;
	}
}; // Конец класса FUNCTIONS FOR LADY
