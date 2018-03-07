#include <QCoreApplication>
#include <protocolstructbuilder.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (ProtocolStructBuilder::validate("../example/demo.xml")){
        qDebug() << "Validation OK";
        ProtocolStructBuilder psb("../example/demo.xml");
        psb.createStructs("demo");
    }
    else {
        qDebug() << "Error in syntax";
    }

    return 0;
}
