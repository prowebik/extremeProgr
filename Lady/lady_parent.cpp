//By Prowebik

#include <stdio.h>
#include "ffl.h"


int main()
{
	setlocale(LC_CTYPE, "Russian");
//    SetConsoleCP(1251);
//    SetConsoleOutputCP(1251);
	ffl x;
	string inpS;
	string prepareAnswers[3] = { "Выполняю", "Приступаю", "Занимаюсь" };
	string positivAnswers[4] = { "Выполнила", "Готово", "Сделано", "Закончила" };
	string bayAnswers[6] = { "Пока", "До встречи", "Желаю удачи", "Всего хорошего", "Еще увидимся", "Давай, пока"};
	bool exitV = false;
	/*MAIN BEGIN*/
	while (!exitV) {
		getline(cin, inpS);
//        transform(inpS.begin(), inpS.end(), inpS.begin(), tolower);
		//FUNCS
		//1 REPEAT
		if (x.syn(inpS, "повтори")) {
			x.message(inpS);
		}
		//2 READ
		if (x.syn(inpS, "прочитай")) {
			x.readbook();
			x.message(x.randSt(positivAnswers, 4));
		}
		//3 OPEN FILE
		if (x.syn(inpS, "открой") && (inpS.find("файл") != string::npos)) {
			x.openFile();
			x.message(x.randSt(positivAnswers, 4));
		}
		//4 EXIT
		if (x.syn(inpS, "пока")) {
			x.message(x.randSt(bayAnswers, 6));
			int t1 = clock(), t2 = t1, p = 1;
			srand(time(0));
//   Скрыть окно

			while ((t2 % (rand() % 300000 + 3000)) != 0) {
				t1 = clock();
//                Sleep((0, 000000001));
				t2 = clock();
				p = t2 - t1;
			}
//            ShowWindow(GetConsoleWindow(), SW_SHOW);
		}
		if (inpS == "exit") {
			exitV = true;
		}
	}
	/*MAIN ENDING*/
	system("pause");
	return 0;
}

