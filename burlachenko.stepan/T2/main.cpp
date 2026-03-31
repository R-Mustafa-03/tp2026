#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>

// var 14

struct DataStruct
{
    double key1;
    std::pair<long long, unsigned long long> key2;
    std::string key3;
};

static void skipSpaces(std::istream& is)
{
    char c;
    while (is.peek() == ' ' || is.peek() == '\t')
    {
        is.get(c);
    }
}


std::istream& operator>>(std::istream& is, DataStruct& ds)
{
    char c;
    while (is.get(c))
    {
        if (c == '(' && is.peek() == ':')
        {
            is.get();
            break;
        }
    }

    if (!is)
    {
        return is;
    }

    bool gotKey1 = false, gotKey2 = false, gotKey3 = false;

    while (is)
    {
        std::string fieldName;
        while (is.get(c))
        {
            if (c == ' ')
            {
                break;
            }
            fieldName += c;
        }

        if (fieldName == ")")
        {
            break;
        }

        if (fieldName == "key1")
        {
            std::string token;
            while (is.get(c))
            {
                if (c == ':') break;
                token += c;
            }

            if (!token.empty() && (token.back() == 'd' || token.back() == 'D'))
            {
                token.pop_back();
            }

            std::istringstream ss(token);
            double val;
            if (!(ss >> val))
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            if (token.find('.') == std::string::npos)
            {
                is.setstate(std::ios::failbit);
                return is;
            }
            ds.key1 = val;
            gotKey1 = true;
        }
        else if (fieldName == "key2")
        {

            if (!is.get(c) || c != '(')
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            if (!is.get(c) || c != ':')
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            std::string nLabel;
            while (is.get(c))
            {
                if (c == ' ') break;
                nLabel += c;
            }

            if (nLabel != "N")
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            std::string numStr;
            while (is.get(c))
            {
                if (c == ':') break;
                numStr += c;
            }

            long long num;
            std::istringstream ss1(numStr);
            if (!(ss1 >> num))
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            std::string dLabel;
            while (is.get(c))
            {
                if (c == ' ') break;
                dLabel += c;
            }

            if (dLabel != "D")
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            std::string denStr;
            while (is.get(c))
            {
                if (c == ':') break;
                denStr += c;
            }
            unsigned long long den;
            std::istringstream ss2(denStr);
            if (!(ss2 >> den))
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            if (!is.get(c) || c != ')')
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            if (!is.get(c) || c != ':')
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            ds.key2 = {num, den};
            gotKey2 = true;
        }
        else if (fieldName == "key3")
        {
            std::string token;
            while (is.get(c))
            {
                if (c == ':') break;
                token += c;
            }
            if (token.size() < 2 || token.front() != '"' || token.back() != '"')
            {
                is.setstate(std::ios::failbit);
                return is;
            }
            ds.key3 = token.substr(1, token.size() - 2);
            gotKey3 = true;
        }
        else
        {
            while (is.get(c))
            {
                if (c == ':') break;
            }
        }

        if (is.peek() == ')')
        {
            is.get();
            break;
        }
    }

    if (!gotKey1 || !gotKey2 || !gotKey3)
    {
        is.setstate(std::ios::failbit);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const DataStruct& ds)
{
    os << "(:key1 "
       << std::fixed << std::setprecision(1) << ds.key1 << "d:"
       << "key2 (:N " << ds.key2.first << ":D " << ds.key2.second << ":):"
       << "key3 \"" << ds.key3 << "\":)";
    return os;
}

bool compare(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1)
    {
        return a.key1 < b.key1;
    }

    double ra = (a.key2.second != 0) ? static_cast<double>(a.key2.first) / a.key2.second : 0.0;
    double rb = (b.key2.second != 0) ? static_cast<double>(b.key2.first) / b.key2.second : 0.0;
    if (ra != rb)
    {
        return ra < rb;
    }

    return a.key3.length() < b.key3.length();
}

int main()
{
    std::vector<DataStruct> data;


    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), compare);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}

