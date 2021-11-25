#pragma once

#include <QWidget>
#include <QLabel>
#include <QTextBrowser>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDateTime >

#include "ASR_ETRI.h"
#include <string>
#include <thread>
#include <atomic>

class KWidgetASR : public QWidget {
	Q_OBJECT
private :
	QVBoxLayout layout_main;
	QHBoxLayout layout_button;
	QString qstr_text;

	ASR_ETRI* asr;

	std::thread* thread_asr;
	std::atomic<bool> bool_thread;

public:
	QToolButton button_load;
	QToolButton button_reset;

	QTextBrowser label_result;

	KWidgetASR(std::string key, std::string language);
	KWidgetASR();
	~KWidgetASR();

	void Init(std::string key, std::string language);
	void Load(std::string path);
	void ASR(std::string path);
	

signals : 
	void signal_load(QString path);
	void signal_update(QString text);

public slots : 
	void slot_load(QString path);
	void slot_update(QString text);
	void slot_reset();

};