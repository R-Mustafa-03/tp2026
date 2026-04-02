#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <complex>
#include <cmath>

namespace nspace {
    const double EPS = 1e-9;

    struct DataStruct {
        unsigned long long key1;
        std::complex<double> key2;
        std::string key3;
    };

    class iofmtguard {
    public:
        explicit iofmtguard(std::basic_ios<char>& s) :
            s_(s), width_(s.width()), precision_(s.precision()), fill_(s.fill()), fmt_(s.flags()) {
        }
        ~iofmtguard() {
            s_.width(width_);
            s_.fill(fill_);
            s_.precision(precision_);
            s_.flags(fmt_);
        }
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        std::streamsize precision_;
        char fill_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    // Удаление пробелов
    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        return str.substr(start, end - start + 1);
    }

    // Парсинг числа с суффиксом для key1
    bool parseKey1(const std::string& str, unsigned long long& result) {
        if (str.empty()) return false;

        std::string num = trim(str);
        if (num.empty()) return false;

        // Убираем суффикс
        if (num.back() == 'd') {
            num.pop_back();
        }
        else if (num.size() >= 2 && num.substr(num.size() - 2) == "ll") {
            num = num.substr(0, num.size() - 2);
        }
        else if (num.size() >= 3 && num.substr(num.size() - 3) == "ull") {
            num = num.substr(0, num.size() - 3);
        }

        // Проверка на hex (0x...)
        if (num.size() >= 2 && num[0] == '0' && (num[1] == 'x' || num[1] == 'X')) {
            return !!(std::istringstream(num) >> std::hex >> result);
        }

        // Проверка на binary (0b...)
        if (num.size() >= 2 && num[0] == '0' && num[1] == 'b') {
            result = 0;
            for (size_t i = 2; i < num.size(); i++) {
                if (num[i] == '0') result = result * 2;
                else if (num[i] == '1') result = result * 2 + 1;
                else return false;
            }
            return true;
        }

        // Обычное число
        std::istringstream iss(num);
        iss >> result;
        return !iss.fail();
    }

    // Парсинг key2 (комплексное число или обычное)
    bool parseKey2(const std::string& str, std::complex<double>& result) {
        std::string val = trim(str);
        if (val.empty()) return false;

        // Комплексное число #c(real imag)
        if (!val.empty() && val[0] == '#') {
            size_t open = val.find('(');
            size_t close = val.find(')');
            if (open != std::string::npos && close != std::string::npos) {
                std::string nums = val.substr(open + 1, close - open - 1);
                std::istringstream iss(nums);
                double real, imag;
                if (iss >> real >> imag) {
                    result = std::complex<double>(real, imag);
                    return true;
                }
            }
            return false;
        }

        // Обычное число с суффиксом
        std::string num = val;
        if (num.back() == 'd') num.pop_back();

        // Научная нотация (1.0e-1)
        std::istringstream iss(num);
        double dval;
        if (iss >> dval) {
            result = std::complex<double>(dval, 0.0);
            return true;
        }

        return false;
    }

    std::string extractValue(const std::string& line, const std::string& key) {
        std::string search = ":" + key + ":";
        size_t pos = line.find(search);
        if (pos == std::string::npos) return "";
        pos += search.length();

        // Значение до следующего ':'
        size_t end = line.find(':', pos);
        if (end == std::string::npos) return "";

        return line.substr(pos, end - pos);
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        std::string line;
        while (std::getline(in, line)) {
            // Убираем \r в конце
            if (!line.empty() && line.back() == '\r') line.pop_back();

            // Проверяем формат: начинается с '(', заканчивается на ')'
            line = trim(line);
            if (line.size() < 4 || line[0] != '(' || line.back() != ')') continue;

            // Извлекаем значения
            std::string v1 = extractValue(line, "key1");
            std::string v2 = extractValue(line, "key2");
            std::string v3 = extractValue(line, "key3");

            if (v1.empty() || v2.empty() || v3.empty()) continue;

            // Парсим key1
            unsigned long long val1;
            if (!parseKey1(v1, val1)) continue;

            // Парсим key2
            std::complex<double> val2;
            if (!parseKey2(v2, val2)) continue;

            // Парсим key3 (строка в кавычках)
            v3 = trim(v3);
            if (v3.size() < 2 || v3.front() != '"' || v3.back() != '"') continue;

            dest.key1 = val1;
            dest.key2 = val2;
            dest.key3 = v3.substr(1, v3.size() - 2);
            return in;
        }

        in.setstate(std::ios::failbit);
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;
        iofmtguard fmtguard(out);

        out << "(:key1 " << src.key1;

        // Вывод key2
        if (std::abs(src.key2.imag()) < EPS) {
            out << ":key2 " << static_cast<long long>(src.key2.real()) << "d";
        }
        else {
            out << ":key2 #c(" << std::fixed << std::setprecision(1)
                << src.key2.real() << " " << src.key2.imag() << ")";
        }

        out << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        double modA = std::abs(a.key2);
        double modB = std::abs(b.key2);
        if (std::abs(modA - modB) > EPS) return modA < modB;
        return a.key3.length() < b.key3.length();
    }
}

int main() {
    std::vector<nspace::DataStruct> vec;

    std::copy(std::istream_iterator<nspace::DataStruct>(std::cin),
        std::istream_iterator<nspace::DataStruct>(),
        std::back_inserter(vec));

    if (vec.empty()) {
        std::cerr << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
        return 1;
    }

    std::sort(vec.begin(), vec.end(), nspace::compareDataStruct);

    std::copy(vec.begin(), vec.end(),
        std::ostream_iterator<nspace::DataStruct>(std::cout, "\n"));

    return 0;
}
