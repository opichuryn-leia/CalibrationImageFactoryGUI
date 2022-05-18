#include <QtCore>
#include <QFile>
#include <QDebug>
#include <QProcessEnvironment>

QFileInfo findFile(const QStringList& dirs, const QString& file) {
    QFileInfo info {file};
    if (info.exists()) {
        return info;
    }

    for (const auto& dirPath : dirs) {
        QDir dir {dirPath};
        auto list = dir.entryInfoList({file}, QDir::Files);
        if (!list.empty()) {
            return list.first();
        }
    }
    return {};
}

QStringList readDependecyFile(const QString& filePath) {
    QFile f(filePath);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "Error! Cannot open " << filePath;
    }
    QTextStream in(&f);
    return in.readAll().split("\n", Qt::SkipEmptyParts);
}

// file arg dependecies file, second - destination folder
int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);

    if (argc != 3) {
        qWarning() << "   Usage: copy_deps <deps_file> <dest_folder>";
        return 1;
    }

    const QStringList dirs = QProcessEnvironment::systemEnvironment().value("PATH").split(';', Qt::SkipEmptyParts);
    const QStringList files = readDependecyFile(argv[1]);
    const QString dest = argv[2];

    bool success = true;
    for (const auto& file : qAsConst(files)) {
        QFileInfo info = findFile(dirs, file);
        if (info.exists()) {
            QString deployFilePath = QDir{dest}.absoluteFilePath(info.fileName());
            QFile::remove(deployFilePath);
            const bool copied = QFile::copy(info.absoluteFilePath(), deployFilePath);
            qDebug() << "FOUND " << file << " -> " << info.absoluteFilePath() << " COPIED " << copied;
            success &= copied;
        } else {
            qDebug() << ">>>>>>>>>>>> NOT FOUND " << file;
            success = false;
        }
    }
    return success ? 0: 1;
}
