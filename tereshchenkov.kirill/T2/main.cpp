#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iomanip>

namespace nspace {
    struct DataStruct {
        double key1 = 0;
        unsigned long long key2 = 0;
        std::string key3;
    };

    struct DelimiterIO { char exp; };
    struct KeyIO { std::string& ref; };
    struct DoubleIO { double& ref; };
    struct BinUllIO { unsigned long long& ref; };
    struct StringIO { std::string& ref; };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        char c;
        in >> c;
        if (in && std::tolower(c) != std::tolower(dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, KeyIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        dest.ref.clear();
        char c;
        while (in >> c && std::isalnum(c)) {
            dest.ref += c;
            if (!std::isalnum(in.peek())) break;
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& dest) {
        if (!(in >> dest.ref)) return in;
        if (std::tolower(in.peek()) == 'd') in.get();
        return in;
    }

    std::istream& operator>>(std::istream& in, BinUllIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        std::string prefix(2, '\0');
        in >> prefix[0] >> prefix[1];
        if (prefix == "0b" || prefix == "0B") {
            std::string bits;
            while (std::isdigit(in.peek())) bits += static_cast<char> (in.get());
            try { dest.ref = std::stoull(bits, nullptr, 2); }
            catch (...) { in.setstate(std::ios::failbit); }
        } else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        char c;
        if (!(in >> c) || c != '"') return in.setstate(std::ios::failbit), in;
        std::getline(in, dest.ref, '"');
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        DataStruct temp;
        if (!(in >> DelimiterIO{'('} >> DelimiterIO{':'})) return in;

        for (int i = 0; i < 3; ++i) {
            std::string key;
            in >> KeyIO{key};
            if (key == "key1") in >> DoubleIO{temp.key1};
            else if (key == "key2") in >> BinUllIO{temp.key2};
            else if (key == "key3") in >> StringIO{temp.key3};
            else { in.setstate(std::ios::failbit); break; }
            in >> DelimiterIO{':'};
        }
        if (in >> DelimiterIO{')'}) dest = temp;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        out << "(:key1 " << std::scientific << std::setprecision(1) << std::lowercase << src.key1;
        out << ":key2 0b" << (src.key2 == 0 ? "0" : "");
        if (src.key2 > 0) {
            std::string b;
            for (unsigned long long n = src.key2; n > 0; n /= 2) b += (n % 2 ? '1' : '0');
            std::reverse(b.begin(), b.end());
            out << b;
        }
        out << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    bool compare_data(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
    }
}

int main() {
    std::vector<nspace::DataStruct> vec;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::copy(
            std::istream_iterator<nspace::DataStruct>(iss),
            std::istream_iterator<nspace::DataStruct>(),
            std::back_inserter(vec)
        );
    }

    std::sort(vec.begin(), vec.end(), nspace::compare_data);

    std::copy(
        vec.begin(),
        vec.end(),
        std::ostream_iterator<nspace::DataStruct>(std::cout, "\n")
    );

    return 0;
}
