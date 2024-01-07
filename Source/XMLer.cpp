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

    // Setting the stylesheet
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
    layout->addLayout(row2Layout);

    row3Layout->addWidget(showSocialNetwork);
    row3Layout->addWidget(suggestFollowersButton);
    row3Layout->addWidget(findMutualButton);
    row3Layout->addWidget(grapInfoButton);
    row3Layout->addWidget(postSearchButton);
    layout->addLayout(row3Layout);


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

/****************************** Functions Connected to Buttons **************************************/
/* Level 1 */
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

void XMLer::formatXml(){
    stack_undo.push(resultTextEdit->toPlainText());
    if (!resultTextEdit->toPlainText().isEmpty()) {
        QString xmlContent = resultTextEdit->toPlainText();

        xmlContent=xmlContent.replace("\n", "");

        QString previousTag="";
        QString currentTag="";
        int length=xmlContent.length();
        for(int i=0;i<length;i++)
        {

            if(xmlContent[i]=='<')
            {
                int startTag = i + 1;
                int endTag = xmlContent.indexOf('>', startTag);

                QString currentTag = xmlContent.mid(startTag, endTag - startTag);
                QString testTag=currentTag;

                if(currentTag == "id" || currentTag == "name")
                {


                    int index=xmlContent.indexOf('>',xmlContent.indexOf('>',i+1)+1);
                    xmlContent.insert(index+1,"\n");
                    length=length+1;
                    i=index;
                    previousTag=currentTag;
                    continue;
                }
                if(previousTag==(testTag.remove(0,1)))
                {
                    //qDebug() << "yarab";
                    xmlContent.insert(i,"\n");
                    length=length+1;
                }
                int Index=xmlContent.indexOf('>',i);
                xmlContent.insert(Index+1,"\n");
                length=length+1;
                previousTag=currentTag;


            }
        }

        QStringList list =xmlContent.split(u'\n',Qt::SkipEmptyParts);

        QStack<int> indent;
        QString line;
        indent.push(0);
        QString oldline;
        QString newline;
        QString formatted;
        for(int i=0;i<list.length();i++)
        {
            line=list[i];
            for(QChar character : line)
            {
                if(character !=' ')
                    break;
                line.remove(0,1);
            }

            if(oldline.contains("<") && !oldline.contains("/"))
                indent.push((indent.top())+4);
            if(line.contains("/") && line.count('<')==1)
                indent.pop();
            for(int k=0;k<indent.top();k++)
                newline.append(" ");
            newline.append(line);
            formatted.append(newline+"\n");
            oldline=newline;
            newline.clear();
        }
        if (formatted.endsWith('\n')) {
            // Remove the last character (newline)
            formatted.chop(1);
        }

        resultTextEdit->setPlainText(formatted);
    } else {
        resultTextEdit->setPlainText("Please select an XML file.");
    }

}


void XMLer::ToJSON(){
    if (!resultTextEdit->toPlainText().isEmpty()) {
        stack_undo.push(resultTextEdit->toPlainText());
        // Open the XML file
        QString XML;
        QString JSON;
        // Read and store the original XML content
        QString textContent=resultTextEdit->toPlainText();
        QTextStream in(&textContent);
        XML = in.readAll();
        QString Block;

        JSON = JSON + "{\n";

        int y = 1;

        for (int x = 0; x < XML.length(); x++) {
            if (XML[x] == '<') {
                while (XML[x + y] != '>') {
                    Block = Block + XML[x + y];
                    y++;
                }
                y = 1;

                if (XML[x + 1] != '/') {
                    qDebug() << "\"";
                    JSON = JSON + "\"";
                } else {
                    x = x + 1 + Block.length();
                    close(Block,&JSON);
                    Block.clear();
                }

            } else if (XML[x] == '>') {
                qDebug() << "\"";
                JSON = JSON + "\"";

                open(Block,&JSON);
                Block.clear();

            } else {
                qDebug() << XML[x];
                JSON = JSON + XML[x];
            }
        }
        JSON += "\n}";
        resultTextEdit->setPlainText(JSON);

    } else {
        resultTextEdit->setPlainText("Please select an XML file.");
    }

}


