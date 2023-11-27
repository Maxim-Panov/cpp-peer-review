#include <array>
#include <cstdint>

struct DateTime {
    uint8_t second = 0;
    uint8_t minute = 0;
    uint8_t hour = 0;
    uint8_t day = 0;
    uint8_t month = 0;
    uint16_t year = 0;
};

enum class DateStatus {
    WRONG_YEAR,
    WRONG_MONTH,
    WRONG_DAY,
    WRONG_HOUR,
    WRONG_MINUTE,
    WRONG_SECOND,
    CORRECT_DATE
};

DateStatus CheckDateTimeValidity(const DateTime& dt) {

    if (dt.year < 1 || dt.year > 9999)
        return DateStatus::WRONG_YEAR;

    if (dt.month < 1 || dt.month > 12)
        return DateStatus::WRONG_MONTH;

    bool leap = dt.year % 4 == 0 && (dt.year % 100 != 0 || dt.year % 400 == 0);
    const std::array month_lengths = { 31, 28 + leap, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (dt.day < 1 || dt.day > month_lengths[dt.month - 1])
        return DateStatus::WRONG_DAY;

    if (dt.hour < 0 || dt.hour > 23)
        return DateStatus::WRONG_HOUR;

    if (dt.minute < 0 || dt.minute > 59)
        return DateStatus::WRONG_MINUTE;

    if (dt.second < 0 || dt.second > 59)
        return DateStatus::WRONG_SECOND;

    return DateStatus::CORRECT_DATE;
}

int main() {
    return 0;
}