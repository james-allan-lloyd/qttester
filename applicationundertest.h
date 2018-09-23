#ifndef APPLICATIONUNDERTEST_H
#define APPLICATIONUNDERTEST_H

#include <QObject>
#include <QString>
#include <QVariant>

class ApplicationUnderTest : public QObject
{
    Q_OBJECT
public:
    ApplicationUnderTest();
    virtual ~ApplicationUnderTest() override;

public slots:
    void setAttribute(QString path, QString attributeName, QVariant variant);
    void setAttribute(QObject* object, QString attributeName, QVariant variant);

    QVariant getAttribute(QObject* object, QString attributeName);

private:
    QObject* findObject(QString path);
};

#endif // APPLICATIONUNDERTEST_H