void XMLer::undo(void) {

    if (!stack_undo.empty())
    {
        stack_redo.push(resultTextEdit->toPlainText());
        resultTextEdit->setPlainText(stack_undo.top());
        stack_undo.pop();
    }

}

void XMLer::redo(void) {
    if (!stack_redo.empty())
    {
        stack_undo.push(resultTextEdit->toPlainText());
        resultTextEdit->setPlainText(stack_redo.top());
        stack_redo.pop();
    }

}

void XMLer::checkXml() {
    //QString filePath = fileLineEdit->text();
    if (!resultTextEdit->toPlainText().isEmpty()) {
        QString textContent=resultTextEdit->toPlainText();
        QVector<QString> xmlLines;
        QTextStream in(&textContent);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            xmlLines.push_back(line);
        }
        // Testing
        qDebug() << xmlLines[0];



        if (checkConsistency(xmlLines))
        {
            QMessageBox::information(nullptr, "Ok", "The XML is consistent <3 ");
        }
        else
        {
            QMessageBox::warning(nullptr, "Warning", "The XML has errors :( ");
        }

    } else {
        resultTextEdit->setPlainText("Please select an XML file.");
    }
}

void XMLer::findErrorXml(){
    if (!resultTextEdit->toPlainText().isEmpty()) {
        QString textContent=resultTextEdit->toPlainText();

        QVector<QString> xmlLines;
        QTextStream in(&textContent);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            xmlLines.push_back(line);
        }

        QVector<ErrorData> errorVector = findErrors(xmlLines);

        QString errorDetails = displayErrors(QVector<ErrorData>(errorVector.begin(), errorVector.end()));


        if (errorDetails.isEmpty()) {
            QMessageBox::information(nullptr, "Ok", "There are no errors <3 ");
        } else {
            QMessageBox::warning(nullptr, "Warning", "There are errors :( \n\n" + errorDetails);
        }


    } else {
        resultTextEdit->setPlainText("Please select an XML file.");
    }


}

void XMLer::correctXml()
{
    QString textContent=resultTextEdit->toPlainText();
    QVector<QString> xmlLines;
    QTextStream in(&textContent);

    while (!in.atEnd()) {
        QString line = in.readLine();
        xmlLines.push_back(line);
    }

    // Find errors in the XML file
    QVector<ErrorData> errorVector = findErrors(xmlLines);

    if (!errorVector.isEmpty()) {
        // Correct errors in the XML file
        QStringList correctedLines = error_corrector(xmlLines, errorVector);

        // Join the QStringList elements into a single QString
        //for texteditor
        QString correctedText = correctedLines.join("\n");
        stack_undo.push(resultTextEdit->toPlainText());
        resultTextEdit->setPlainText(correctedText);

        QMessageBox::information(nullptr, "Success", "Errors corrected and saved successfully.");
    } else {
        QMessageBox::information(nullptr, "No Errors", "There are no errors to correct.");
    }

}

/* Level 2 */
void XMLer::drawGraph()
{
    if (!resultTextEdit->toPlainText().isEmpty())
    {
        // Read XML content from the file
        QString textContent = resultTextEdit->toPlainText();
        QVector<QString> xmlLines;
        QTextStream in(&textContent);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            xmlLines.push_back(line);
        }

        QVector<Vertex> parsed;
        parsed = xmlParse(xmlLines);

        // For Testing
        // Debug output to check if 'parsed' is non-empty
        // qDebug() << "Number of vertices in parsed: " << parsed.size();

        // if (!parsed.isEmpty())
        // {
        //     for (int i = 0; i < parsed.size(); ++i)
        //     {
        //         // Debug output to check the content of the first vertex
        //         qDebug() << "User Name: " << parsed[i].user_name;
        //         qDebug() << "User ID: " << parsed[i].user_id;
        //         qDebug() << "Followers IDs: " << parsed[i].followers_id;
        //     }
        // }
        createGraphVisualization(parsed);
    }
    else
    {
        resultTextEdit->setPlainText("Please select an XML file.");
    }
}

