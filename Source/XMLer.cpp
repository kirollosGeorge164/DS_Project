#include "XMLer.h"
#include <bits/stdc++.h>
#include <QLabel>
#include <QProcess>
#include <QDir>
#include <QCoreApplication>
#include <fstream>
#include "QPixmap"
#include <QApplication>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

QStack<QString> stack_undo;
QStack<QString> stack_redo;

QString Current;

XMLer::XMLer(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *row1Layout = new QHBoxLayout();
    QHBoxLayout *row2Layout = new QHBoxLayout();
    QHBoxLayout *row3Layout = new QHBoxLayout();
    QVBoxLayout *col1layout = new QVBoxLayout(this);
    QVBoxLayout *col2layout = new QVBoxLayout(this);

    fileLineEdit = new QLineEdit(this);
    fileLineEdit->setPlaceholderText("Select an XML file...");
    QString windowStyle = "color: white;"
                          "border: none ;";
    fileLineEdit->setStyleSheet(windowStyle);

    browseButton = new QPushButton("Browse", this);
    compressButton = new QPushButton("Compress", this);
    decompressButton = new QPushButton("Decompress", this);
    minifyingButton = new QPushButton("Minify", this);
    checkButton = new QPushButton("Check", this);
    findButton = new QPushButton("Find Error", this);
    formatButton = new QPushButton("Prettify", this);
    JSON = new QPushButton("Convert to JSON", this);
    showSocialNetwork = new QPushButton("Show Social Network ", this);
    saveAsXml = new QPushButton("Save As XML", this);
    undoButton = new QPushButton("Undo", this);
    redoButton = new QPushButton("Redo", this);
    correctButton = new QPushButton("Correct Error", this);

    suggestFollowersButton = new QPushButton("Suggest Followers", this);
    findMutualButton = new QPushButton("Find Mutual", this);
    grapInfoButton = new QPushButton("Get Info", this);
    postSearchButton = new QPushButton("Search for Posts", this);

    resultTextEdit = new QTextEdit(this);
    QString TextEditor = "color: white;"
                         "border:1px solid white ;";
    resultTextEdit->setStyleSheet(TextEditor);
    resultTextEdit->setReadOnly(false);

    row1Layout->addWidget(browseButton);
    row1Layout->addWidget(undoButton);
    row1Layout->addWidget(redoButton);
    row1Layout->addWidget(saveAsXml);
    layout->addLayout(row1Layout);
    layout->addWidget(fileLineEdit);

    col1layout->addWidget(compressButton);
    col1layout->addWidget(decompressButton);
    col1layout->addWidget(minifyingButton);
    col1layout->addWidget(formatButton);

    /* STYLE */
    QString buttonStyle = "QPushButton {"
                          "background-color: orange;"
                          "color: black;"
                          "}";

    // Setting the palette for each button
    browseButton->setStyleSheet(buttonStyle);
    compressButton->setStyleSheet(buttonStyle);
    decompressButton->setStyleSheet(buttonStyle);
    minifyingButton->setStyleSheet(buttonStyle);
    checkButton->setStyleSheet(buttonStyle);
    findButton->setStyleSheet(buttonStyle);
    formatButton->setStyleSheet(buttonStyle);
    JSON->setStyleSheet(buttonStyle);
    showSocialNetwork->setStyleSheet(buttonStyle);
    saveAsXml->setStyleSheet(buttonStyle);
    undoButton->setStyleSheet(buttonStyle);
    redoButton->setStyleSheet(buttonStyle);
    correctButton->setStyleSheet(buttonStyle);
    suggestFollowersButton->setStyleSheet(buttonStyle);
    findMutualButton->setStyleSheet(buttonStyle);
    grapInfoButton->setStyleSheet(buttonStyle);
    postSearchButton->setStyleSheet(buttonStyle);
    /*********/
    col1layout->addWidget(JSON);
    col1layout->addWidget(checkButton);
    col1layout->addWidget(findButton);
    col1layout->addWidget(correctButton);

    row2Layout->addLayout(col1layout);
    row2Layout->addWidget(resultTextEdit);
    row2Layout->addLayout(col2layout);
    // layout->addWidget(browseButton);
    layout->addLayout(row2Layout);

    row3Layout->addWidget(showSocialNetwork);
    row3Layout->addWidget(suggestFollowersButton);
    row3Layout->addWidget(findMutualButton);
    row3Layout->addWidget(grapInfoButton);
    row3Layout->addWidget(postSearchButton);
    layout->addLayout(row3Layout);

    // layout->addWidget(postSearchButton);

    connect(browseButton, &QPushButton::clicked, this, &XMLer::browseFile);
    connect(compressButton, &QPushButton::clicked, this, &XMLer::compressXml);
    connect(decompressButton, &QPushButton::clicked, this, &XMLer::decompressXml);
    connect(minifyingButton, &QPushButton::clicked, this, &XMLer::minifyXml);
    connect(checkButton, &QPushButton::clicked, this, &XMLer::checkXml);
    connect(findButton, &QPushButton::clicked, this, &XMLer::findErrorXml);
    connect(formatButton, &QPushButton::clicked, this, &XMLer::formatXml);
    connect(JSON, &QPushButton::clicked, this, &XMLer::ToJSON);
    connect(showSocialNetwork, &QPushButton::clicked, this, &XMLer::drawGraph);
    connect(saveAsXml, &QPushButton::clicked, this, &XMLer::saveXml);
    connect(undoButton, &QPushButton::clicked, this, &XMLer::undo);
    connect(redoButton, &QPushButton::clicked, this, &XMLer::redo);
    connect(correctButton, &QPushButton::clicked, this, &XMLer::correctXml);
    connect(suggestFollowersButton, &QPushButton::clicked, this, &XMLer::suggestFollowers);
    connect(findMutualButton, &QPushButton::clicked, this, &XMLer::findMutual);
    connect(grapInfoButton, &QPushButton::clicked, this, &XMLer::grapInfo);
    connect(postSearchButton, &QPushButton::clicked, this, &XMLer::postSearch);
}

