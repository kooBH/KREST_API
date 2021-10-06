#include <iostream>
#include <QApplication>
#include "KWidgetASR.h"

// SEE
// https://aiopen.etri.re.kr/guide_recognition.php
//#define KEY "YOUR-KEY"
//#define LANGUAGE "YOUR-LANGUAGE"
#define KEY "YOUR-KEY"
#define LANGUAGE "YOUR-LANGUAGE"

int main(int argc, char*argv[]){
	QApplication app(argc, argv);
	KWidgetASR widget_main(KEY, LANGUAGE);
	
	widget_main.show();
	widget_main.Load("speech_male_kor.wav");

	
	return app.exec();
}