void XMLer::findMutual() //take the id of users from two text editor
{

    QString user1="";
    QString user2="";

    int user1Index=-1;
    int user2Index=-1;

    QVector<QString> mutualFriends;
    QVector<QString> mutualNames;

    if (!resultTextEdit->toPlainText().isEmpty()) {
        QString textContent=resultTextEdit->toPlainText();
        QVector<QString> xmlLines;
        QTextStream in(&textContent);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            xmlLines.push_back(line);
        }

        QVector<Vertex> parsed;
        parsed = xmlParse(xmlLines);

        QDialog inputDialog;
        inputDialog.setWindowTitle("Write their ID's");

        QFormLayout formLayout(&inputDialog);

        QLineEdit input1Edit;
        QLineEdit input2Edit;
        formLayout.addRow("User 1 ID:", &input1Edit);
        formLayout.addRow("User 2 ID:", &input2Edit);

        QPushButton okButton("find");
        formLayout.addRow(&okButton);
        QPushButton okayButton("ok");
        formLayout.addRow(&okayButton);
        QObject::connect(&okayButton, &QPushButton::clicked, [&]() {
            inputDialog.accept();
        });
        QObject::connect(&okButton, &QPushButton::clicked, [&]() {
            QString user1 = input1Edit.text();
            QString user2 = input2Edit.text();
            mutualFriends.clear();
            mutualNames.clear();
         
            for(int i=0;i<parsed.size();i++)
            {
                if(user1== parsed[i].user_id)
                    user1Index=i;

                if(user2== parsed[i].user_id)
                    user2Index=i;

            }

            for (int i = 0; i < parsed[user1Index].followers_id.size(); i++)
            {

                for (int j = 0; j < parsed[user2Index].followers_id.size(); j++)
                {
                    if(parsed[user1Index].followers_id[i]==parsed[user2Index].followers_id[j])
                        mutualFriends.push_back((parsed[user1Index].followers_id[i]));
                }

            }
            for(int i=0;i<mutualFriends.size();i++)
            {
                for(int j=0;j<parsed.size();j++)
                {
                    if(mutualFriends[i]==parsed[j].user_id)
                        mutualNames.push_back(parsed[j].user_name);
                }

            }
            QString finalResult;
            if(!mutualFriends.empty())
            {
                if(mutualFriends.size()==1)
                    finalResult="The mutual follower is :\n";
                else
                    finalResult="The mutual followers are :\n";
                for(int i=0;i<mutualFriends.size();i++)
                {
                    finalResult+=mutualNames[i];
                    finalResult+=" whose id is ";
                    finalResult+=mutualFriends[i];
                    if(i<(mutualFriends.size()-1))
                        finalResult+=" and ";
                    if(i==(mutualFriends.size()-1))
                        finalResult+=".";
                }
            }
            else
            {
                finalResult="There are no mutual followers between the 2 users \n";
            }

            QDialog outputDialog;
            outputDialog.setWindowTitle("Mutual Followers");
            QLabel *resultLabel = new QLabel(finalResult, &outputDialog);
            QVBoxLayout *layout2 = new QVBoxLayout(&outputDialog);
            layout2->addWidget(resultLabel);
            QPushButton *okkButton=new QPushButton("ok", &outputDialog);
            layout2->addWidget(okkButton);
            QObject::connect(okkButton, &QPushButton::clicked, [&]() {
                outputDialog.accept();
            });
            outputDialog.exec();
        });
        inputDialog.exec();

    }
}

