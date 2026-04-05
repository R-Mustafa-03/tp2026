#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <iomanip>
#include <complex>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cstdlib>

struct DataStruct {
    long long key1;                 
    std::complex<double> key2;     
    std::string key3;
};

struct DelimiterIO {
    char exp;
};

struct SllIO {
    long long& ref;
};

struct CmpIO {
    std::complex<double>& ref;
};

struct StrIO {
    std::string& ref;
};

class iofmtguard {
public:
    iofmtguard(std::basic_ios<char>& s);
    ~iofmtguard();
private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, SllIO&& dest);
std::istream& operator>>(std::istream& in, CmpIO&& dest);
std::istream& operator>>(std::istream& in, StrIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

bool compareDataStruct(const DataStruct& a, const DataStruct& b);

int main() {
    std::vector<DataStruct> data;

    while (std::cin) {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
                 );

        if (std::cin.fail() && !std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
             );

    return EXIT_SUCCESS;
}

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

std::istream& operator>>(std::istream& in, SllIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    in >> dest.ref;
    if (!in) {
        return in;
    }

    if (in.peek() == 'l' || in.peek() == 'L') {
        in.get();
        if (in.peek() == 'l' || in.peek() == 'L') {
            in.get();
        }
        else {
            in.setstate(std::ios::failbit);
        }
    }
    return in;
}

std::istream& operator>>(std::istream& in, CmpIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    double re = 0.0, im = 0.0;
    in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' }
    >> re >> im >> DelimiterIO{ ')' };
    if (in) {
        dest.ref = std::complex<double>(re, im);
    }
    return in;
}

std::istream& operator>>(std::istream& in, StrIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    if (in >> DelimiterIO{ '"' }) {
        std::getline(in, dest.ref, '"');
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    DataStruct input;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

    in >> DelimiterIO{ '(' };

    for (int i = 0; i < 3; ++i) {
        in >> DelimiterIO{ ':' };
        std::string label;
        in >> label;
        if (!in) {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (label == "key1" && !hasKey1) {
            in >> SllIO{ input.key1 };
            hasKey1 = true;
        }
        else if (label == "key2" && !hasKey2) {
            in >> CmpIO{ input.key2 };
            hasKey2 = true;
        }
        else if (label == "key3" && !hasKey3) {
            in >> StrIO{ input.key3 };
            hasKey3 = true;
        }
        else {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };

    if (in && hasKey1 && hasKey2 && hasKey3)
        dest = std::move(input);
    else
        in.setstate(std::ios::failbit);

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }

    iofmtguard guard(out);
    out << "(:key1 " << src.key1 << "ll"
        << ":key2 #c(" << std::fixed << std::setprecision(1)
        << src.key2.real() << " " << src.key2.imag() << ")"
        << ":key3 \"" << src.key3 << "\":)";
    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }

    double absA = std::abs(a.key2);
    double absB = std::abs(b.key2);
   
    if (std::abs(absA - absB) > 1e-9) {
        return absA < absB;
    }

    return a.key3.length() < b.key3.length();
}

iofmtguard::iofmtguard(std::basic_ios<char>& s)
    : s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags()) {
}

iofmtguard::~iofmtguard() {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}