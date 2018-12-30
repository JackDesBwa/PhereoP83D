#include "toolbox.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace {
QString picturesPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
QString picturesPathFinal = picturesPath + QDir::separator() + "PhereoRoll3D";
}

Toolbox::Toolbox(QNetworkAccessManager & nam, QObject *parent) : QObject(parent), m_nam(nam) {
    regexpURL = QRegularExpression("(https?://([^\\s])+)");
    regexpURL.optimize();
}

bool Toolbox::hasWritePermissions() {
    QString testPath = picturesPathFinal;
    if (!QFile(testPath).exists())
        testPath = picturesPath;
    QFileInfo picturesPathInfo(testPath);
    return (picturesPathInfo.isDir() && picturesPathInfo.isWritable());
}

QString Toolbox::md5(QString txt) {
    return QCryptographicHash::hash(txt.toUtf8(), QCryptographicHash::Md5).toHex();
}

QString Toolbox::reformatText(QString txt) {
    txt.replace(regexpURL, "<a href=\"\\1\" style=\"color: white;\">\\1</a>");
    txt.replace("\n", "<br>");
    return txt;
}

void Toolbox::download(QString imgurl, QString imgid) {
    QNetworkRequest request;
    request.setUrl(QUrl(imgurl));

    QNetworkReply *reply = m_nam.get(request);
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, imgid](){
        QDir().mkpath(picturesPathFinal);
        QFile f(picturesPathFinal + QDir::separator() + imgid + "_3d_sbs.jpg");
        if (f.open(QFile::WriteOnly)) {
            f.write(reply->readAll());
        }
        reply->deleteLater();
    });
    QObject::connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), reply, &QNetworkReply::deleteLater);
    QObject::connect(reply, &QNetworkReply::sslErrors, reply, &QNetworkReply::deleteLater);
}
