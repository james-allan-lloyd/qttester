#include <boost/python.hpp>
#include "mainwindow.h"
#include "applicationundertest.h"

#include <QApplication>
#include <QDebug>
#include <QThread>
#include <iostream>

namespace python = boost::python;

// from https://wiki.python.org/moin/boost.python/EmbeddingPython
std::string extractException()
{
    using namespace boost::python;

    PyObject *exc,*val,*tb;
    PyErr_Fetch(&exc,&val,&tb);
    PyErr_NormalizeException(&exc,&val,&tb);
    handle<> hexc(exc),hval(allow_null(val)),htb(allow_null(tb));
    if(!hval)
    {
        return extract<std::string>(str(hexc));
    }
    else
    {
        object traceback(import("traceback"));
        object format_exception(traceback.attr("format_exception"));
        object formatted_list(format_exception(hexc,hval,htb));
        object formatted(str("").join(formatted_list));
        return extract<std::string>(formatted);
    }
}

std::ostream& operator<<(std::ostream& os, boost::python::error_already_set&) {
    os << extractException();
    return os;
}


void qt_debug_print(const std::string& out)
{
    qDebug() << out.c_str();
}

ApplicationUnderTest* g_aut = nullptr;

class QObjectReference
{
    QObject* object_ = nullptr;
    QObjectReference* parent_ = nullptr;
public:
    QObjectReference(QObject* object, QObjectReference* parent)
        : object_(object)
        , parent_(parent)
    {

    }

    QObjectReference child(const std::string& childName)
    {
        QString qChildName(QString::fromStdString(childName));
        if(object_)
        {
            for(QObject* obj: object_->children())
            {
                if(obj->objectName() == qChildName)
                {
                    qDebug() << "Found" << qChildName << "in" << object_->objectName();
                    return QObjectReference(obj, this);
                }
            }
            qDebug() << "Not Found" << qChildName << "in" << object_->objectName();
        }
        return QObjectReference(nullptr, this);
    }

    python::object getattr(const std::string& attributeName)
    {
        qDebug() << "getattr" << attributeName.c_str();
        // return python::object();
        QVariant result(object_->property(attributeName.c_str()));
        switch (result.type()) {
        case QVariant::Invalid:
            break;
        case QVariant::Bool:
            break;
        case QVariant::Int:
            break;
        case QVariant::UInt:
            break;
        case QVariant::LongLong:
            break;
        case QVariant::ULongLong:
            break;
        case QVariant::Double:
            break;
        case QVariant::Char:
            break;
        case QVariant::Map:
            break;
        case QVariant::List:
            break;
        case QVariant::StringList:
            break;
        case QVariant::ByteArray:
            break;
        case QVariant::BitArray:
            break;
        case QVariant::Date:
            break;
        case QVariant::Time:
            break;
        case QVariant::DateTime:
            break;
        case QVariant::Url:
            break;
        case QVariant::Locale:
            break;
        case QVariant::Rect:
            break;
        case QVariant::RectF:
            break;
        case QVariant::Size:
            break;
        case QVariant::SizeF:
            break;
        case QVariant::Line:
            break;
        case QVariant::LineF:
            break;
        case QVariant::Point:
            break;
        case QVariant::PointF:
            break;
        case QVariant::RegExp:
            break;
        case QVariant::RegularExpression:
            break;
        case QVariant::Hash:
            break;
        case QVariant::EasingCurve:
            break;
        case QVariant::Uuid:
            break;
        case QVariant::ModelIndex:
            break;
        case QVariant::PersistentModelIndex:
            break;
        case QVariant::LastCoreType:
            break;
        case QVariant::Font:
            break;
        case QVariant::Pixmap:
            break;
        case QVariant::Brush:
            break;
        case QVariant::Color:
            break;
        case QVariant::Palette:
            break;
        case QVariant::Image:
            break;
        case QVariant::Polygon:
            break;
        case QVariant::Region:
            break;
        case QVariant::Bitmap:
            break;
        case QVariant::Cursor:
            break;
        case QVariant::KeySequence:
            break;
        case QVariant::Pen:
            break;
        case QVariant::TextLength:
            break;
        case QVariant::TextFormat:
            break;
        case QVariant::Matrix:
            break;
        case QVariant::Transform:
            break;
        case QVariant::Matrix4x4:
            break;
        case QVariant::Vector2D:
            break;
        case QVariant::Vector3D:
            break;
        case QVariant::Vector4D:
            break;
        case QVariant::Quaternion:
            break;
        case QVariant::PolygonF:
            break;
        case QVariant::Icon:
            break;
        case QVariant::SizePolicy:
            break;
        case QVariant::UserType:
            break;
        case QVariant::LastType:
            break;
        case QVariant::String:
            return python::object(result.toString().toStdString());
        }

        return python::object();
    }

