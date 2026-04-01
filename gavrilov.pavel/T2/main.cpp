#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <cmath>
#include <regex>
#include <iomanip>

namespace nspace
{
    struct DataStruct
    {
        double key1;
        std::pair<long long, unsigned long long> key2;
        std::string key3;
    };

    struct DelimiterIO
    {
        char exp;
    };

    struct DoubleLitIO
    {
        double& ref;
    };

    struct RationalIO
    {
        std::pair<long long, unsigned long long>& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LabelIO
    {
        std::string exp;
    };

    class iofmtguard
    {
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
    std::istream& operator>>(std::istream& in, DoubleLitIO&& dest);
    std::istream& operator>>(std::istream& in, RationalIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& src);
}

nspace::iofmtguard::iofmtguard(std::basic_ios<char>& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{
}

nspace::iofmtguard::~iofmtguard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}

std::istream& nspace::operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& nspace::operator>>(std::istream& in, DoubleLitIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    double value;
    char suffix;
    in >> value >> suffix;
    if (in && (suffix == 'd' || suffix == 'D'))
    {
        dest.ref = value;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& nspace::operator>>(std::istream& in, RationalIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    long long numerator;
    unsigned long long denominator;
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' };
    in >> numerator;
    in >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' };
    in >> denominator;
    in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    if (in && denominator != 0)
    {
        dest.ref = std::make_pair(numerator, denominator);
    }
    else
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& nspace::operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& nspace::operator>>(std::istream& in, LabelIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    std::string data;
    if ((in >> StringIO{ data }) && (data != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& nspace::operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    DataStruct input;
    bool key1Set = false;
    bool key2Set = false;
    bool key3Set = false;

    in >> DelimiterIO{ '(' };

    while (in)
    {
        char c;
        in >> c;
        if (c == ')')
        {
            break;
        }
        in.putback(c);

        std::string fieldName;
        in >> StringIO{ fieldName };

        if (fieldName == "key1")
        {
            in >> DelimiterIO{ ' ' } >> DoubleLitIO{ input.key1 };
            key1Set = true;
        }
        else if (fieldName == "key2")
        {
            in >> DelimiterIO{ ' ' } >> RationalIO{ input.key2 };
            key2Set = true;
        }
        else if (fieldName == "key3")
        {
            in >> DelimiterIO{ ' ' } >> StringIO{ input.key3 };
            key3Set = true;
        }
        else
        {
            in.setstate(std::ios::failbit);
            break;
        }
    }

    if (in && key1Set && key2Set && key3Set)
    {
        dest = std::move(input);
    }
    else
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::ostream& nspace::operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    nspace::iofmtguard fmtguard(out);

    out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
    out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
    out << ":key3 \"" << src.key3 << "\":)";
    return out;
}

int main()
{
    using nspace::DataStruct;

    std::vector<DataStruct> dataVector;

    std::istream_iterator<DataStruct> inputBegin(std::cin);
    std::istream_iterator<DataStruct> inputEnd;
    std::copy(inputBegin, inputEnd, std::back_inserter(dataVector));

    std::sort(dataVector.begin(), dataVector.end(),
        [](const DataStruct& a, const DataStruct& b) {
            if (a.key1 != b.key1) {
                return a.key1 < b.key1;
            }
            double aKey2Value = static_cast<double>(a.key2.first) / a.key2.second;
            double bKey2Value = static_cast<double>(b.key2.first) / b.key2.second;
            if (std::abs(aKey2Value - bKey2Value) > 1e-12) {
                return aKey2Value < bKey2Value;
            }
            return a.key3.length() < b.key3.length();
        });

    std::ostream_iterator<DataStruct> outputBegin(std::cout, "\n");
    std::copy(dataVector.begin(), dataVector.end(), outputBegin);

    return 0;
}
