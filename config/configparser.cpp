#include "configparser.h"

#include <QFile>
#include <QDir>
#include <QJsonParseError>
#include <QJsonObject>

bool BookSet::initialize(const QString &config_path)
{
    QFile inFile(config_path + "/config.json");
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error while opening config file";
        return false;
    }
    QByteArray data = inFile.readAll();
    inFile.close();

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
    if (doc.isNull()) {
        qDebug() << "Config file parsing failed";
    } else {
        //        qDebug() << doc;
    }

    auto root = doc.object();

    _publisherName = root["publisher_name"].toString();
    _publisherLogoPath = root["publisher_logo_path"].toString();
    _publisherFullLogoPath = root["publisher_full_logo_path"].toString();
    _fullscreen = root["fullscreen"].toBool();
    _bookTitle = root["book_title"].toString();
    _bookCover = root["book_cover"].toString();
    auto bks = root["books"].toArray();
    _bookCount = bks.size();
    for (auto const &b : bks) {
        auto bObj = b.toObject();

        Book *book = new Book;
        book->_type = bObj["type"].toInt();
        book->_name = bObj["name"].toString();
        book->_modules = handleBooksModules(bObj["modules"].toArray());
        // adding all pages to a single vector
        for (Module *m: book->_modules) {
            for(Page *p: m->_pages) {
                book->_pages.push_back(p);
            }
        }
        _books.push_back(book);
    }
    return true;
}