void XMLer::suggestFollowers()
{

    if (!resultTextEdit->toPlainText().isEmpty()) {
        QString textContent=resultTextEdit->toPlainText();
        QVector<QString> xmlLines;
        QTextStream in(&textContent);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            xmlLines.push_back(line);
        }

        QVector<Vertex> parsed;
        parsed = xmlParse(xmlLines);
        QString suggestedFollowers;
        QString temp;
        temp.clear();
        QVector<int>followersIndex;
        QVector<QString>suggestedIDS;
        suggestedFollowers.clear();
        for(int i=0;i<parsed.size();i++)
        {
           // qDebug()<<"the followers suggested for user " <<  parsed[i].user_name<<"are :\n";
            for(int j=0;j<parsed[i].followers_id.size();j++)
            {
                for(int k=0;k<parsed.size();k++)
                {
                    if(parsed[i].followers_id[j]==parsed[k].user_id)
                    {
                        followersIndex.push_back(k);
                        continue;
                    }

                }
            }
            for(int j=0;j<followersIndex.size();j++)
            {
                for(auto follower : parsed[followersIndex[j]].followers_id)
                {
                    if(!suggestedIDS.contains(follower) && follower !=parsed[i].user_id)
                        suggestedIDS.push_back(follower);

                }

            }
            for(int j=0;j<suggestedIDS.size();j++)
            {
                for(int k=0;k<parsed.size();k++)
                {
                    if(suggestedIDS[j]==parsed[k].user_id)
                    {
                        suggestedFollowers.append(parsed[k].user_name);
                        suggestedFollowers.append(" whose id is ");
                        suggestedFollowers.append(parsed[k].user_id);
                        if(j<(suggestedIDS.size()-1))
                            suggestedFollowers.append(",");
                        if(j==(suggestedIDS.size()-1))
                            suggestedFollowers.append(".");
                        continue;
                    }

                }
            }
            if(suggestedFollowers.isEmpty())
            {
                temp.append("\n\nNo followers to suggest for user "+parsed[i].user_name+".");
                    
            }
            else if(suggestedIDS.size()==1)
            {
                temp.append("\n\nThe follower suggested for user "+parsed[i].user_name+" is :\n"+suggestedFollowers);
            }
            else
            {
                temp.append("\n\nThe followers suggested for user "+parsed[i].user_name+" are :\n"+suggestedFollowers);
            }

            suggestedFollowers.clear();
            suggestedIDS.clear();
            followersIndex.clear();
        }
        if(temp.isEmpty())
        {
            temp="No followers to suggest :(\n";
        }
        if(temp[0]=='\n')
        {
            temp.remove(0,1);
            temp.remove(0,1);
        }
        QDialog inputDialog;
        inputDialog.setWindowTitle("Suggested Followers");

        QFormLayout formLayout(&inputDialog);
        QLabel *resultLabel = new QLabel(temp, &inputDialog);

        formLayout.addRow(resultLabel);
        QPushButton okButton("ok");
        formLayout.addRow(&okButton);
        QObject::connect(&okButton, &QPushButton::clicked, [&]() {
            inputDialog.accept();
        });
        inputDialog.exec();

    }
}

