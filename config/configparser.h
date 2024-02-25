#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <QRect>
#include <QString>
#include <QVector>
#include <QHash>
#include <QJsonArray>
#include <QObject>

struct CircleExtra : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QRect coords READ coords CONSTANT)
    Q_PROPERTY(QString text READ text CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)

public:
    QRect _coords;
    QString _text;
    QString _type;

    QRect coords() const { return _coords; }
    QString text() const { return _text; }
    QString type() const { return _type; }
};

struct Letter : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QRect coords READ coords CONSTANT)
    Q_PROPERTY(QString text READ text CONSTANT)

public:
    QRect _coords;
    QString _text;

    QRect coords() const { return _coords; }
    QString text() const { return _text; }
};

struct Answer : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(int no READ no CONSTANT)
    Q_PROPERTY(QRect coords READ coords CONSTANT)
    Q_PROPERTY(QString text READ text CONSTANT)
    Q_PROPERTY(bool isCorrect READ isCorrect CONSTANT)
    Q_PROPERTY(QRect sourceCoords READ sourceCoords CONSTANT)
    Q_PROPERTY(QString sourceText READ sourceText CONSTANT)
    Q_PROPERTY(bool diagonal READ diagonal CONSTANT)
    Q_PROPERTY(QString diagonalSide READ diagonalSide CONSTANT)
    Q_PROPERTY(QString realAnswer READ realAnswer CONSTANT)
    Q_PROPERTY(QVariantList letters READ letters CONSTANT)
    Q_PROPERTY(QVariantList circleExtra READ circleExtra CONSTANT)

public:
    Answer(QObject *parent = nullptr) :
        QObject(parent) {}

    int _no;
    QRect _coords;
    QString _text;
    bool _isCorrect;
    QRect _sourceCoords;
    QString _sourceText;
    bool _diagonal;
    QString _diagonalSide;
    QString _realAnswer;
    QVector<Letter*> _letters;
    QVector<CircleExtra*> _circleExtra;

    int no() const { return _no; }
    QRect coords() const { return _coords; }
    QString text() const { return _text; }
    bool isCorrect() const { return _isCorrect; }
    QRect sourceCoords() const { return _sourceCoords; }
    QString sourceText() const { return _sourceText; }
    bool diagonal() const { return _diagonal; }
    QString diagonalSide() const { return _diagonalSide; }
    QString realAnswer() const { return _realAnswer; }
    QVariantList letters() const {
        QVariantList l;
        for (Letter *a: _letters) {
            l << QVariant::fromValue(a);
        }
        return l;
    }
    QVariantList circleExtra() const {
        QVariantList l;
        for (CircleExtra *c: _circleExtra) {
            l << QVariant::fromValue(c);
        }
        return l;
    }
};

struct Sentences : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QString sentence READ sentence CONSTANT)
    Q_PROPERTY(QString sentenceAfter READ sentenceAfter CONSTANT)
    Q_PROPERTY(QString word READ word CONSTANT)
public:
    QString _sentence;
    QString _sentenceAfter;
    QString _word;

    QString sentence() const { return _sentence; }
    QString sentenceAfter() const { return _sentenceAfter; }
    QString word() const { return _word; }
};

struct Activity : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QRect coords READ coords CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QString sectionPath READ sectionPath CONSTANT)
    Q_PROPERTY(QVariantList answers READ answers CONSTANT)
    Q_PROPERTY(QVariantList words READ words CONSTANT)
    Q_PROPERTY(QVariantList sentences READ sentences CONSTANT)
    Q_PROPERTY(QString headerText READ headerText CONSTANT)

public:
    QRect _coords;
    QString _type;
    QString _section_path;
    QVector<Answer*> _answers;
    QVector<QString> _words;
    QVector<Sentences*> _sentences;
    QString _header_text;

    QRect coords() const { return _coords; }
    QString type() const { return _type; }
    QString sectionPath() const { return _section_path; }
    QVariantList answers() const {
        QVariantList l;
        for (Answer *a: _answers) {
            l << QVariant::fromValue(a);
        }
        return l;
    }
    QVariantList words() const {
        QVariantList l;
        for (QString s: _words) {
            l << QVariant::fromValue(s);
        }
        return l;
    }
    QVariantList sentences() const {
        QVariantList l;
        for (Sentences* s: _sentences) {
            l << QVariant::fromValue(s);
        }
        return l;
    }
    QString headerText() const { return _header_text; }
};

struct AudioExtra : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(QRect coords READ coords CONSTANT)
public:
    AudioExtra(QObject *parent = nullptr) :
        QObject(parent) {}

    QString _path;
    QRect _coords;
    QString path() const { return _path; }
    QRect coords() const { return _coords; }
};

struct Magnifier : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QString sectionPath READ sectionPath CONSTANT)
    Q_PROPERTY(QRect coords READ coords CONSTANT)
public:
    Magnifier(QObject *parent = nullptr) :
        QObject(parent) {}

    QString _sectionPath;
    QRect _coords;
    QString sectionPath() const { return _sectionPath; }
    QRect coords() const { return _coords; }
};

struct Section : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QRect coords READ coords CONSTANT)
    Q_PROPERTY(QVariant magnifier READ magnifier CONSTANT)
    Q_PROPERTY(QVariant activity READ activity CONSTANT)
    Q_PROPERTY(QString audio_path READ audioPath CONSTANT)
    Q_PROPERTY(QRect show_all_answers READ showAllAnswers CONSTANT)
    Q_PROPERTY(QRect lock_screen READ lockScreen CONSTANT)
    Q_PROPERTY(QString video_path READ videoPath CONSTANT)
    Q_PROPERTY(QVariantList answers READ answers CONSTANT)
    Q_PROPERTY(QVariant audio_extra READ audioExtra CONSTANT)
