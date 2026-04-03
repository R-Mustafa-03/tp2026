#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <limits>
#include <cctype>
#include <cmath>
#include <sstream>

struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
};

struct DelimiterIO {
    char expected;
};

struct DoubleScientificIO {
    double& value;
};

struct UnsignedBinaryIO {
    unsigned long long& value;
};

struct StringIO {
    std::string& value;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char c;
    in >> std::ws >> c;
    if (in && std::tolower(c) != std::tolower(dest.expected)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DoubleScientificIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    std::string token;
    if (!(in >> token)) return in;
    bool hasExp = (token.find('e') != std::string::npos || token.find('E') != std::string::npos);
    if (!hasExp) {
        in.setstate(std::ios::failbit);
        return in;
    }
    try {
        dest.value = std::stod(token);
    } catch (...) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, UnsignedBinaryIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char c1, c2;
    if (!(in >> c1 >> c2) || c1 != '0' || std::tolower(c2) != 'b') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string binaryDigits;
    char ch;
    while (in >> ch && (ch == '0' || ch == '1')) {
        binaryDigits += ch;
    }
    if (!binaryDigits.empty() || in) {
        in.putback(ch);
    }
    dest.value = binaryDigits.empty() ? 0 : std::stoull(binaryDigits, nullptr, 2);
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char quote;
    if (!(in >> quote) || quote != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::getline(in, dest.value, '"');
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    DataStruct input;
    if (!(in >> DelimiterIO{'('})) return in;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    while (in.good()) {
        DelimiterIO colon{':'};
        std::string label;
        if (!(in >> colon >> label)) break;
        if (label == "key1") {
            if (hasKey1) { in.setstate(std::ios::failbit); break; }
            in >> DoubleScientificIO{input.key1};
            hasKey1 = true;
        } else if (label == "key2") {
            if (hasKey2) { in.setstate(std::ios::failbit); break; }
            in >> UnsignedBinaryIO{input.key2};
            hasKey2 = true;
        } else if (label == "key3") {
            if (hasKey3) { in.setstate(std::ios::failbit); break; }
            in >> StringIO{input.key3};
            hasKey3 = true;
        } else {
            in.setstate(std::ios::failbit);
            break;
        }
        if (in.fail()) break;
    }
    if (hasKey1 && hasKey2 && hasKey3 && (in >> DelimiterIO{')'})) {
        dest = input;
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::string formatBinary(unsigned long long value) {
    if (value == 0) return "0";
    std::string result;
    while (value > 0) {
        result += (value & 1) ? '1' : '0';
        value >>= 1;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

struct iofmtguard {
    iofmtguard(std::ostream& s) : s_(s), fill_(s.fill()), precision_(s.precision()), fmt_(s.flags()) {}
    ~iofmtguard() { s_.fill(fill_); s_.precision(precision_); s_.flags(fmt_); }
private:
    std::ostream& s_;
    char fill_;
    std::streamsize precision_;
    std::ios::fmtflags fmt_;
};

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    iofmtguard guard(out);
    out << "(:key1 " << std::scientific << std::setprecision(1)
        << src.key1 << ":key2 0b" << formatBinary(src.key2)
        << ":key3 \"" << src.key3 << "\":)";
    return out;
}

bool compareData(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

int main() {
    std::vector<DataStruct> storage;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        DataStruct temp;
        if (iss >> temp) {
            storage.push_back(temp);
        }
    }
    std::sort(storage.begin(), storage.end(), compareData);
    std::copy(storage.begin(), storage.end(),
              std::ostream_iterator<DataStruct>(std::cout, "\n"));
    return 0;
}