QVector<Module*> BookSet::handleBooksModules(const QJsonArray &doc)
{
    QVector<Module*> modules;
    for (const auto & m : doc) {
        auto mObj = m.toObject();
        Module *module = new Module;
        module->_type = mObj["type"].toString();
        module->_name = mObj["name"].toString();
        if (module->_type == "game") {
            auto gArr = mObj["games"].toArray();
            for (const auto &g : gArr) {
                auto gObj = g.toObject();
                Game *game = new Game;
                game->_name = gObj["name"].toString();
                game->_type = gObj["type"].toString();
                auto sWords = gObj["secretwords"].toArray();
                for (const auto &sw : sWords) {
                    game->_secretwords.push_back(sw.toString());
                }
                module->_games.push_back(game);
            }
        } else {
            auto cnts = mObj["pages"].toArray();
            for (const auto &c : cnts) {
                auto cObj = c.toObject();
                Page *page = new Page;
                page->_page_number = cObj["page_number"].toInt();
                page->_image_path = cObj["image_path"].toString();

                auto sctns = cObj["sections"].toArray();

                for (const auto &s : sctns) {
                    auto sObj = s.toObject();
                    Section *section = new Section;
                    section->_title = sObj["title"].toString();
                    section->_type = sObj["type"].toString();
                    section->_audio_path = sObj["audio_path"].toString();
                    section->_video_path = sObj["video_path"].toString();
                    auto scObj = sObj["coords"].toObject();
                    section->_coords = QRect(scObj["x"].toInt(), scObj["y"].toInt(), scObj["w"].toInt(), scObj["h"].toInt());
                    // magnifier
                    scObj = sObj["magnifier"].toObject();
                    auto scCoord = scObj["coords"].toObject();
                    Magnifier *m = new Magnifier;
                    m->_coords = QRect(scCoord["x"].toInt(), scCoord["y"].toInt(), scCoord["w"].toInt(), scCoord["h"].toInt());
                    m->_sectionPath = scObj["section_path"].toString();
                    section->_magnifier = m;

                    // Activity
                    scObj = sObj["activity"].toObject();
                    Activity *act = new Activity;
                    act->_type = scObj["type"].toString();
                    act->_section_path = scObj["section_path"].toString();
                    act->_header_text = scObj["headerText"].toString();
                    cObj = scObj["coords"].toObject();
                    act->_coords = QRect(cObj["x"].toInt(), cObj["y"].toInt(), cObj["w"].toInt(), cObj["h"].toInt());

                    auto actansws = scObj["answer"].toArray();

                    for (const auto &a : actansws) {
                        auto aObj = a.toObject();
                        Answer *answer = new Answer;
                        answer->_isCorrect = aObj["isCorrect"].toBool();
                        answer->_realAnswer = aObj["real_answer"].toString();
                        answer->_text = aObj["text"].toString();
                        answer->_diagonal = aObj["diagonal"].toBool();
                        answer->_diagonalSide = aObj["diagonal_side"].toString();
                        answer->_no = aObj["no"].toInt();
                        auto cObj = aObj["coords"].toObject();
                        answer->_coords = QRect(cObj["x"].toInt(), cObj["y"].toInt(), cObj["w"].toInt(), cObj["h"].toInt());

                        auto larr = aObj["letters"].toArray();
                        for (const auto &le : larr) {
                            auto lObj = le.toObject();
                            Letter *l = new Letter;
                            l->_text = lObj["text"].toString();
                            auto lcoords = lObj["coords"].toObject();
                            l->_coords = QRect(lcoords["x"].toInt(), lcoords["y"].toInt(), lcoords["w"].toInt(), lcoords["h"].toInt());

                            answer->_letters.push_back(l);
                        }

                        auto circExtraArr = aObj["extra"].toArray();
                        for (const auto &ce : circExtraArr) {
                            auto ceObj = ce.toObject();
                            CircleExtra *c = new CircleExtra;
                            c->_type = ceObj["type"].toString();
                            c->_text = ceObj["text"].toString();
                            auto ccoords = ceObj["coords"].toObject();
                            c->_coords = QRect(ccoords["x"].toInt(), ccoords["y"].toInt(), ccoords["w"].toInt(), ccoords["h"].toInt());
                            answer->_circleExtra.push_back(c);
                        }
                        act->_answers.push_back(answer);
                    }

                    auto actArr = scObj["sentences"].toArray();
                    for (const auto &a : actArr) {
                        Sentences *s = new Sentences;
                        auto sObj = a.toObject();
                        s->_sentence = sObj["sentence"].toString();
                        s->_sentenceAfter = sObj["sentence_after"].toString();
                        s->_word = sObj["word"].toString();
                        act->_sentences.push_back(s);
                    }

                    actArr = scObj["words"].toArray();
                    for (const auto &a : actArr) {
                        act->_words.push_back(a.toString());
                    }


                    section->_activity = act;
                    scObj = sObj["show_all_answers"].toObject();
                    section->_show_all_answers = QRect(scObj["x"].toInt(), scObj["y"].toInt(), scObj["w"].toInt(), scObj["h"].toInt());
                    scObj = sObj["lock_screen"].toObject();
                    section->_lock_screen = QRect(scObj["x"].toInt(), scObj["y"].toInt(), scObj["w"].toInt(), scObj["h"].toInt());

                    auto answs = sObj["answer"].toArray();

                    for (const auto &a : answs) {
                        auto aObj = a.toObject();
                        Answer *answer = new Answer;
                        answer->_text = aObj["text"].toString();
                        answer->_no = aObj["no"].toInt();
                        answer->_diagonal = aObj["diagonal"].toBool();
                        answer->_diagonalSide = aObj["diagonal_side"].toString();
                        auto cObj = aObj["coords"].toObject();
                        answer->_coords = QRect(cObj["x"].toInt(), cObj["y"].toInt(), cObj["w"].toInt(), cObj["h"].toInt());
                        answer->_isCorrect = aObj["isCorrect"].toBool();
                        cObj = aObj["sourceCoords"].toObject();
                        answer->_sourceCoords = QRect(cObj["x"].toInt(), cObj["y"].toInt(), cObj["w"].toInt(), cObj["h"].toInt());
                        answer->_sourceText = cObj["text"].toString();

                        //extra
                        auto circExtraArr = aObj["extra"].toArray();
                        for (const auto &ce : circExtraArr) {
                            auto ceObj = ce.toObject();
                            CircleExtra *c = new CircleExtra;
                            c->_type = ceObj["type"].toString();
                            c->_text = ceObj["text"].toString();
                            auto ccoords = ceObj["coords"].toObject();
                            c->_coords = QRect(ccoords["x"].toInt(), ccoords["y"].toInt(), ccoords["w"].toInt(), ccoords["h"].toInt());
                            answer->_circleExtra.push_back(c);
                        }


                        section->_answers.push_back(answer);
                    }

                    auto audioExtraObj = sObj["audio_extra"].toObject();
                    AudioExtra *ae = new AudioExtra;
                    ae->_path = audioExtraObj["path"].toString();
                    cObj = audioExtraObj["coords"].toObject();
                    ae->_coords = QRect(cObj["x"].toInt(), cObj["y"].toInt(), cObj["w"].toInt(), cObj["h"].toInt());
                    section->_audio_extra = ae;

                    page->_sections.push_back(section);
                }
                module->_pages.push_back(page);
            }
        }
        modules.push_back(module);
    }
    return modules;
}

bool ConfigParser::initialize()
{
    QDir directory = QDir::currentPath() + "/books";
    directory.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    auto dirList = directory.entryList();
    bool result = true;
    for (const auto& d : dirList) {
        BookSet *bset = new BookSet;
        result = bset->initialize(directory.path() + "/" + d);
        _bookSets.push_back(bset);
    }
    return result;
}