    void setattr(const std::string& attributeName, python::object obj)
    {
        qDebug() << "setattr" << attributeName.c_str() << python::extract<char*>(python::str(obj));
        QVariant variant = python::extract<char*>(python::str(obj));
        QMetaObject::invokeMethod(g_aut, "setAttribute", Qt::QueuedConnection,
                                  Q_ARG(QObject*, object_),
                                  Q_ARG(QString, QString::fromStdString(attributeName)),
                                  Q_ARG(QVariant, variant));
    }
};

class ScriptableApplication
{
public:
    ScriptableApplication()
    {

    }

    ~ScriptableApplication()
    {

    }

    QObjectReference child(const std::string& childName)
    {
        QString qChildName(QString::fromStdString(childName));
        for(QObject* obj: QApplication::topLevelWidgets())
        {
            if(obj->objectName() == QString::fromStdString(childName))
            {
                qDebug() << "Found top level" << qChildName;
                return QObjectReference(obj, nullptr);
            }
        }

        qDebug() << "No such top level" << qChildName;
        return QObjectReference(nullptr, nullptr);
    }
};


void end_test()
{
    std::cout << "ending tests" << std::endl;
    QMetaObject::invokeMethod(qApp, "closeAllWindows", Qt::QueuedConnection);
    std::cout << "tests ended" << std::endl;
}


BOOST_PYTHON_MODULE(qttester)
{
    python::def("print", qt_debug_print);
    python::def("end_test", end_test);

    python::class_<QObjectReference>("QObjectReference", python::no_init)
           .def("child", &QObjectReference::child)
           .def("__getattr__", &QObjectReference::getattr)
           .def("__setattr__", &QObjectReference::setattr)
           ;

    python::class_<ScriptableApplication>("ScriptableApplication")
           .def("child", &ScriptableApplication::child)
           ;
}

// #if PY_MAJOR_VERSION >= 3
// #   define INIT_MODULE PyInit_qttester
//     extern "C" PyObject* INIT_MODULE();
// #else
// #   define INIT_MODULE initqttester
//     extern "C" void INIT_MODULE();
// #endif

class with_gil
{
public:
    with_gil()  { state_ = PyGILState_Ensure(); }
    ~with_gil() { PyGILState_Release(state_);   }

    with_gil(const with_gil&)            = delete;
    with_gil& operator=(const with_gil&) = delete;
private:
    PyGILState_STATE state_;
};

class TestingThread : public QThread
{
    python::object main_namespace;
    void run() override {
        try {
            PyImport_AppendInittab(const_cast<char*>("qttester"), PyInit_qttester);
            Py_Initialize();

            python::object main_module = python::import("__main__");
            main_namespace = main_module.attr("__dict__");
            python::object file = python::exec_file("qttester.py", main_namespace);
        }
        catch(python::error_already_set& e) {
            std::cerr << "Python Exception: " << e << std::endl;
        }
        std::cout << "thread exit" << std::endl;
    }

public:
    TestingThread()
        : QThread()
    {
       start();
    }

    void stop()
    {
        // try {
        //     // with_gil g;
        //     python::object close = main_namespace["close"];
        //     if(!close)
        //     {
        //         std::cerr << "Couldn't find close method" << std::endl;
        //     }
        //     else
        //     {
        //         close();
        //         std::cout << "closed" << std::endl;
        //     }
        // }
        // catch(python::error_already_set& e) {
        //     std::cerr << "Python Exception while closing: " << std::endl;
        // }
        wait();
        std::cout << "Thread exit clean" << std::endl;
    }

    virtual ~TestingThread() override {
        // stop();
    }
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    std::cout << "Starting application" << std::endl;

    MainWindow w;
    TestingThread thread;
    ApplicationUnderTest aut;
    g_aut = &aut;

    int ret = 0;
    w.show();
    ret = app.exec();
    thread.stop();
    std::cout << "Clean exit (" << ret << ")" << std::endl;
    return ret;
}
