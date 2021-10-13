#include "KWidgetASR.h"

KWidgetASR::KWidgetASR() {
  layout_main.addWidget(&button_load);
  layout_main.addWidget(&label_result,Qt::AlignCenter);

  this->setLayout(&layout_main);

  QFont font = label_result.font();
  font.setPointSize(48);
  label_result.setFont(font);

  label_result.setWordWrap(true);
  label_result.setAlignment(Qt::AlignCenter);

  button_load.setText("load");
  
  label_result.setText("label");
  QObject::connect(&button_load, &QToolButton::clicked, [&]() {
    QString fileName;
    QFileDialog dialog;
    fileName = dialog.getOpenFileName(this,
      tr("Open Wav File"), ".", tr("something (*.wav)"));
    emit(signal_load(fileName));
    });

  asr = nullptr;

 
}

KWidgetASR::KWidgetASR(std::string key, std::string language) {
  Init(key, language);
 
}

void KWidgetASR::Init(std::string key, std::string language) {
	asr = new ASR_ETRI(std::string(key), std::string(language));
  QObject::connect(this, &KWidgetASR::signal_load, &KWidgetASR::slot_load);
}

KWidgetASR::~KWidgetASR() {

  delete asr;

}

void KWidgetASR::Load(std::string path) {
    emit(signal_load(QString::fromStdString(path)));

}

void KWidgetASR::slot_load(QString path) {
  if (path.isEmpty())
    return;
  QString result;
  std::string str_path = path.toStdString();
  label_result.setText("Recognizing...");
  label_result.repaint();
  std::string ret = asr->Request(str_path);
  result = QString::fromUtf8(ret.c_str());  

 // std::cout << ret<<"\n";

  label_result.setText(result);


}