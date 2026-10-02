#pragma once
#include <QSoundEffect>
#include <QFile>
#include <QAudioSink>

class Buzzer : public QObject {
    Q_OBJECT

    public:
        explicit Buzzer(QObject *parent = nullptr);
        ~Buzzer();

    public slots:
        void play();

    private:
        QFile m_sourceFile{};
        QAudioSink *m_sink;
        bool m_playable{false};
};

