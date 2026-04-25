// 4 VAR
// DBL SCI
// CHR LIT
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>

const double EPS = 1e-9;

struct DataStruct
{
    double key1;
    char key2;
    std::string key3;
};

struct DelimiterIO
{
    char exp;
};

struct DoubleSciIO
{
    double& ref;
};

struct CharLitIO
{
    char& ref;
};

struct StringIO
{
    std::string& ref;
};

class Iofmtguard
{
public:
    Iofmtguard(std::basic_ios<char>& s);
    ~Iofmtguard();
private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, DoubleSciIO&& dest);
std::istream& operator>>(std::istream& in, CharLitIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
bool compareDataStruct(const DataStruct& a, const DataStruct& b);

int main()
{
    std::vector<DataStruct> data;
    std::string input;

    while (std::getline(std::cin, input))
    {
        if (input.empty()) continue;
        std::istringstream iss(input);
        DataStruct temp;
        if (iss >> temp) data.push_back(temp);
    }

    std::sort(data.begin(), data.end(), compareDataStruct);
    std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));
    return 0;
}

Iofmtguard::Iofmtguard(std::basic_ios<char>& s)
    : s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()), fmt_(s.flags()) {
}

Iofmtguard::~Iofmtguard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char c;
    in >> c;
    if (in && c != dest.exp) in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, CharLitIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    return in >> DelimiterIO{ '\'' } >> dest.ref >> DelimiterIO{ '\'' };
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, DoubleSciIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    if (in.peek() == '0')
    {
        std::streampos pos = in.tellg();
        std::string test;
        in >> test;
        if (test == "0.0" || test == "0")
        {
            dest.ref = 0.0;
            return in;
        }
        in.seekg(pos);
    }

    std::string num;
    char sign = '+';

    if (in.peek() == '-') in.get(sign);

    char c;
    while (in.get(c) && (std::isdigit(c) || c == '.')) num += c;

    if (c != 'e' && c != 'E')
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    num += c;

    if (in.peek() == '+' || in.peek() == '-')
    {
        in.get(c);
        num += c;
    }
    else
    {
        num += '+';
    }

    std::string exp;
    while (in.get(c) && std::isdigit(c)) exp += c;
    if (exp.empty())
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (c != std::char_traits<char>::eof()) in.putback(c);
    num += exp;

    try
    {
        dest.ref = std::stod(num);
        if (sign == '-') dest.ref = -dest.ref;
    }
    catch (...)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct temp;
    bool f1 = false, f2 = false, f3 = false;

    if (!(in >> DelimiterIO{ '(' }))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (size_t i = 0; i < 3; ++i)
    {
        if (!(in >> DelimiterIO{ ':' })) break;

        std::string label;
        if (!(in >> label)) break;

        if (label == "key1")
        {
            if (!(in >> DoubleSciIO{ temp.key1 })) break;
            f1 = true;
        }
        else if (label == "key2")
        {
            if (!(in >> CharLitIO{ temp.key2 })) break;
            f2 = true;
        }
        else if (label == "key3")
        {
            if (!(in >> StringIO{ temp.key3 })) break;
            f3 = true;
        }
        else
        {
            in.setstate(std::ios::failbit);
            break;
        }
    }

    if (!(in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' }) || !f1 || !f2 || !f3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    dest = std::move(temp);
    return in;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (std::abs(a.key1 - b.key1) > EPS) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

std::string formatCharLit(char c)
{
    switch (c)
    {
    case '\n': return "'\\n'";
    case '\t': return "'\\t'";
    case '\\': return "'\\\\'";
    case '\'': return "'\\''";
    default: return std::string("'") + c + "'";
    }
}


std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry) return out;

    Iofmtguard fmtguard(out);

    double mantissa = src.key1;
    int exponent = 0;

    if (std::abs(mantissa) < EPS)
    {
        out << "(:key1 0.0e+00";
    }
    else
    {
        bool negative = mantissa < 0;
        if (negative) mantissa = -mantissa;

        while (mantissa < 1.0 && exponent > -100)
        {
            mantissa *= 10.0;
            exponent--;
        }
        while (mantissa >= 10.0 && exponent < 100)
        {
            mantissa /= 10.0;
            exponent++;
        }

        if (negative) mantissa = -mantissa;

        out << "(:key1 " << std::fixed << std::setprecision(1) << mantissa;
        out << "e";
        if (exponent >= 0) out << "+";
        out << exponent;
    }
    out << ":key2 " << formatCharLit(src.key2)
        << ":key3 \"" << src.key3 << "\":)";
    return out;
}
