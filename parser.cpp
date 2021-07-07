#include "parser.h"
#include"errors.h"
#include<QCoreApplication>

arguments_list parse_args(int argc, char *argv[]){
    auto parser = new QCommandLineParser;
    parser->setApplicationDescription("File finder app.");
    parser->addHelpOption();
    QCoreApplication app(argc, argv);
    parser->addOption({
                              {"d", "depth"},
                              "максимальная глубина поиска",
                              "Depth"
                      });
    parser->addOption({
                              {"e", "exact"},
                              "проверять точное совпадение по имени файла, вместо поиска подстроки."
                      });
    parser->addOption({
                              {"f", "full-path"},
                              "полный абсолютный путь до файла."
                      });
    parser->addOption({
                              {"p", "relative-path"},
                              "относительный путь до файла.",
                      });
    parser->addOption({
                              {"е", "type"},
                              "что искать — директорию (d) или файл (f).",
                              "Type"
                      });
    parser->addOption({
                              {"i", "case-insensitive"},
                              "при поиске не учитывать регистр (по умолчанию регистр должен учитываться).",
                      });

    parser->addOption({
                              "info",
                              "выводит информацию о текущей директории.",
                      });
    parser->addOption({
                              "analyze-huge-files",
                              "ищет файлы с размером больше 100 МБ.",
                      });
    parser->addOption({
                              "analyze-huge-files-noext",
                              "ищет файлы с размером больше 100 МБ и без расширения.",
                      });
    parser->addOption({
                              {"r", "regex"},
                              "рассматривать EXPR как регулярное выражение.",
                      });
    parser->addOption({
                              {"g", "size-gt"},
                              "искать файлы с размером больше либо равным SIZE",
                              "SIZE"
                      });
    parser->addOption({
                              {"s", "size-lt"},
                              "искать файлы с размером меньше либо равным SIZE",
                              "SIZE"
                      });
    parser->process(app);
    if (!parser->parse(QCoreApplication::arguments()))
        error(parser->errorText().toLatin1().toStdString());
    arguments_list result;
    if(argc >= 2) result.DIR = argv[1];
    if(argc >= 3 && argv[2][0] != '-') result.EXPR = argv[2];
    result.Full_path = parser->isSet("full-path");
    result.Relative_path = parser->isSet("relative-path");
    if (result.Full_path == result.Relative_path && result.Full_path)
        error("Неверный формат ввода, ожидался один формат вывода");
    if(result.Full_path == result.Relative_path && !result.Full_path) result.Full_path = true;
    if(parser->isSet("size-lt")) result.Size_lt = parser->value("size-lt").toInt();
    if(parser->isSet("size-gt")) result.Size_gt = parser->value("size-gt").toInt();
    if(parser->isSet("depth")) result.Depth = parser->value("depth").toInt();
    result.Exact = parser->isSet("exact");
    if(result.Type != 'd' && result.Type != 't') {
        error("Неверный ввод, ожидалось [d/t]");
    }
    if(parser->isSet("type")) result.Type = parser->value("type").toStdString()[0];
    result.Case_insensitive = parser->isSet("case-insensitive");
    result.regex = parser->isSet("regex");
    if(!result.Exact) result.EXPR = ".*" + result.EXPR + ".*";
    auto flag = std::regex_constants::collate;
    if(result.Case_insensitive) flag |= std::regex_constants::icase;
    result.REGEXPR = std::regex(result.EXPR, flag);
    result.Info = parser->isSet("info");
    result.Huge_files = parser->isSet("analyze-huge-files");
    result.Huge_files_noext = parser->isSet("analyze-huge-files-noext");
    return result;
}



