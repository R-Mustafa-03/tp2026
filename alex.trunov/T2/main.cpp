#include <algorithm>
#include <cctype>
#include <ios>
#include <istream>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <string>
#include <complex>
#include <iostream>
#include <vector>
#include <limits>


//(:key1 10ll:key2 #c(1.0 -1.0):key3 "Data":)
//(:key1 -5ll:key2 #c(3.0 4.0):key3 "Hello":)
//(3k)
//(:key1 10ll:key2 #c(1.0 -1.0):key3 "Short":)
//(:key1 10ll:key2 #c(0.0 0.0):key3 "Zero":)
//(:key2 #c(2.0 0.0):key3 "Reordered":key1 -5ll:)
//vkwbje12e
//(:key1 ll:key2 #c(1.0 1.0):key3 "Invalid":)
//(:key1 99ll:key2 #c(1.0 -1.0):key3 "Last":)


struct DataStruct {
    long long key1;
    std::complex<double> key2;
    std::string key3;
};

struct DelimiterIO {
    char exp;
};

struct LiteralIO {
    long long& ref;
};

struct ComplexIO {
    std::complex<double>& ref;
};

struct StringIO {
    std::string& ref;
};

struct LabelIO {
    std::string& ref;
};

// ========================================
std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, LiteralIO&& dest);
std::istream& operator>>(std::istream& in, ComplexIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, LabelIO&& dest);

std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
// ========================================

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    char c = '0';
    in >> c;

    if (in && c != dest.exp) {
        in.setstate(std::ios::failbit);
    }

    return in;
}
std::istream& operator>>(std::istream& in, LiteralIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string raw;
    if (in.peek() == '-' || in.peek() == '+') {
        raw += static_cast<char>(in.get());
    }

    while (std::isdigit(static_cast<unsigned char>(in.peek()))) {
        raw += static_cast<char>(in.get());
    }

    if (raw.empty() ||
        (raw.size() == 1 && !std::isdigit(static_cast<unsigned char>(raw[0])))) {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string suf;
    while (in.peek() != EOF &&
        !std::isspace(static_cast<unsigned char>(in.peek())) &&
        in.peek() != ':') {
        suf += static_cast<char>(in.get());
    }

    std::string sufLower;
    for (char c : suf) {
        sufLower += std::tolower(static_cast<unsigned char>(c));
    }

    if (sufLower != "ll") {
        in.setstate(std::ios::failbit);
    }

    try { dest.ref = std::stoll(raw); }
    catch (...) { in.setstate(std::ios::failbit); }

    return in;
}
std::istream& operator>>(std::istream& in, ComplexIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    double re, im;

    in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' } >> re;
    if (!in) {
        return in;
    }

    if (!std::isspace(in.peek())) {
        in.setstate(std::ios::failbit);
        return in;
    }
    in.get();

    in >> im >> DelimiterIO{ ')' };
    if (in) {
        dest.ref = std::complex<double>(re, im);
    }

    return in;
}
std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!in) {
        return in;
    }

    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}
std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    dest.ref.clear();

    char c = '0';
    while (in.get(c) && !std::isspace(static_cast<unsigned char>(c))) {
        dest.ref.push_back(c);
    }

    if (!in || dest.ref.empty()) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    DataStruct data;
    struct KeyStatus {
        bool key1 = false;
        bool key2 = false;
        bool key3 = false;

        bool allSet() { return key1 && key2 && key3; }
    };

    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
    if (!in) {
        return in;
    }

    KeyStatus keys;
    while (in && in.peek() != ')') {
        std::string fieldType;
        in >> LabelIO{ fieldType };
        if (!in) {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (fieldType == "key1" && !keys.key1) {
            in >> LiteralIO{ data.key1 };
            keys.key1 = true;
        }
        else if (fieldType == "key2" && !keys.key2) {
            in >> ComplexIO{ data.key2 };
            keys.key2 = true;
        }
        else if (fieldType == "key3" && !keys.key3) {
            in >> StringIO{ data.key3 };
            keys.key3 = true;
        }
        else {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (!in) {
            return in;
        }

        in >> DelimiterIO{ ':' };
    }

    in >> DelimiterIO{ ')' };

    if (in && keys.allSet()) {
        dest = std::move(data);
    }
    else {
        in.setstate(std::ios::failbit);
    }

    return in;
}
std::ostream& operator<<(std::ostream& out, const DataStruct& dest) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }

    out << std::fixed << std::setprecision(1)
        << "(:key1 " << dest.key1 << "ll"
        << ":key2 #c(" << dest.key2.real() << " " << dest.key2.imag() << ")"
        << ":key3 \"" << dest.key3 << "\":)";

    return out;
}

bool compareKeys(const DataStruct& data1, const DataStruct& data2) {
    if (data1.key1 != data2.key1) {
        return data1.key1 < data2.key1;
    }

    constexpr double EPSILON = 1e-9;
    if (std::abs(std::abs(data1.key2) - std::abs(data2.key2)) > EPSILON) {
        return std::abs(data1.key2) < std::abs(data2.key2);
    }

    return data1.key3.length() < data2.key3.length();
}

int main() {

    std::vector<DataStruct> data;
    std::string raw;

    while (std::getline(std::cin, raw)) {
        if (raw.empty()) {
            continue;
        }

        std::istringstream iss(raw);

        std::copy(
            std::istream_iterator<DataStruct>(iss),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );
    }

    std::sort(data.begin(), data.end(), compareKeys);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
