#include "KWidgetASR.h"

KWidgetASR::KWidgetASR(std::string key, std::string language) {
  layout_main.addWidget(&button_load);
  layout_main.addWidget(&label_result);
  this->setLayout(&layout_main);

  button_load.setText("load");
  
  label_result.setText("label");
  
	asr = new ASR_ETRI(std::string(key), std::string(language));

  
  QObject::connect(this, &KWidgetASR::signal_load, &KWidgetASR::slot_load);
  QObject::connect(&button_load, &QToolButton::clicked, [&]() {
    QString fileName;
    QFileDialog dialog;
    fileName = dialog.getOpenFileName(this,
      tr("Open Wav File"), ".", tr("something (*.wav)"));
    emit(signal_load(fileName));
    });
}

KWidgetASR::~KWidgetASR() {

  delete asr;

}

void KWidgetASR::Load(std::string path) {

  emit(signal_load(QString::fromStdString(path)));

}

void KWidgetASR::slot_load(QString path) {
  QString result;
  std::string str_path = path.toStdString();
  std::string ret = asr->Request(str_path);
  result = QString::fromUtf8(ret.c_str());  

 // std::cout << ret<<"\n";

  label_result.setText(result);


}