void XMLer::grapInfo()
{   // for the most influencer user (has the most followers)
    int maxNumOfFollowers=-1;
    QString NameOfMaxNumOfFollowers="";
    // for  the most active user (connected to lots of users)
    int maxDeg=-1;
    QString NameOfMaxDeg="";
    int counter=0;

    if (!resultTextEdit->toPlainText().isEmpty()) {
        QString textContent=resultTextEdit->toPlainText();
        QVector<QString> xmlLines;
        QTextStream in(&textContent);
        QString finalResult;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            xmlLines.push_back(line);
        }

        QVector<Vertex> parsed;
        parsed = xmlParse(xmlLines);
        for(int i=0;i<parsed.size();i++)
        {
            if(parsed[i].followers_id.size()>maxNumOfFollowers)
            {
                maxNumOfFollowers= parsed[i].followers_id.size();
                NameOfMaxNumOfFollowers = parsed[i].user_name;
            }
        }
        finalResult.append("The most influencer user is " +NameOfMaxNumOfFollowers+" who has "+QString::number(maxNumOfFollowers)+" followers.\n");
        
        // Testing
        //qDebug()<<"the most influencer user ID" << maxNumOfFollowers<<"\n"<<"the most influencer user Name :"<<NameOfMaxNumOfFollowers;


        for(int i=0;i<parsed.size();i++)
        {
            counter+= parsed[i].followers_id.size();
            for (int j = i+1; j < parsed.size(); ++j) {
                for(int y=0;y<parsed[j].followers_id.size();y++)
                {
                    if(parsed[i].user_id==parsed[j].followers_id[y])
                        counter++;
                }
            }
            if(counter>maxDeg)
            {
                maxDeg=counter;
                NameOfMaxDeg=parsed[i].user_name;
            }
            counter=0;
        }
        finalResult.append("The most active user is " +NameOfMaxDeg+" who has "+QString::number(maxDeg)+" connections.\n");
        QDialog inputDialog;
        inputDialog.setWindowTitle("Graph Info");

        QFormLayout formLayout(&inputDialog);
        QLabel *resultLabel = new QLabel(finalResult, &inputDialog);

        formLayout.addRow(resultLabel);
        QPushButton okButton("ok");
        formLayout.addRow(&okButton);
        QObject::connect(&okButton, &QPushButton::clicked, [&]() {
            inputDialog.accept();
        });
        inputDialog.exec();
        qDebug()<<"the most active user Degree" << maxDeg<<"\n"<<"the most active userName:"<<NameOfMaxDeg;
    }
}
/****************************** Private Functions **********************************/
/* Level 1 */
// Check consistency
// Reading tags
QString XMLer::extractOpeningTag(const QString& line) {
    int start = line.indexOf('<');
    int end = line.indexOf('>', start);
    if (start != -1 && line[start + 1] != '/')
        return line.mid(start + 1, end - start - 1);
    else
        return "INVALID";
}

QString XMLer::extractClosingTag(const QString& line) {
    int start = line.indexOf('/');
    int end = line.indexOf('>', start);
    if (start != -1)
        return line.mid(start + 1, end - start - 1);
    else
        return "INVALID";
}
bool XMLer::checkConsistency(const QVector<QString>& xmlLines) {
    QStack<QString> tagStack;
    for (int i = 0; i < xmlLines.size(); i++) {
        QString currentLine = xmlLines[i];
        QString openingTag = extractOpeningTag(currentLine);
        QString closingTag = extractClosingTag(currentLine);

        if (openingTag != "INVALID" || closingTag != "INVALID") {
            if (openingTag != "INVALID" && closingTag != "INVALID") {
                // Both opening and closing tags present
                if (openingTag != closingTag) {
                    return false;
                }
            } else if (openingTag != "INVALID") {
                // Opening tag encountered
                tagStack.push(openingTag);
            } else if (closingTag != "INVALID") {
                // Closing tag encountered
                if (tagStack.empty() || closingTag != tagStack.top()) {
                    return false;
                }
                tagStack.pop();
            }
        }
    }

    return tagStack.empty();
}
/* detect error */
bool XMLer::detectErrors(const QString& openTag, const QString& closedTag, QStack<QString>& tagStack, QString& errorType)
{
    QStack<QString> tempStack;

    if (openTag.compare("INVALID", Qt::CaseInsensitive) && closedTag.compare("INVALID", Qt::CaseInsensitive))
    {
        if (openTag.compare(closedTag, Qt::CaseInsensitive))
        {
            errorType = "Mismatched tags: " + openTag + " and " + closedTag;
            return false;
        }
    }

    if (openTag.compare("INVALID", Qt::CaseInsensitive))
    {
        tagStack.push(openTag);
    }
    if (closedTag.compare("INVALID", Qt::CaseInsensitive))
    {
        if (!tagStack.isEmpty())
        {
            if (!closedTag.compare(tagStack.top(), Qt::CaseInsensitive)) //matching
                tagStack.pop();
            else
            {
                while (!tagStack.isEmpty() && closedTag.compare(tagStack.top(), Qt::CaseInsensitive)) //not matchy
                {
                    tempStack.push(tagStack.top());
                    tagStack.pop();
                }
                // If the tag stack is empty, set errorType for a missing open tag
                if (tagStack.isEmpty())
                {
                    errorType = "<" + closedTag + ">";
                    while (!tempStack.isEmpty())
                    {
                        tagStack.push(tempStack.top());
                        tempStack.pop();
                    }
                }
                else
                {
                    // If tag stack is not empty, set errorType for a missing closed tag
                    tagStack.pop();
                    while (!tempStack.isEmpty())
                    {
                        tagStack.push(tempStack.top());
                        tempStack.pop();
                    }
                    errorType = "</" + tagStack.top() + ">";
                    tagStack.pop();
                }
                return false;
            }
        }
        else
        {
            errorType = "<" + closedTag + ">";
            return false;
        }
        return true;
    }
    return true;
}

