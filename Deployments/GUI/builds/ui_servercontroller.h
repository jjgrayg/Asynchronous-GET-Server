/********************************************************************************
** Form generated from reading UI file 'servercontroller.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERCONTROLLER_H
#define UI_SERVERCONTROLLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerController
{
public:
    QWidget *MainWidget;
    QGridLayout *gridLayout;
    QFrame *frame_8;
    QGridLayout *gridLayout_3;
    QPushButton *ServerControl;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QFrame *frame_11;
    QFrame *frame_10;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *PortNumContainer;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout;
    QLabel *PortNumLabel;
    QLineEdit *PortNumInput;
    QFrame *frame_4;
    QHBoxLayout *LoggingContainer;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_2;
    QLabel *LoggingLabel;
    QCheckBox *LoggingInput;
    QFrame *frame_3;
    QHBoxLayout *DebuggingContainer;
    QFrame *frame_7;
    QHBoxLayout *horizontalLayout_3;
    QLabel *DebuggingLabel;
    QCheckBox *DebuggingInput;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QTextEdit *ConsoleOutputBroswer;
    QSpacerItem *horizontalSpacer;
    QFrame *frame_9;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ServerController)
    {
        if (ServerController->objectName().isEmpty())
            ServerController->setObjectName(QString::fromUtf8("ServerController"));
        ServerController->resize(853, 750);
        ServerController->setLayoutDirection(Qt::LeftToRight);
        MainWidget = new QWidget(ServerController);
        MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QGridLayout(MainWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        frame_8 = new QFrame(MainWidget);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_8);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        ServerControl = new QPushButton(frame_8);
        ServerControl->setObjectName(QString::fromUtf8("ServerControl"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(10);
        sizePolicy.setHeightForWidth(ServerControl->sizePolicy().hasHeightForWidth());
        ServerControl->setSizePolicy(sizePolicy);
        ServerControl->setMinimumSize(QSize(200, 200));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe Fluent Icons")});
        ServerControl->setFont(font);

        gridLayout_3->addWidget(ServerControl, 0, 0, 1, 1);

        frame = new QFrame(frame_8);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMinimumSize(QSize(0, 340));
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Sunken);
        frame->setLineWidth(15);
        frame->setMidLineWidth(7);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);
        label_3->setFrameShape(QFrame::WinPanel);
        label_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(label_3);

        frame_11 = new QFrame(frame);
        frame_11->setObjectName(QString::fromUtf8("frame_11"));
        frame_11->setFrameShape(QFrame::HLine);
        frame_11->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frame_11);

        frame_10 = new QFrame(frame);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setFrameShape(QFrame::WinPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_10);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        PortNumContainer = new QHBoxLayout();
        PortNumContainer->setObjectName(QString::fromUtf8("PortNumContainer"));
        PortNumContainer->setContentsMargins(5, 5, 5, 5);
        frame_5 = new QFrame(frame_10);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy2);
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        PortNumLabel = new QLabel(frame_5);
        PortNumLabel->setObjectName(QString::fromUtf8("PortNumLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(PortNumLabel->sizePolicy().hasHeightForWidth());
        PortNumLabel->setSizePolicy(sizePolicy3);
        PortNumLabel->setMinimumSize(QSize(100, 0));
        PortNumLabel->setFont(font);

        horizontalLayout->addWidget(PortNumLabel);

        PortNumInput = new QLineEdit(frame_5);
        PortNumInput->setObjectName(QString::fromUtf8("PortNumInput"));
        PortNumInput->setEnabled(true);
        PortNumInput->setFont(font);

        horizontalLayout->addWidget(PortNumInput);


        PortNumContainer->addWidget(frame_5);


        verticalLayout_2->addLayout(PortNumContainer);

        frame_4 = new QFrame(frame_10);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::HLine);
        frame_4->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(frame_4);

        LoggingContainer = new QHBoxLayout();
        LoggingContainer->setObjectName(QString::fromUtf8("LoggingContainer"));
        LoggingContainer->setContentsMargins(5, 5, 5, 5);
        frame_6 = new QFrame(frame_10);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        LoggingLabel = new QLabel(frame_6);
        LoggingLabel->setObjectName(QString::fromUtf8("LoggingLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(LoggingLabel->sizePolicy().hasHeightForWidth());
        LoggingLabel->setSizePolicy(sizePolicy4);
        LoggingLabel->setMinimumSize(QSize(100, 0));
        LoggingLabel->setFont(font);

        horizontalLayout_2->addWidget(LoggingLabel);

        LoggingInput = new QCheckBox(frame_6);
        LoggingInput->setObjectName(QString::fromUtf8("LoggingInput"));
        LoggingInput->setFont(font);

        horizontalLayout_2->addWidget(LoggingInput);


        LoggingContainer->addWidget(frame_6);


        verticalLayout_2->addLayout(LoggingContainer);

        frame_3 = new QFrame(frame_10);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::HLine);
        frame_3->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(frame_3);

        DebuggingContainer = new QHBoxLayout();
        DebuggingContainer->setObjectName(QString::fromUtf8("DebuggingContainer"));
        DebuggingContainer->setContentsMargins(5, 5, 5, 5);
        frame_7 = new QFrame(frame_10);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_7);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        DebuggingLabel = new QLabel(frame_7);
        DebuggingLabel->setObjectName(QString::fromUtf8("DebuggingLabel"));
        sizePolicy4.setHeightForWidth(DebuggingLabel->sizePolicy().hasHeightForWidth());
        DebuggingLabel->setSizePolicy(sizePolicy4);
        DebuggingLabel->setMinimumSize(QSize(100, 0));
        DebuggingLabel->setFont(font);

        horizontalLayout_3->addWidget(DebuggingLabel);

        DebuggingInput = new QCheckBox(frame_7);
        DebuggingInput->setObjectName(QString::fromUtf8("DebuggingInput"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(DebuggingInput->sizePolicy().hasHeightForWidth());
        DebuggingInput->setSizePolicy(sizePolicy5);
        DebuggingInput->setFont(font);

        horizontalLayout_3->addWidget(DebuggingInput);


        DebuggingContainer->addWidget(frame_7);


        verticalLayout_2->addLayout(DebuggingContainer);


        verticalLayout->addWidget(frame_10);


        gridLayout_3->addWidget(frame, 0, 2, 1, 1);

        frame_2 = new QFrame(frame_8);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFont(font);
        frame_2->setFrameShape(QFrame::WinPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        ConsoleOutputBroswer = new QTextEdit(frame_2);
        ConsoleOutputBroswer->setObjectName(QString::fromUtf8("ConsoleOutputBroswer"));
        ConsoleOutputBroswer->setFont(font);
        ConsoleOutputBroswer->setLayoutDirection(Qt::RightToLeft);
        ConsoleOutputBroswer->setFrameShape(QFrame::Box);
        ConsoleOutputBroswer->setFrameShadow(QFrame::Plain);
        ConsoleOutputBroswer->setLineWidth(1);
        ConsoleOutputBroswer->setMidLineWidth(0);
        ConsoleOutputBroswer->setAutoFormatting(QTextEdit::AutoBulletList);

        gridLayout_2->addWidget(ConsoleOutputBroswer, 0, 0, 1, 1);


        gridLayout_3->addWidget(frame_2, 1, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 1, 1, 1);


        gridLayout->addWidget(frame_8, 2, 2, 1, 1);

        frame_9 = new QFrame(MainWidget);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_9);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_2 = new QLabel(frame_9);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setMinimumSize(QSize(250, 0));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/icon.png")));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_2);

        label = new QLabel(frame_9);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);
        label->setTextFormat(Qt::MarkdownText);

        horizontalLayout_5->addWidget(label);


        gridLayout->addWidget(frame_9, 0, 2, 1, 1);

        ServerController->setCentralWidget(MainWidget);
        menubar = new QMenuBar(ServerController);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 853, 25));
        ServerController->setMenuBar(menubar);
        statusbar = new QStatusBar(ServerController);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ServerController->setStatusBar(statusbar);

        retranslateUi(ServerController);

        QMetaObject::connectSlotsByName(ServerController);
    } // setupUi

    void retranslateUi(QMainWindow *ServerController)
    {
        ServerController->setWindowTitle(QCoreApplication::translate("ServerController", "Asynchronous GET Server", nullptr));
        ServerControl->setText(QCoreApplication::translate("ServerController", "Start", nullptr));
        label_3->setText(QCoreApplication::translate("ServerController", "<html><head/><body><p align=\"center\"><span style=\" font-size:11pt; text-decoration: underline; color:#a9a9a9;\">Options</span></p><p align=\"center\"><span style=\" font-size:8pt; color:rgb(255, 97, 97);\">Check before starting</span></p></body></html>", nullptr));
        PortNumLabel->setText(QCoreApplication::translate("ServerController", "Port#:", nullptr));
        PortNumInput->setInputMask(QCoreApplication::translate("ServerController", "90000", nullptr));
        PortNumInput->setText(QCoreApplication::translate("ServerController", "8080", nullptr));
        LoggingLabel->setText(QCoreApplication::translate("ServerController", "Logging:", nullptr));
        LoggingInput->setText(QString());
        DebuggingLabel->setText(QCoreApplication::translate("ServerController", "Debugging:", nullptr));
        DebuggingInput->setText(QString());
        ConsoleOutputBroswer->setMarkdown(QString());
        ConsoleOutputBroswer->setHtml(QCoreApplication::translate("ServerController", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Segoe Fluent Icons'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Segoe UI';\"><br /></p></body></html>", nullptr));
        ConsoleOutputBroswer->setPlaceholderText(QString());
        label_2->setText(QString());
        label->setText(QCoreApplication::translate("ServerController", "<html><head/><body><p><span style=\" font-size:22pt;\">&emsp;&emsp;GETServer V</span><span style=\" font-size:22pt; vertical-align:super;\">2.0</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerController: public Ui_ServerController {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERCONTROLLER_H
