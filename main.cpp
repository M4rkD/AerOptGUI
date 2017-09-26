/*********************************************
**
**	Created on: 	28/12/2013 2013
**	Author: 	matt - Matt Edmunds
**	File:		main.cpp
**  This is the English version of this programme
**
**********************************************/

#include <iostream>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QSettings>

#include "DebugOutput.h"
#include "MainWindow.h"
#include "OptimisationModel.h"

void settings()
{
    //Setup up default settings
    QCoreApplication::setOrganizationName("Swansea University (Engineering)");
    QCoreApplication::setOrganizationDomain("engineering.swansea.ac.uk");
    QCoreApplication::setApplicationName("AerOpt");

    QSettings settings;

    // set up paths
    QString appPath = QDir::fromNativeSeparators(QCoreApplication::applicationFilePath());

    int index = appPath.lastIndexOf("/AerOpt/");

    QString aeroptExe;
    QString mesherExe;
    if (index != -1)
    {
        //Determin app path
        appPath.remove(index, appPath.size() - index);
        appPath = QDir::toNativeSeparators(appPath);
        qDebug() << "App Path:" << appPath;

        //Determin mesher path
        mesherExe = QDir::toNativeSeparators(appPath + "/AerOpt/FLITE/Mesher/MeshGenerator");
        settings.setValue("mesher/exe", mesherExe);
        qDebug() << "Mesher Exe Path" << mesherExe;

        //Determin flite path << lol
        aeroptExe = QDir::toNativeSeparators(appPath + "/AerOpt/FortranFiles/AeroOpt/AerOpt");
        settings.setValue("AerOpt/executable", aeroptExe);
        qDebug() << "AerOpt Exe Path: " << aeroptExe;

        bool c = true;
        QFileInfo checkMesher(mesherExe);
        QFileInfo checkAerOpt(aeroptExe);
        c &= checkMesher.exists();
        c &= checkMesher.isFile();
        c &= checkAerOpt.exists();
        c &= checkAerOpt.isFile();

        if (!c)
        {
            qCritical() << "Application executables not available!";
        }
    } else {
            qCritical() << "Application path could not be determined!";
    }

    settings.beginWriteArray("profiles");
    settings.setArrayIndex(0);
    settings.setValue("filepath","/Volumes/HardDrive/Users/mark/AerOpt/AerOpt/Example_profile_files/NACA0024.prf");
    settings.setArrayIndex(1);
    settings.setValue("filepath","/Volumes/HardDrive/Users/mark/AerOpt/AerOpt/Example_profile_files/NACA21120.prf");
    settings.endArray();

    settings.setValue("mesher/workdir","/Volumes/HardDrive/Users/mark/AerOpt/AerOpt/Project");

    QString AerOptWorkDir = appPath;
    AerOptWorkDir += "/AerOpt/FLITE/";
    AerOptWorkDir = QDir::toNativeSeparators(AerOptWorkDir);
    settings.setValue("AerOpt/workdir", AerOptWorkDir);

    QString AerOptInFile = AerOptWorkDir;
    AerOptInFile += "Input_Data/AerOpt_InputParameters.txt";
    AerOptInFile = QDir::toNativeSeparators(AerOptInFile);
    settings.setValue("AerOpt/inputFile", AerOptInFile);

    QString AerOptNodeFile = AerOptWorkDir;
    AerOptNodeFile += "Input_Data/Control_Nodes.txt";
    AerOptNodeFile = QDir::toNativeSeparators(AerOptNodeFile);
    settings.setValue("AerOpt/nodeFile", AerOptNodeFile);

    QString projectDir = appPath + "/AerOpt/Scratch";
    projectDir = QDir::toNativeSeparators(projectDir);
    QDir projectDirObj = QDir(projectDir);
    if(!projectDirObj.exists()) {
        projectDirObj.mkpath(".");
    }
    settings.setValue("mesher/scratchDir", projectDir);

    QString meshDatFile;
    meshDatFile = QDir(projectDirObj.absolutePath() + "/scratch.dat").absolutePath();
    meshDatFile = QDir::toNativeSeparators(meshDatFile);
    settings.setValue("mesher/datFile", meshDatFile);

    QString rootDir = QDir::toNativeSeparators(appPath + "/AerOpt/FLITE");
    settings.setValue("AerOpt/rootDir", rootDir);

    QString inFolder = QDir::toNativeSeparators(rootDir + "/Input_Data");
    QDir().mkdir(inFolder);
    settings.setValue("AerOpt/inFolder", inFolder);

    QString outFolder = QDir::toNativeSeparators(rootDir + "/Output_Data");
    QDir().mkdir(outFolder);
    settings.setValue("AerOpt/outFolder", outFolder);
}

//Programme entry point.
int main(int argc, char *argv[])
{
    //Initialises the QT library application event loops.
    QApplication app(argc, argv);
    app.setWindowIcon( QIcon(":/images/AerOpt.png") );
    //app.setAttribute(Qt::AA_DontUseNativeMenuBar);
    settings();

    //Application main interaction classes.
    DebugOutput::Instance();
    qInfo() << " *** Welcome to AerOpt ***";
    qInfo() << " ***     Have a nice day     ***";

    // Setup optimisation model and selection model
    OptimisationModel* optimisationModel = new OptimisationModel();
    QItemSelectionModel* optimisationSelectionModel = new QItemSelectionModel(optimisationModel);

    //Main window setup and show.
    MainWindow w;
    w.setWindowTitle("AerOpt");
    w.setOptimisationModel(optimisationModel);
    w.show();

    return app.exec();
}
