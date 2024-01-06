#ifndef XMLER_H
#define XMLER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QTextEdit>
#include <QFile>
#include <QStack>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonObject>
#include <QXmlStreamReader>
#include <QJsonDocument>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>


struct Node {
    QJsonValue value;
    QString name;
    bool isArray;
    Node* parent = nullptr;
};

struct ErrorData
{
    QString errorType;
    int errorLocation;
};

typedef struct {
    QString user_name;
    QString user_id;
    QStringList followers_id;
    QVector<QString> posts;
    QVector<QVector<QString>> topics;
}Vertex;



class XMLer : public QWidget {
    Q_OBJECT

public:
    XMLer(QWidget *parent = nullptr);

private slots:
    void browseFile();
    void compressXml();
    void decompressXml();
    void minifyXml();
    void checkXml();
    void findErrorXml();
    void formatXml();
    void ToJSON();
    void saveXml();
    void drawGraph();
    void correctXml();
    void suggestFollowers();
    void findMutual();
    void grapInfo();
    void postSearch();
private:
    QLineEdit *fileLineEdit;

    QPushButton *browseButton;
    QPushButton *compressButton;
    QPushButton *decompressButton;
    QPushButton *minifyingButton;
    QPushButton *checkButton;
    QPushButton *findButton;
    QPushButton *formatButton;
    QPushButton *JSON;
    QPushButton *saveAsXml;
    QPushButton *showSocialNetwork;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QPushButton *correctButton;
    QPushButton *suggestFollowersButton;
    QPushButton *findMutualButton;
    QPushButton *grapInfoButton;
    QPushButton *postSearchButton;

    QTextEdit *resultTextEdit;

    QString originalXmlContent;

    QString compressData(const QString& data);
    QString decompressData(const QString& data);

    /* Error Check */
    QString extractOpeningTag(const QString& line);
    QString extractClosingTag(const QString& line);
    bool    checkConsistency(const QVector<QString>& xmlLines);

    /* Find Error */

    bool detectErrors(const QString& openTag, const QString& closedTag, QStack<QString>& tagStack, QString& errorType);
    QVector<ErrorData> findErrors(const QVector<QString>& xmlLines);
    QString displayErrors(const QVector<ErrorData>& errorVector);

    /* Json */
    void open(const QString &Block, QString *JSON);
    void close(const QString &Block, QString *JSON);

    QJsonValue XMLtoJSON(QXmlStreamReader*reader);

    /* Mininfy */
    bool isWhitespace(QChar c);

    /* Parse XML */
    QVector<Vertex> xmlParse(const QVector<QString>& xmlLines);

    /* undo + redo */
    void undo(void);
    void redo(void);
    QVector<QString> error_corrector(const QVector<QString>& xml_vector, const QVector<ErrorData>& error_vector);

    /* Social Network */
    void createGraphVisualization(const QVector<Vertex> &vec);


};

#endif // XMLER_H
