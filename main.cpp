#include <boost/python.hpp>
#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
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
        return python::object();
    }

    void setattr(const std::string& attributeName, python::object obj)
    {
        qDebug() << "setattr" << attributeName.c_str() << python::extract<char*>(python::str(obj));
        if(!object_)
            throw std::runtime_error("Invalid object");

        QVariant prop(object_->property(attributeName.c_str()));
        if(!prop.isValid())
        {
            throw std::runtime_error("Invalid property: " +attributeName);
        }

        object_->setProperty(attributeName.c_str(),
                             QString::fromUtf8(python::extract<const char*>(python::str(obj))));
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


BOOST_PYTHON_MODULE(qttester)
{
    python::def("print", qt_debug_print);

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


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if(app.arguments().length() < 2)
    {
        std::cout << "Usage: qt-tester SCRIPT" << std::endl;
        return 1;
    }

    std::cout << "Starting application" << std::endl;
    qDebug() << "Foo";

    MainWindow w;

    try {
        PyImport_AppendInittab((char*)"qttester", PyInit_qttester);
        Py_Initialize();

        python::object main_module = python::import("__main__");
        python::object main_namespace = main_module.attr("__dict__");

        python::object file = python::exec_file(app.arguments().at(1).toUtf8(), main_namespace);
        // python::object file = python::exec_file(argv[1], main_namespace);
        ScriptableApplication scriptableApp;
        main_namespace["test_setting_text"](scriptableApp);
        w.show();
        int result = app.exec();
        // int result = 0;
        std::cout << "Clean exit (" << result << ")" << std::endl;
        return result;
    }
    catch(python::error_already_set& e) {
        std::cerr << "Python Exception: " << e << std::endl;
        return 255;
    }

}
