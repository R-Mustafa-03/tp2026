#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <complex>
#include <cmath>

struct DataStruct
{
    long long key1;
    std::complex<double> key2;
    std::string key3;
};

struct DelimiterIO
{
    char exp;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }
    char c = '0';
    in.get(c);
    if (in && c != dest.exp)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

struct SLLLiteralIO
{
    long long& ref;
};

std::istream& operator>>(std::istream& in, SLLLiteralIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    std::string token;
    char c;
    while (in.get(c) && c != ':' && c != ')' && !std::isspace(c))
    {
        token.push_back(c);
    }
    in.unget();

    if (token.empty())
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (token.size() >= 2 && (token.substr(token.size() - 2) == "LL" || token.substr(token.size() - 2) == "ll"))
    {
        token = token.substr(0, token.size() - 2);
    }

    size_t start = 0;
    if (token[0] == '-')
    {
        start = 1;
    }
    for (size_t i = start; i < token.size(); i++)
    {
        if (!std::isdigit(static_cast<unsigned char>(token[i])))
        {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    try
    {
        dest.ref = std::stoll(token);
    }
    catch (...)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

struct ComplexIO
{
    std::complex<double>& ref;
};

std::istream& operator>>(std::istream& in, ComplexIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    std::string token;
    char c;
    while (in.get(c) && c != ':' && c != ')' && !std::isspace(c))
    {
        token.push_back(c);
    }
    in.unget();

    if (token.size() < 6)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (token[0] != '#' || token[1] != 'c' || token[2] != '(' || token.back() != ')')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string inside = token.substr(3, token.size() - 4);
    std::stringstream ss(inside);
    double real, imag;
    
    if (!(ss >> real >> imag))
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    
    dest.ref = std::complex<double>(real, imag);
    return in;
}

struct StringIO
{
    std::string& ref;
};

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    char c;
    in.get(c);
    if (c != '"')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    dest.ref.clear();
    while (in.get(c) && c != '"')
    {
        dest.ref.push_back(c);
    }

    if (!in)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    return in;
}

struct IdentifierIO
{
    std::string& ref;
};

std::istream& operator>>(std::istream& in, IdentifierIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    dest.ref.clear();
    char c = '0';
    while (in.get(c) && (std::isalpha(static_cast<unsigned char>(c)) ||
        std::isdigit(static_cast<unsigned char>(c))))
    {
        dest.ref.push_back(c);
    }

    if (dest.ref.empty())
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    while (std::isspace(in.peek()))
    {
        in.get();
    }

    if (in.peek() != '(')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    DataStruct temp;
    bool key1_set = false;
    bool key2_set = false;
    bool key3_set = false;

    in >> DelimiterIO{ '(' };
    if (!in)
    {
        return in;
    }

    while (in && in.peek() != ')')
    {
        if (in.peek() == ':')
        {
            in.get();
        }

        std::string field_name;
        in >> IdentifierIO{ field_name };
        if (!in)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (in.peek() == ' ')
        {
            in.get();
        }

        if (field_name == "key1" && !key1_set)
        {
            in >> SLLLiteralIO{ temp.key1 };
            key1_set = true;
        }
        else if (field_name == "key2" && !key2_set)
        {
            in >> ComplexIO{ temp.key2 };
            key2_set = true;
        }
        else if (field_name == "key3" && !key3_set)
        {
            in >> StringIO{ temp.key3 };
            key3_set = true;
        }
        else
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (!in)
        {
            return in;
        }

        if (in.peek() == ':')
        {
            in.get();
        }
        else if (in.peek() != ')')
        {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    in >> DelimiterIO{ ')' };

    if (in && key1_set && key2_set && key3_set)
    {
        dest = temp;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    out << "(:key1 " << src.key1;
    out << ":key2 #c(" << src.key2.real() << " " << src.key2.imag() << ")";
    out << ":key3 \"" << src.key3 << "\":)";

    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1)
    {
        return a.key1 < b.key1;
    }
    double modA = std::abs(a.key2);
    double modB = std::abs(b.key2);
    if (modA != modB)
    {
        return modA < modB;
    }
    return a.key3.length() < b.key3.length();
}

int main()
{
    std::vector<DataStruct> data;
    std::string line;

    while (std::getline(std::cin, line))
    {
        bool onlyWhitespace = true;
        for (char c : line)
        {
            if (!std::isspace(static_cast<unsigned char>(c)))
            {
                onlyWhitespace = false;
                break;
            }
        }
        if (onlyWhitespace)
        {
            continue;
        }

        std::istringstream lineStream(line);
        std::copy(
            std::istream_iterator<DataStruct>(lineStream),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );
    }

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
//
