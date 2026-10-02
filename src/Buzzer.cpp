
#include "Buzzer.hpp"
#include <QDebug>
#include <QMediaDevices>
#include <QUrl>

auto const BEEP_FILE = ":/assets/beep.pcm";

Buzzer::Buzzer(QObject *parent) : QObject(parent) {
  m_sourceFile.setFileName(BEEP_FILE);

  if (!m_sourceFile.open(QIODevice::ReadOnly)) {
    qWarning() << "Failed to open beep source file.";
  }

  QAudioFormat format{};
  format.setSampleRate(44100);
  format.setChannelCount(1);
  format.setSampleFormat(QAudioFormat::Int16);

  QAudioDevice info(QMediaDevices::defaultAudioOutput());
  if (!info.isFormatSupported(format)) {
    qWarning() << "Audio format not supported.";
    return;
  }

  m_sink = new QAudioSink(format, this);

  connect(m_sink, &QAudioSink::stateChanged, this, [this]() {
    if (m_sink->state() == QtAudio::State::ActiveState)
      m_sink->stop();
  });

  m_playable = true;
}

void Buzzer::play() {
  if (!m_playable)
    return;

  m_sink->stop();
  if (m_sourceFile.seek(0))
    m_sink->start(&m_sourceFile);
}

Buzzer::~Buzzer() {
  m_sourceFile.close();
  if (m_playable) {
    m_sink->stop();
    delete m_sink;
  }
}