void XMLer::browseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open XML File", "", "XML Files (*.xml)");
    stack_undo.push(resultTextEdit->toPlainText());
    if (!filePath.isEmpty())
    {
        fileLineEdit->setText(filePath);
        // Read and store the original XML content
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        originalXmlContent = file.readAll();

        // Set Current
        Current = originalXmlContent;

        file.close();
        // Display compressed XML in QTextEdit
        resultTextEdit->setPlainText(originalXmlContent);
    }
}

void XMLer::saveXml()
{
    QString textContent = resultTextEdit->toPlainText();
    if (!textContent.isEmpty())
    {
        QString filter = "All File (.) ;; Text File (.txt) ;; XML File (.xml)";
        // Open file dialog to get the save path from the user
        QString filePath = QFileDialog::getSaveFileName(this, tr("Save XML File"), "", tr("XML Files (*.xml)"));
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Could not open the file for writing";
            return;
        }

        // Write the text content to the file
        QTextStream out(&file);
        out << textContent;

        file.close();
    }
    else
    {
        resultTextEdit->setPlainText("Please select an XML file.");
    }
}

void XMLer::minifyXml(){

    stack_undo.push(resultTextEdit->toPlainText());
    if (!resultTextEdit->toPlainText().isEmpty()) {


        // Get the text from the textEdit object
        QString originalText = resultTextEdit->toPlainText();
        QString minified = "";

        bool inTag = false;

        for (int32_t i = 0; i < originalText.size(); i++) {
            if (originalText[i] == '<') {
                inTag = true;
            }

            if (!inTag && isWhitespace(originalText[i])) {
                // Skip whitespace before content within tags
                while ( isWhitespace(originalText[i+1])) {
                    i++;
                }
                if(originalText[i+1]=='<')
                    i++;

            }

            if (originalText[i] == '>') {
                inTag = false;
            }

            minified += originalText[i];
        }
        resultTextEdit->setPlainText(minified);
    } else {
        resultTextEdit->setPlainText("Please select an XML file.");
    }
}

/****************************** Private Functions **********************************/
