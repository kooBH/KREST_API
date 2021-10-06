#pragma once

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QFileDialog>


#include "ASR_ETRI.h"

#include <string>


class KWidgetASR : public QWidget {
	Q_OBJECT
private :
	QVBoxLayout layout_main;

	ASR_ETRI* asr;

public:
	QToolButton button_load;
	QLabel label_result;

	KWidgetASR(std::string key, std::string language);
	~KWidgetASR();

	void Load(std::string path);

signals : 
	void signal_load(QString path);

public slots : 
	void slot_load(QString path);

};