/* find error */
QVector<ErrorData> XMLer::findErrors(const QVector<QString>& xmlLines)
{
    QVector<ErrorData> errorVector;
    ErrorData error;
    QString errorType;
    QStack<QString> tagStack;
    QString line;

    for (int i = 0; i < xmlLines.size(); i++)
    {
        line = xmlLines[i];
        QString openTag;
        QString closedTag;
        openTag = extractOpeningTag(line);
        closedTag = extractClosingTag(line);

        if(i==2)
            qDebug() << closedTag;

        if (openTag.compare("INVALID", Qt::CaseInsensitive) || closedTag.compare("INVALID", Qt::CaseInsensitive)) //if valid
        {
            qDebug() << closedTag;
            if (!detectErrors(openTag, closedTag, tagStack, errorType)) //detectErrors: returns false if there is an error
            {
                error.errorType = errorType;
                error.errorLocation = i;

                errorVector.push_back(error);
            }
        }
    }

    //lw file 5eles w lesa feh tags fel stack
    int i = xmlLines.size();

    while (!tagStack.isEmpty())
    {
        QString err = "</" + tagStack.top() + ">";
        error.errorType = err;
        error.errorLocation = i;
        errorVector.push_back(error);
        i++;
        tagStack.pop();
    }

    return errorVector;
}

/* display error */
QString XMLer::displayErrors(const QVector<ErrorData>& errorVector)
{
    QString errorMessage;

    for (const auto& error : errorVector)
    {
        if (error.errorType.size() > 30)
        {
            errorMessage += QString("Error at line %1: %2\n").arg(error.errorLocation + 1).arg(error.errorType);
        }
        else
        {
            errorMessage += QString("Error at line %1: Missing %2\n").arg(error.errorLocation + 1).arg(error.errorType);
        }
    }

    return errorMessage;
}

/* correct error */

QStringList XMLer::error_corrector(const QStringList& xmlLines, const QVector<ErrorData>& errorVector)
{
    QStringList correctedLines;
    int counter = 0;

    for (int i = 0; i < xmlLines.size(); ++i) {
        if (counter < errorVector.size() && i == errorVector[counter].errorLocation) {
            const QString& errorType = QString(errorVector[counter].errorType);

            if (errorType.length() > 25) {
                // Mismatched tags error, replace the closing tag with the opening tag
                QString correctedLine = xmlLines[i];
                correctedLine.replace(extractClosingTag(xmlLines[i]), extractOpeningTag(xmlLines[i]));
                correctedLines.push_back(correctedLine);
            } else {
                QString temp;
                // Normal error, insert the error type and the original line
                int count = 0;

                for (const QChar& ch : xmlLines[i]) {
                    if (ch.isSpace()) {
                        // Increment count for each whitespace character
                        count++;
                    } else {
                        // Break the loop when a non-whitespace character is encountered
                        break;
                    }
                }
                temp  =  xmlLines[i].trimmed();
                correctedLines.push_back(QString(count, ' ') + errorType + temp);
                // correctedLines.push_back(errorType + temp);
                //  correctedLines.push_back(xmlLines[i]);
            }

            ++counter;
        } else {
            correctedLines.push_back(xmlLines[i]);
        }
    }

    // If there are remaining errors at the end of the file, add them to the corrected lines
    for (; counter < errorVector.size(); ++counter) {
        correctedLines.push_back(QString(errorVector[counter].errorType));
    }

    return correctedLines;
}


/* Level 2 */

