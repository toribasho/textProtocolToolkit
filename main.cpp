#include <QCoreApplication>
#include <protocolstructbuilder.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (ProtocolStructBuilder::validate("meteoDecodeRules.xml")){
        qDebug() << "Validation OK";
    }
    else {
        qDebug() << "Error in syntax";
    }

    return 0;
}
