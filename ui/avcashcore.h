#ifndef AVCASHCORE_H
#define AVCASHCORE_H

#include <QObject>


class AvCashCore : public QObject
{
    Q_OBJECT
public:
    explicit AvCashCore();

    static bool baseInitialize();

public Q_SLOTS:
    void initialize();
    void shutdown();

Q_SIGNALS:
    void initializeResult(bool success);
    void shutdownResult();
    void runawayException(const QString &message);

private:
    void handleRunawayException(const std::exception *e);
};

#endif // AVCASHCORE_H