// Parsing Algorithim
QVector<Vertex> XMLer::xmlParse(const QVector<QString>& xmlLines){

    QVector<Vertex> userVector;
    int userNum = -1;
    //int USERNUM=-1;
    int id_counter = 0;
    int postNumber=-1;
    //int topicNumber=-1;

    for (int j = 0; j < xmlLines.size(); j++)
    {
        for(int i = 0; i < (xmlLines[j]).size(); i++)
        {
            //qDebug() << i;
            QString tag = "";

            if (xmlLines[j][i] == '<' && xmlLines[j][i + 1] != '/') // open tag
            {
                i++;
                while (xmlLines[j][i] != '>')
                {
                    tag += xmlLines[j][i];
                    i++;
                }
                // qDebug() << tag;
                if (tag == "user")
                {
                    if (userNum < userVector.size())
                    {
                        userNum++;
                        postNumber=-1;
                        id_counter = 0;
                        userVector.resize(userNum + 1); // Ensure the vector has enough space
                    }
                    else
                    {
                        qDebug() << "Error: userNum exceeds vector size.";
                    }
                }

                if (tag == "name")
                {
                    QString name = "";
                    i++; // don't take > of open tag
                    while (xmlLines[j][i] != '<')
                    {
                        name += xmlLines[j][i];
                        i++;
                    }
                    if (userNum >= 0 && userNum < userVector.size())
                    {
                        userVector[userNum].user_name = name;
                    }
                    else
                    {
                        qDebug() << "Error: Invalid userNum.";
                    }
                }

                if (tag == "id")
                {
                    QString userID = "";
                    i++; // don't take > of open tag
                    while (xmlLines[j][i] != '<')
                    {
                        userID += xmlLines[j][i];
                        i++;
                    }
                    qDebug() << userID;
                    if (userNum >= 0 && userNum < userVector.size())
                    {
                        if (id_counter == 0)
                        {
                            userVector[userNum].user_id = userID;
                        }
                        else
                        {
                            userVector[userNum].followers_id.push_back(userID);
                        }
                        id_counter++;
                    }
                    else
                    {
                        qDebug() << "Error: Invalid userNum.";
                    }
                }
               
                if(tag=="body")
                {
                    postNumber++;
                    j++;
                    
                    QString content;
                    while(!xmlLines[j].contains("/body"))
                    {
                       
                        content.append(xmlLines[j]);
                        j++;
                    }
                    userVector[userNum].posts.push_back(content);
                    
                }
                if(tag=="topic")
                {
                   
                    j++;
                    userVector[userNum].topics.resize(postNumber+1);
                    userVector[userNum].topics[postNumber].push_back(xmlLines[j].trimmed());
                    
                }
            }
        }
    }

    return userVector;
}

/***********************END OF PARSING*****************************/
void XMLer::createGraphVisualization(const QVector<Vertex> &vec)
{

    remove("Graph.dot");
    QString temp;

    QString dotFile_Str = "digraph test {\nnode [shape=";
    dotFile_Str += temp + "record ," + temp + "  color=" + temp + "black" + temp + "] \n\n";

    for (const auto &v : vec)
    {
        dotFile_Str += v.user_id + "[ label = " + temp + "\"{Name: " + v.user_name + " | ID: " + v.user_id + "} \"" + temp + "]\n" + v.user_id + " -> {";

        for (unsigned int i = 0; i < v.followers_id.size(); i++)
        {
            dotFile_Str += v.followers_id.at(i);
            if (i < v.followers_id.size() - 1)
            {
                dotFile_Str += ",";
            }
        }
        dotFile_Str += "}\n";
    }
    dotFile_Str += "}";

    std::ofstream img("Graph.dot");
    img << dotFile_Str.toStdString();
    img.close();

    system("dot -Tpng -O Graph.dot");
    QString imagePath = "Graph.dot.png";

    QPixmap image(imagePath);

    if (image.isNull())
    {
        QMessageBox::warning(nullptr, "Error", "Failed to open the image.");
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIconPixmap(image);
        msgBox.setWindowTitle("Graph Visualization");
        msgBox.exec();
    }
}
