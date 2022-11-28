#include "KWidgetASR.h"

KWidgetASR::KWidgetASR() {
  layout_button.addWidget(&button_load);
  layout_button.addWidget(&button_reset);
  layout_button.addWidget(&label_status);
  layout_main.addLayout(&layout_button);

  layout_button.setAlignment(Qt::AlignLeft);
  //layout_main.addWidget(&label_result,Qt::AlignCenter);
  layout_main.addWidget(&label_result);

  bool_thread.store(false);
  thread_asr = nullptr;

  this->setLayout(&layout_main);

  QFont font = label_result.font();
  font.setPointSize(24);
  label_result.setFont(font);

  //label_result.setWordWrap(true);
  //label_result.setAlignment(Qt::AlignCenter);
  label_result.setAlignment(Qt::AlignLeft);

  button_load.setText("load");
  button_reset.setText("reset");
  QObject::connect(&button_load, &QToolButton::clicked, [&]() {
    QString fileName;
    QFileDialog dialog;
    fileName = dialog.getOpenFileName(this,
      tr("Open Wav File"), ".", tr("something (*.wav)"));
    emit(signal_load(fileName));
    });
  QObject::connect(&button_reset, &QToolButton::clicked, [&]() {
    qstr_text = "";
    label_result.setText(qstr_text);
    }
  );

  asr = nullptr;

  QObject::connect(this, &KWidgetASR::signal_update, this, &KWidgetASR::slot_update);

  this->setEnabled(false);


}

KWidgetASR::KWidgetASR(std::string key, std::string language):KWidgetASR() {
  Init(key, language);
}

void KWidgetASR::Init(std::string key, std::string language) {
	asr = new ASR_ETRI(std::string(key), std::string(language));
  QObject::connect(this, &KWidgetASR::signal_load, &KWidgetASR::slot_load);
  this->setEnabled(true);
}

KWidgetASR::~KWidgetASR() {

  delete asr;

  if (thread_asr) {
    delete thread_asr;
    thread_asr = nullptr;
  }
}

void KWidgetASR::Load(std::string path) {
  printf("KWidgetASR::Load(%s)\n",path.c_str());
  //emit(signal_load(QString::fromStdString(path)));

  slot_load(QString::fromStdString(path));

}

void KWidgetASR::slot_load(QString path) {
  if (path.isEmpty())
    return;
  QString result;
  std::string str_path = path.toStdString();
  if (bool_thread.load()) {
    label_result.setText("Already Recognizing");
    label_result.repaint();
    return;
  
  }

  if (thread_asr) {
    delete thread_asr;
    thread_asr = nullptr;
  }
  label_status.setText("Recognizing...");
  label_result.repaint();

  thread_asr = new std::thread([=] {this->ASR(str_path); });
  thread_asr->detach();

}

void KWidgetASR::ASR(std::string path){
  bool_thread.store(true);
  QString result;
  std::string ret = asr->Request(path);
  result = QString::fromUtf8(ret.c_str());  
  //std::cout << ret<<"\n";

  emit(signal_update(result));

  bool_thread.store(false);
}


void KWidgetASR::slot_update(QString text) {
  //qstr_text.append(QDateTime::currentDateTime().toString("hh:mm:ss")+ " : " +  text + "\n");
  //qstr_text = QString(QDateTime::currentDateTime().toString("hh:mm:ss")+ " : " +  text);
  qstr_text = QString(text);
  //label_result.setText(qstr_text);
  label_result.append(qstr_text);
  label_status.setText("");


}
