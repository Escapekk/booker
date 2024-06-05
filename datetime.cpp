#include "datetime.h"

bool Datetime::operator==(const Datetime& datetime) {
    return year == datetime.year && month == datetime.month && day == datetime.day &&
        startTime == datetime.startTime && endTime == datetime.endTime;
}
Datetime& Datetime::operator=(const Datetime& datetime) {
    year = datetime.year;
    month = datetime.month;
    day = datetime.day;
    weekday = datetime.weekday;
    startTime = datetime.startTime;
    endTime = datetime.endTime;
    return *this;
}

// �����ַ����������ʱ��
void Datetime::parseDatetime(const std::string& dateStr, const std::string& timeStr) {
    // dateStr: 2016-06-02
    // timeStr: 20:00~22:00
    year = stoi(dateStr.substr(0, 4));
    month = stoi(dateStr.substr(5, 2));
    day = stoi(dateStr.substr(8));
    getWeekdayByDate();

    startTime = stoi(timeStr.substr(0, 2));
    endTime = stoi(timeStr.substr(6, 2));
}

// �ж�����ʱ�������Ƿ��н���
bool Datetime::haveIntersection(const Datetime& datetime) {
    if (year == datetime.year && month == datetime.month && day == datetime.day) {
        return startTime < datetime.endTime&& endTime > datetime.startTime;
    }
    return false;
}

// �жϵ�ǰʱ���Ƿ�����datetime
bool Datetime::isLater(const Datetime& datetime) {
    if (year > datetime.year) {
        return true;
    }
    else if (year < datetime.year) {
        return false;
    }
    else {
        if (month > datetime.month) {
            return true;
        }
        else if (month < datetime.month) {
            return false;
        }
        else {
            if (day > datetime.day) {
                return true;
            }
            else if (day < datetime.day) {
                return false;
            }
            else {
                return startTime > datetime.startTime;
            }
        }
    }
}

// �������ڼ������ڼ�
int Datetime::getWeekdayByDate() {
    int y = year, m = month, d = day;
    if (m <= 2) {
        m += 12;
        y -= 1;
    }
    weekday = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7 + 1;
    return weekday;
}