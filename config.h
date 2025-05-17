#ifndef CONFIG_H
#define CONFIG_H

#include "homewindows.h"
#include <QHostInfo>
#include <QRegularExpression>

const QRegularExpression float_expr1("(?!\\d[\\.\\,][1-9]+)0+$");
const QRegularExpression float_expr2("[\\.\\,]$");

#define FORMAT_DATETIME_TO_STR "dd/MM/yyyy HH:mm:ss"
#define FORMAT_DATETIME_TO_STR2 "dd/MM/yyyy HH:mm"
#define FORMAT_DATE_TO_STR "dd/MM/yyyy"
#define FORMAT_TIME_TO_STR "HH:mm:ss"
#define FORMAT_DATE_TO_STR_MYSQL "yyyy-MM-dd"
#define float_str(value, f) QLocale().toString(value, 'f', f).remove(float_expr1).remove(float_expr2)
#define str_float(value) QLocale().toDouble(value)
#define hostinfo QHostInfo::localHostName().toLower()
#define hostuser QString(getenv("USER")).isEmpty() ? getenv("USERNAME") : getenv("USER")

extern HomeWindows *__parent;
extern QString __dbhost;
extern QString __dbschema;
extern QString __dbusername;
extern QString __dbpassword;
extern int __appFontSize;
extern int __userid;
extern int __userrole;

#endif // CONFIG_H
