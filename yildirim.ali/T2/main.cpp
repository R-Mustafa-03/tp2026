#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <iomanip>
#include <limits>

struct DataStruct {
    double key1;
    char key2;
    std::string key3;
};
struct DelimiterIO {
    char exp;
};
struct DoubleIO {
    double& ref;
};
struct CharIO {
    char& ref;
};
struct StringIO {
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
std::istream& operator>>(std::istream& in, DoubleIO&& dest);
std::istream& operator>>(std::istream& in, CharIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
bool compare(const DataStruct& a, const DataStruct& b);

iofmtguard::iofmtguard(std::basic_ios<char>& s)
    : s_(s), width_(s.width()), fill_(s.fill()),
      precision_(s.precision()), fmt_(s.flags()) {
}
iofmtguard::~iofmtguard() {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}
std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char c;
    in >> c;
    if (in && c != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}
std::istream& operator>>(std::istream& in, DoubleIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    in >> dest.ref;
    if (in) {
        char suffix;
        in >> suffix;
        if (suffix != 'd' && suffix != 'D') {
            in.setstate(std::ios::failbit);
        }
    }
    return in;
}
std::istream& operator>>(std::istream& in, CharIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char quote;
    in >> quote;
    if (quote != '\'') {
        in.setstate(std::ios::failbit);
        return in;
    }
    in >> dest.ref;
    in >> quote;
    if (quote != '\'') {
        in.setstate(std::ios::failbit);
    }
    return in;
}
std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char quote;
    in >> quote;
    if (quote != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::getline(in, dest.ref, '"');
    return in;
}
std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct temp;

    if (!(in >> DelimiterIO{'('} >> DelimiterIO{':'})) {
        in.setstate(std::ios::failbit);
        return in;
    }
    bool has_key1 = false;
    bool has_key2 = false;
    bool has_key3 = false;

    for (int i = 0; i < 3; ++i) {
        std::string key;
        if (!(in >> key)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        if (key == "key1") {
            in >> DoubleIO{temp.key1};
            has_key1 = true;
        } else if (key == "key2") {
            in >> CharIO{temp.key2};
            has_key2 = true;
        } else if (key == "key3") {
            in >> StringIO{temp.key3};
            has_key3 = true;
        } else {
            in.setstate(std::ios::failbit);
            return in;
        }
        in >> DelimiterIO{':'};
    }
    in >> DelimiterIO{':'} >> DelimiterIO{')'};

    if (has_key1 && has_key2 && has_key3) {
        dest = temp;
        in.clear();
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}
std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) return out;

    iofmtguard fmtguard(out);

    out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
    out << ":key2 '" << src.key2 << "'";
    out << ":key3 \"" << src.key3 << "\":)";

    return out;
}
bool compare(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2) {
        return a.key2 < b.key2;
    }
    return a.key3.length() < b.key3.length();
}
int main() {
    std::vector<DataStruct> data;

    while (std::cin) {
        DataStruct ds;
        if (std::cin >> ds) {
            data.push_back(ds);
        } else if (!std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    std::sort(data.begin(), data.end(), compare);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
    return 0;
}
//
