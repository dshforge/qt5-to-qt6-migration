// Characterisation probe for the one behavioural change in this migration.
//
// QScadaBoardController writes device configuration XML through a
// QTextStream whose encoding was set with setCodec("UTF-8") under Qt 5
// and setEncoding(QStringConverter::Utf8) under Qt 6. Both compile.
// This asserts they produce byte-identical output.
//
// One source file, built under both Qt versions. The only difference is
// the API call, which is what we are testing.

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Content chosen to exercise the encoder rather than pass by luck:
    // ASCII, Latin-1 range, symbols outside it, CJK, and a codepoint
    // above the BMP that requires a surrogate pair in UTF-16.
    const QString payload = QStringLiteral(
        "<scada>\n"
        "  <device name=\"Pump-01\" unit=\"25.4 \u00B0C\"/>\n"
        "  <device name=\"Vanne-\u00E9t\u00E9\" tol=\"\u00B10.5\"/>\n"
        "  <device name=\"\u041D\u0430\u0441\u043E\u0441\" note=\"cyrillic\"/>\n"
        "  <device name=\"\u6D41\u91CF\u8A08\" note=\"cjk\"/>\n"
        "  <device name=\"\U0001F6A6\" note=\"astral plane\"/>\n"
        "</scada>\n");

    const QString path = argc > 1 ? QString::fromLocal8Bit(argv[1])
                                  : QStringLiteral("probe-out.xml");

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "cannot open" << path;
        return 1;
    }

    QTextStream out(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    out.setCodec("UTF-8");            // the Qt 5 call this migration replaced
#else
    out.setEncoding(QStringConverter::Utf8);   // the Qt 6 replacement
#endif
    out << payload;
    out.flush();
    file.close();

    QFile back(path);
    back.open(QIODevice::ReadOnly);
    const QByteArray bytes = back.readAll();
    const QByteArray digest =
        QCryptographicHash::hash(bytes, QCryptographicHash::Sha256).toHex();

    QTextStream(stdout) << "qt      " << QT_VERSION_STR << "\n"
                        << "bytes   " << bytes.size() << "\n"
                        << "sha256  " << digest << "\n";
    return 0;
}
