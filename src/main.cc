/*=====================================================================

QGroundControl Open Source Ground Control Station

(c) 2009 - 2011 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>

This file is part of the QGROUNDCONTROL project

    QGROUNDCONTROL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QGROUNDCONTROL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Main executable
 *   @author Lorenz Meier <mavteam@student.ethz.ch>
 *
 */

#include "QGCCore.h"
#include "MainWindow.h"
#include "configuration.h"
#include "QsLog.h"
#include <QtGui/QApplication>

/* SDL does ugly things to main() */
#ifdef main
#undef main
#endif


// Install a message handler so you do not need
// the MSFT debug tools installed to se
// qDebug(), qWarning(), qCritical and qAbort
#ifdef Q_OS_WIN
void msgHandler( QtMsgType type, const char* msg )
{
    const char symbols[] = { 'I', 'E', '!', 'X' };
    QString output = QString("[%1] %2").arg( symbols[type] ).arg( msg );
    std::cerr << output.toStdString() << std::endl;
    if( type == QtFatalMsg ) abort();
}

#endif

/**
 * @brief Starts the application
 *
 * @param argc Number of commandline arguments
 * @param argv Commandline arguments
 * @return exit code, 0 for normal exit and !=0 for error cases
 */

int main(int argc, char *argv[])
{
// install the message handler
#ifdef Q_OS_WIN
    qInstallMsgHandler( msgHandler );
#endif

    QGCCore core(argc, argv);
    // init the logging mechanism
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::DebugLevel);

    const QString sLogPath(QDir(QGC::appDataDirectory()).filePath("log.txt"));

    QsLogging::DestinationPtr fileDestination(
       QsLogging::DestinationFactory::MakeFileDestination(sLogPath, true, 0, 5) );
    QsLogging::DestinationPtr debugDestination(
       QsLogging::DestinationFactory::MakeDebugOutputDestination() );
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);

    // This is required to start the logger
    core.initialize();

    return core.exec();
}
