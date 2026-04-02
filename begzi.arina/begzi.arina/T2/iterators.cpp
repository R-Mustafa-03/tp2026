#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <limits>

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
    if (!sentry) {
        return in;
    }
    char c;
    in >> c;
    if (in && std::tolower(c) != std::tolower(dest.expected)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DoubleScientificIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    return in >> dest.value;
}

std::istream& operator>>(std::istream& in, UnsignedBinaryIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c1, c2;
    if (in >> c1 >> c2 && c1 == '0' && std::tolower(c2) == 'b') {
        std::string binaryRepresentation;
        while (std::isdigit(in.peek())) {
            binaryRepresentation += static_cast<char>(in.get());
        }
        if (!binaryRepresentation.empty()) {
            dest.value = std::stoull(binaryRepresentation, nullptr, 2);
        }
        else {
            in.setstate(std::ios::failbit);
        }
    }
    else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    if ((in >> DelimiterIO{ '"' })) {
        std::getline(in, dest.value, '"');
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct input;
    if (!(in >> DelimiterIO{ '(' })) return in;

    for (int i = 0; i < 3; ++i) {
        std::string label;
        in >> DelimiterIO{ ':' };

        char c;
        while (in >> c && std::isalpha(c) || std::isdigit(c)) {
            label += c;
            if (!std::isalnum(in.peek())) break;
        }

        if (label == "key1") in >> DoubleScientificIO{ input.key1 };
        else if (label == "key2") in >> UnsignedBinaryIO{ input.key2 };
        else if (label == "key3") in >> StringIO{ input.key3 };
        else in.setstate(std::ios::failbit);
    }

    if (in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' }) {
        dest = input;
    }
    return in;
}

std::string formatBinary(unsigned long long value) {
    if (value == 0) return "0";
    std::string result;
    while (value > 0) {
        result += (value % 2 == 0 ? '0' : '1');
        value /= 2;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }
    out << "(:key1 " << std::scientific << std::setprecision(1) << src.key1;
    out << ":key2 0b" << formatBinary(src.key2);
    out << ":key3 \"" << src.key3 << "\":)";
    return out;
}

bool compareData(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2) {
        return a.key2 < b.key2;
    }
    return a.key3.length() < b.key3.length();
}

int main() {
    std::vector<DataStruct> storage;

    while (!std::cin.eof()) {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(storage)
        );
        if (std::cin.fail() && !std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(storage.begin(), storage.end(), compareData);

    std::copy(
        storage.begin(),
        storage.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
