#include <iostream>
#include <QApplication>
#include "KWidgetASR.h"

// SEE
// https://aiopen.etri.re.kr/guide_recognition.php

#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

int main(int argc, char*argv[]){
	QApplication app(argc, argv);

	std::ifstream ifs("../private/config.json");
	json j = json::parse(ifs);

	KWidgetASR widget_main(j["key"].get<std::string>(),j["language"].get<std::string>());
	
	widget_main.show();
//	widget_main.Load("speech_male_kor.wav");

	
	return app.exec();
}