public:
    Section(QObject *parent = nullptr) :
        QObject(parent) {}

    QString _title;
    QString _type;
    QRect _coords;
    Magnifier *_magnifier;
    Activity *_activity;
    QString _audio_path;
    QRect _show_all_answers;
    QRect _lock_screen;
    QString _video_path;
    AudioExtra *_audio_extra;
    QVector<Answer*> _answers;

    QString title() const { return _title; }
    QString type() const { return _type; }
    QRect coords() const { return _coords; }
    QVariant magnifier() const { return QVariant::fromValue(_magnifier); }
    QVariant activity() const { return QVariant::fromValue(_activity); }
    QString audioPath() const { return _audio_path; }
    QRect showAllAnswers() const { return _show_all_answers; }
    QString videoPath() const { return _video_path; }
    QRect lockScreen() const { return _lock_screen; }
    QVariantList answers() const {
        QVariantList l;
        for (Answer *a: _answers) {
            l << QVariant::fromValue(a);
        }
        return l;
    }
    QVariant audioExtra() const {
        return QVariant::fromValue(_audio_extra);
    }
};

struct Page : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(int page_number READ pageNumber CONSTANT)
    Q_PROPERTY(QString image_path READ imagePath CONSTANT)
    Q_PROPERTY(QVariantList sections READ sections CONSTANT)
public:
    int _page_number;
    QString _image_path;
    QVector<Section*> _sections;

    int pageNumber() const { return _page_number; }
    QString imagePath() const { return _image_path; }
    QVariantList sections() const {
        QVariantList l;
        for (Section *s: _sections) {
            l << QVariant::fromValue(s);
        }
        return l;
    }
};

struct Game: public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QVariantList secretwords READ secretwords CONSTANT)
public:
    QString _name;
    QString _type;
    QVector<QString> _secretwords;

    QString name() const { return _name; }
    QString type() const { return _type; }
    QVariantList secretwords() const {
        QVariantList l;
        for (QString s: _secretwords) {
            l << QVariant::fromValue(s);
        }
        return l;
    }
};

struct Module : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QVariantList pages READ pages CONSTANT)
    Q_PROPERTY(QVariantList games READ games CONSTANT)
public:
    QString _name;
    QString _type;
    QVector<Page*> _pages;
    QVector<Game*> _games;

    QString name() const { return _name; }
    QString type() const { return _type; }
    QVariantList pages() const {
        QVariantList l;
        for (Page *c: _pages) {
            l << QVariant::fromValue(c);
        }
        return l;
    }
    QVariantList games() const {
        QVariantList l;
        for (Game *c: _games) {
            l << QVariant::fromValue(c);
        }
        return l;
    }
};

struct Book : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(int type READ type CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QVariantList modules READ modules CONSTANT)
    Q_PROPERTY(QVariantList pages READ pages CONSTANT)

public:
    int _type;
    QString _name;
    int _book_count;
    QVector<Module*> _modules;
    QVector<Page*> _pages;

    int type() const { return _type; }
    QString name() const { return _name; }
    QVariantList modules() const {
        QVariantList l;
        for (Module *m: _modules) {
            l << QVariant::fromValue(m);
        }
        return l;
    }
    QVariantList pages() const {
        QVariantList l;
        for (Page *p: _pages) {
            l << QVariant::fromValue(p);
        }
        return l;
    }
};

struct Vocabulary{
    QString vocab;
};

struct BookSet : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(int bookCount                    READ bookCount              CONSTANT)
    Q_PROPERTY(QString publisherName            READ publisherName          CONSTANT)
    Q_PROPERTY(QString publisherLogoPath        READ publisherLogoPath      CONSTANT)
    Q_PROPERTY(QString publisherFullLogoPath    READ publisherFullLogoPath  CONSTANT)
    Q_PROPERTY(QString bookTitle                READ bookTitle              CONSTANT)
    Q_PROPERTY(QString bookCover                READ bookCover              CONSTANT)
    Q_PROPERTY(QVariantList books               READ books                  CONSTANT)
    Q_PROPERTY(bool fullscreen                  READ fullscreen             CONSTANT)

public:

    bool initialize(const QString &config_path);

    QString _publisherName;
    QString _publisherLogoPath;
    QString _publisherFullLogoPath;
    QString _bookTitle;
    QString _bookCover;
    int _bookCount;
    QVector<Book*> _books;
    bool _fullscreen;


    int bookCount() const { return _bookCount; }
    QString publisherName() const { return _publisherName; }
    QString publisherLogoPath() const { return _publisherLogoPath; }
    QString publisherFullLogoPath() const { return _publisherFullLogoPath; }
    QString bookTitle() const { return _bookTitle; }
    QString bookCover() const { return _bookCover; }
    QVariantList books() const {
        QVariantList l;
        for (Book *b: _books) {
            l << QVariant::fromValue(b);
        }
        return l;
    }
    bool fullscreen() const { return _fullscreen; }

private:
    QVector<Module *> handleBooksModules(const QJsonArray &doc);
};

struct ConfigParser : public QObject {
private:
    Q_OBJECT
    Q_PROPERTY(QVariantList bookSets READ bookSets CONSTANT)
public:
    bool initialize();
    QVector<BookSet*> _bookSets;
    void setBookSets();
    QVariantList bookSets() const {
        QVariantList l;
        for (BookSet *b: _bookSets) {
            l << QVariant::fromValue(b);
        }
        return l;
    }
};

#endif // CONFIGPARSER_H
