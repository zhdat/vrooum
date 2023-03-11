#include "logger.h"
#include <QDebug>
#include <QDir>
#include <QStringList>

std::unique_ptr<QFile> Logger::_file;

namespace
{
QString uniqueLogFileName()
{
  int nextIndex = 0;
  QStringList logFiles = QDir::current().entryList(QStringList() << "grand_prix_log*.csv");
  for (const QString & name : logFiles) {
    QRegExp re("grand_prix_log([0-9]+).csv");
    if (re.indexIn(name) == 0) {
      int index = re.cap(1).toInt();
      if (index >= nextIndex) {
        nextIndex = index + 1;
      }
    }
  }
  return QString("grand_prix_log%1.csv").arg(nextIndex, 3, 10, QChar('0'));
}

} // namespace

bool Logger::_isEnabled = false;

void Logger::setEnabled()
{
  _isEnabled = true;
}

void Logger::open()
{
  if (_file) {
    _file->close();
    _file.reset();
  }
  _file = std::unique_ptr<QFile>(new QFile(uniqueLogFileName()));
  Q_ASSERT_X(_file->open(QFile::WriteOnly | QFile::Append), __FUNCTION__, "Cannot open file");
}

void Logger::write(QString line)
{
  if (_file->isOpen()) {
    _file->write(line.toUtf8());
  } else {
    qWarning() << __FUNCTION__ << " Cannot write. File not open.";
  }
}

bool Logger::enabled()
{
  return _isEnabled;
}

void Logger::close()
{
  if (_file) {
    _file->flush();
    _file->close();
    _file.reset();
  }
}
