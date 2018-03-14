/*
 *  Copyright © 2018 Gennady Chernyshchuk <genaloner@gmail.com>
 *
 *  This file is part of QOnlineTranslator.
 *
 *  QOnlineTranslator is free library; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a get of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QMediaPlayer>
#include <QJsonDocument>
#include <QJsonArray>

#include "qonlinetranslator.h"

const QStringList QOnlineTranslator::LANGUAGE_NAMES = { QT_TR_NOOP("Automatically detect"), QT_TR_NOOP("Afrikaans"), QT_TR_NOOP("Amharic"), QT_TR_NOOP("Arabic"),
                                                        QT_TR_NOOP("Azerbaijani"), QT_TR_NOOP("Belarusian"), QT_TR_NOOP("Bulgarian"), QT_TR_NOOP("Bengali"),
                                                        QT_TR_NOOP("Bosnian"), QT_TR_NOOP("Catalan"), QT_TR_NOOP("Cebuano"), QT_TR_NOOP("Corsican"),
                                                        QT_TR_NOOP("Czech"), QT_TR_NOOP("Welsh"), QT_TR_NOOP("Danish"), QT_TR_NOOP("German"),
                                                        QT_TR_NOOP("Greek"), QT_TR_NOOP("English"), QT_TR_NOOP("Esperanto"), QT_TR_NOOP("Spanish"),
                                                        QT_TR_NOOP("Estonian"), QT_TR_NOOP("Basque"), QT_TR_NOOP("Persian"), QT_TR_NOOP("Finnish"),
                                                        QT_TR_NOOP("Fijian"), QT_TR_NOOP("French"), QT_TR_NOOP("Frisian"), QT_TR_NOOP("Irish"),
                                                        QT_TR_NOOP("Scots Gaelic"), QT_TR_NOOP("Galician"), QT_TR_NOOP("Gujarati"), QT_TR_NOOP("Hausa"),
                                                        QT_TR_NOOP("Hawaiian"), QT_TR_NOOP("Hebrew"), QT_TR_NOOP("Hindi"), QT_TR_NOOP("Hmong"),
                                                        QT_TR_NOOP("Croatian"), QT_TR_NOOP("Haitian Creole"), QT_TR_NOOP("Hungarian"), QT_TR_NOOP("Armenian"),
                                                        QT_TR_NOOP("Indonesian"), QT_TR_NOOP("Igbo"), QT_TR_NOOP("Icelandic"), QT_TR_NOOP("Italian"),
                                                        QT_TR_NOOP("Japanese"), QT_TR_NOOP("Javanese"), QT_TR_NOOP("Georgian"), QT_TR_NOOP("Kazakh"),
                                                        QT_TR_NOOP("Khmer"), QT_TR_NOOP("Kannada"), QT_TR_NOOP("Korean"), QT_TR_NOOP("Kurdish"),
                                                        QT_TR_NOOP("Kyrgyz"), QT_TR_NOOP("Latin"), QT_TR_NOOP("Luxembourgish"), QT_TR_NOOP("Lao"),
                                                        QT_TR_NOOP("Lithuanian"), QT_TR_NOOP("Latvian"), QT_TR_NOOP("Malagasy"), QT_TR_NOOP("Maori"),
                                                        QT_TR_NOOP("Macedonian"), QT_TR_NOOP("Malayalam"), QT_TR_NOOP("Mongolian"), QT_TR_NOOP("Marathi"),
                                                        QT_TR_NOOP("Malay"), QT_TR_NOOP("Maltese"), QT_TR_NOOP("Hmong Daw"), QT_TR_NOOP("Myanmar"),
                                                        QT_TR_NOOP("Nepali"), QT_TR_NOOP("Dutch"), QT_TR_NOOP("Norwegian"), QT_TR_NOOP("Chichewa"),
                                                        QT_TR_NOOP("Querétaro Otomi"), QT_TR_NOOP("Punjabi"), QT_TR_NOOP("Polish"), QT_TR_NOOP("Pashto"),
                                                        QT_TR_NOOP("Portuguese"), QT_TR_NOOP("Romanian"), QT_TR_NOOP("Russian"), QT_TR_NOOP("Sindhi"),
                                                        QT_TR_NOOP("Sinhala"), QT_TR_NOOP("Slovak"), QT_TR_NOOP("Slovenian"), QT_TR_NOOP("Samoan"),
                                                        QT_TR_NOOP("Shona"), QT_TR_NOOP("Somali"), QT_TR_NOOP("Albanian"), QT_TR_NOOP("Serbian (Cyrillic)"),
                                                        QT_TR_NOOP("Serbian (Latin)"), QT_TR_NOOP("Sesotho"), QT_TR_NOOP("Sundanese"), QT_TR_NOOP("Swedish"),
                                                        QT_TR_NOOP("Swahili"), QT_TR_NOOP("Tamil"), QT_TR_NOOP("Telugu"), QT_TR_NOOP("Tajik"),
                                                        QT_TR_NOOP("Thai"), QT_TR_NOOP("Filipino"), QT_TR_NOOP("Klingon"), QT_TR_NOOP("Klingon (pIqaD)"),
                                                        QT_TR_NOOP("Tongan"), QT_TR_NOOP("Turkish"), QT_TR_NOOP("Tatar"), QT_TR_NOOP("Tahitian"),
                                                        QT_TR_NOOP("Udmurt"), QT_TR_NOOP("Ukrainian"), QT_TR_NOOP("Urdu"), QT_TR_NOOP("Uzbek"),
                                                        QT_TR_NOOP("Vietnamese"), QT_TR_NOOP("Xhosa"), QT_TR_NOOP("Yiddish"), QT_TR_NOOP("Yoruba"),
                                                        QT_TR_NOOP("Yucatec Maya"), QT_TR_NOOP("Cantonese"), QT_TR_NOOP("Chinese Simplified"), QT_TR_NOOP("Chinese Traditional"),
                                                        QT_TR_NOOP("Zulu") };

const QStringList QOnlineTranslator::LANGUAGE_LONG_CODES = { "automatic", "afrikaans", "amharic", "arabic", "azerbaijani", "belarusian", "bulgarian", "bengali", "bosnian", "catalan",
                                                             "cebuano", "corsican", "czech", "welsh", "danish", "german", "greek", "english", "esperanto", "spanish",
                                                             "estonian", "basque", "persian", "finnish", "fijian", "french", "frisian", "irish", "scots-gaelic", "galician",
                                                             "gujarati", "hausa", "hawaiian", "hebrew", "hindi", "hmong", "croatian", "haitian-creole", "hungarian",
                                                             "armenian", "indonesian", "igbo", "icelandic", "italian", "japanese", "javanese", "georgian", "kazakh",
                                                             "khmer", "kannada", "korean", "kurdish", "kyrgyz", "latin", "luxembourgish", "lao", "lithuanian", "latvian",
                                                             "malagasy", "maori", "macedonian", "malayalam", "mongolian", "marathi", "malay", "maltese", "hmong-daw",
                                                             "myanmar", "nepali", "dutch", "norwegian", "chichewa", "querétaro-otomi", "punjabi", "polish", "pashto",
                                                             "portuguese", "romanian", "russian", "sindhi", "sinhala", "slovak", "slovenian", "samoan", "shona", "somali",
                                                             "albanian", "serbian-cyrillic", "serbian-latin", "sesotho", "sundanese", "swedish", "swahili", "tamil",
                                                             "telugu", "tajik", "thai", "filipino", "klingon", "klingon-piqad", "tongan", "turkish", "tatar", "tahitian",
                                                             "udmurt", "ukrainian", "urdu", "uzbek", "vietnamese", "xhosa", "yiddish", "yoruba", "yucatec-maya",
                                                             "cantonese", "chinese-simplified", "chinese-traditional", "zulu" };

const QStringList QOnlineTranslator::LANGUAGE_SHORT_CODES = { "auto", "af", "am", "ar", "az", "be", "bg", "bn", "bs", "ca", "ceb", "co", "cs", "cy", "da", "de", "el", "en", "eo", "es",
                                                              "et", "eu", "fa", "fi", "fj", "fr", "fy", "ga", "gd", "gl", "gu", "ha", "haw", "he", "hi", "hmn", "hr", "ht", "hu",
                                                              "hy", "id", "ig", "is", "it", "ja", "jv", "ka", "kk", "km", "kn", "ko", "ku", "ky", "la", "lb", "lo", "lt", "lv",
                                                              "mg", "mi", "mk", "ml", "mn", "mr", "ms", "mt", "mww", "my", "ne", "nl", "no", "ny", "otq", "pa", "pl", "ps", "pt",
                                                              "ro", "ru", "sd", "si", "sk", "sl", "sm", "sn", "so", "sq", "sr-cyrl", "sr-latn", "st", "su", "sv", "sw", "ta",
                                                              "te", "tg", "th", "tl", "tlh", "tlh-qaak", "to", "tr", "tt", "ty", "udm", "uk", "ur", "uz", "vi", "xh", "yi", "yo",
                                                              "yua", "yue", "zh-cn", "zh-tw", "zu" };

const QString QOnlineTranslator::TTS_URL =
        "http://translate.googleapis.com/translate_tts?ie=UTF-8&client=gtx&tl="
        "%1"
        "&q="
        "%2";

const QString QOnlineTranslator::TRANSLATION_URL =
        "https://translate.googleapis.com/translate_a/single?client=gtx&ie=UTF-8&oe=UTF-8&dt=bd&dt=ex&dt=ld&dt=md&dt=rw&dt=rm&dt=ss&dt=t&dt=at&dt="
        "%1"
        "&sl="
        "%2"
        "&tl="
        "%3"
        "&hl="
        "%4"
        "&q="
        "%5";

const QString QOnlineTranslator::TRANSLATION_SHORT_URL =
        "https://translate.googleapis.com/translate_a/single?client=gtx&sl="
        "%1"
        "&tl="
        "%2"
        "&dt=t&q="
        "%3";

QOnlineTranslator::QOnlineTranslator(const QString &text, const QString &translationLanguage, const QString &sourceLanguage, const QString &translatorLanguage, const bool &autoCorrect)
{
    translate(text, translationLanguage, sourceLanguage, translatorLanguage, autoCorrect);
}

QOnlineTranslator::QOnlineTranslator(const QString &text, const QString &translationLanguage, const QString &sourceLanguage, const QString &translatorLanguage)
{
    translate(text, translationLanguage, sourceLanguage, translatorLanguage);
}

QOnlineTranslator::QOnlineTranslator(const QString &text, const QString &translationLanguage, const QString &sourceLanguage)
{
    translate(text, translationLanguage, sourceLanguage);
}

QOnlineTranslator::QOnlineTranslator(const QString &text, const QString &translationLanguage)
{
    translate(text, translationLanguage);
}

QOnlineTranslator::QOnlineTranslator(const QString &text)
{
    translate(text);
}

void QOnlineTranslator::translate(const QString &text, QString translationLanguage, QString sourceLanguage, const QString &translatorLanguage, const bool &autoCorrect)
{
    // Detect system language if translateLanguage not specified
    if (translationLanguage == "auto") {
        QLocale locale;
        translationLanguage = locale.name();
        translationLanguage.truncate(translationLanguage.indexOf("_"));
    }

    // Set auto-correction query option
    QString autocorrection;
    if (autoCorrect == true)
        autocorrection = "qca";
    else
        autocorrection = "qc";

    QString url = TRANSLATION_URL.arg(autocorrection).arg(sourceLanguage).arg(translationLanguage).arg(translatorLanguage).arg(text);
    QString response = receiveQuery(url);

    // Check for network error
    if(response.startsWith("E")) {
        m_text = response;
        return;
    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonArray jsonData = jsonResponse.array();

    // Parse first sentense. If the answer contains more than one sentence, then at the end of the first one there will be a space
    m_text = jsonData.at(0).toArray().at(0).toArray().at(0).toString();
    for (int i = 1; m_text.endsWith(" "); i++) {
        m_text.append(jsonData.at(0).toArray().at(i).toArray().at(0).toString());
    }

    m_translationTranscription = jsonData.at(0).toArray().last().toArray().at(2).toString();
    m_sourceTranscription = jsonData.at(0).toArray().last().toArray().at(3).toString().replace(",", ", ");

    foreach (QJsonValue typeOfSpeach, jsonData.at(1).toArray()) {
        m_translationOptions.append(QPair<QString, QStringList>());
        m_translationOptions.last().first = typeOfSpeach.toArray().at(0).toString();
        foreach (QJsonValue translationOption, typeOfSpeach.toArray().at(2).toArray()) {
             m_translationOptions.last().second.append(translationOption.toArray().at(0).toString() + ": ");
             // Add the first word without ",", and then add the remaining words
             m_translationOptions.last().second.last().append(translationOption.toArray().at(1).toArray().at(0).toString());
             for (int i = 1; i < translationOption.toArray().at(1).toArray().size(); i++)
                 m_translationOptions.last().second.last().append(", " + translationOption.toArray().at(1).toArray().at(i).toString());
        }
    }

    m_sourceLanguage = jsonData.at(2).toString();
}

void QOnlineTranslator::say()
{
    QUrl preparedUrl = TTS_URL.arg(m_translationLanguage).arg(m_text);
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(preparedUrl);
    player->play();
}

void QOnlineTranslator::say(const QString &text, QString language)
{
    // Google don't support "auto" as argument for text-to-speech, so need to detect language manually
    if (language == "auto") {
        QString preparedUrl = TRANSLATION_SHORT_URL.arg("auto").arg("en").arg(text);
        language = receiveQuery(preparedUrl);
        language.chop(4);
        language = language.mid(language.lastIndexOf("\"") + 1);
    }

    QUrl preparedUrl = QUrl("http://translate.googleapis.com/translate_tts?ie=UTF-8&client=gtx&tl=" + language + "&q=" + text);
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(preparedUrl);
    player->play();
}

QString QOnlineTranslator::translateText(const QString &text, QString translationLanguage, QString sourceLanguage)
{
    // Detect system language if translateLanguage not specified
    if (translationLanguage == "auto") {
        QLocale locale;
        translationLanguage = locale.name();
        translationLanguage.truncate(translationLanguage.indexOf("_"));
    }

    QString preparedUrl = TRANSLATION_SHORT_URL.arg(sourceLanguage).arg(translationLanguage).arg(text);
    QString response = receiveQuery(preparedUrl);

    // Get first sentence from quotes (first quote always starts from 4 fourth character)
    QString translatedText = response.mid(4, response.indexOf("\",\"", 4) - 4);
    while (translatedText.endsWith(" ") || translatedText.endsWith("\\n")) {
        // The text has one more sentence if ends with "space" or "line break"
        response = response.mid(response.indexOf("],[\"") + 4);
        translatedText.append(response.left(response.indexOf("\",\"")));
    }

    return translatedText;
}

QString QOnlineTranslator::sourceLanguage()
{
    return m_sourceLanguage;
}

QString QOnlineTranslator::sourceTranscription()
{
    return m_sourceTranscription;
}

QString QOnlineTranslator::text()
{
    return m_text;
}

QString QOnlineTranslator::translationLanguage()
{
    return m_translationLanguage;
}

QString QOnlineTranslator::translationTranscription()
{
    return m_translationTranscription;
}

QList<QPair<QString, QStringList> > QOnlineTranslator::options()
{
    return m_translationOptions;
}

QString QOnlineTranslator::receiveQuery(const QString &url)
{
    // Send request
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url)));

    // Wait for the response
    QEventLoop event;
    QObject::connect(response, &QNetworkReply::finished, &event, &QEventLoop::quit);
    event.exec();

    if (response->error() == QNetworkReply::NoError)
        return response->readAll();
    else
        return "Error: " + QVariant::fromValue(response->error()).value<QString>();
}
