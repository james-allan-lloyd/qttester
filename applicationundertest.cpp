#include "applicationundertest.h"

#include <stdexcept>
#include <QDebug>

ApplicationUnderTest::ApplicationUnderTest()
{

}

ApplicationUnderTest::~ApplicationUnderTest()
{

}

void ApplicationUnderTest::setAttribute(QString path, QString attributeName, QVariant variant)
{
    QObject* object = findObject(path);
    if(!object)
        throw std::runtime_error("Invalid object");

    setAttribute(object, attributeName, variant);
}

void ApplicationUnderTest::setAttribute(QObject *object, QString attributeName, QVariant variant)
{
    QVariant prop(object->property(attributeName.toUtf8()));
    if(!prop.isValid())
    {
        qWarning() << "Invalid property:" << attributeName;
    }
    else
    {
        object->setProperty(attributeName.toUtf8(), variant);
    }
}

QVariant ApplicationUnderTest::getAttribute(QObject *object, QString attributeName)
{
    return object->property(attributeName.toUtf8());
}

QObject *ApplicationUnderTest::findObject(QString path)
{
    return nullptr;
}
