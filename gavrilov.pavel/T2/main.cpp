#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cctype>

struct DataStruct
{
    double key1;
    std::pair<long long, unsigned long long> key2;
    std::string key3;
};

bool isDoubleLit(const std::string& s)
{
    if (s.empty()) return false;
    size_t pos = 0;
    try {
        std::stod(s, &pos);
    } catch (...) {
        return false;
    }
    if (pos + 1 != s.length()) return false;
    char suffix = s[pos];
    return (suffix == 'd' || suffix == 'D');
}

double parseDoubleLit(const std::string& s)
{
    return std::stod(s.substr(0, s.length() - 1));
}

bool isRational(const std::string& s)
{
    if (s.empty() || s.front() != '(' || s.back() != ')') return false;
    std::string inner = s.substr(1, s.length() - 2);
    try {
        if (inner.find(":N") == std::string::npos) return false;
        if (inner.find(":D") == std::string::npos) return false;
        size_t nPos = inner.find(":N") + 2;
        size_t dPos = inner.find(":D") + 2;
        std::stoll(inner.substr(nPos, inner.find(":", nPos) - nPos));
        unsigned long long den = std::stoull(inner.substr(dPos, inner.find(":", dPos) - dPos));
        return den != 0;
    } catch (...) {
        return false;
    }
}

std::pair<long long, unsigned long long> parseRational(const std::string& s)
{
    std::string inner = s.substr(1, s.length() - 2);
    size_t nPos = inner.find(":N") + 2;
    size_t dPos = inner.find(":D") + 2;
    long long num = std::stoll(inner.substr(nPos, inner.find(":", nPos) - nPos));
    unsigned long long den = std::stoull(inner.substr(dPos, inner.find(":", dPos) - dPos));
    return std::make_pair(num, den);
}

bool isQuotedString(const std::string& s)
{
    return s.length() >= 2 && s.front() == '"' && s.back() == '"';
}

std::string parseQuotedString(const std::string& s)
{
    return s.substr(1, s.length() - 2);
}

class InputStreamIterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = DataStruct;
    using difference_type = std::ptrdiff_t;
    using pointer = const DataStruct*;
    using reference = const DataStruct&;

    InputStreamIterator() : in_(nullptr), consumed_(true) {}
    explicit InputStreamIterator(std::istream& in) : in_(&in), consumed_(true)
    {
        ++(*this);
    }

    reference operator*() const { return value_; }
    pointer operator->() const { return &value_; }

    InputStreamIterator& operator++()
    {
        if (!in_) return *this;

        if (consumed_)
        {
            std::string line;
            while (std::getline(*in_, line))
            {
                if (line.empty()) continue;

                if (line.front() == '(' && line.back() == ')')
                {
                    std::string content = line.substr(1, line.length() - 2);

                    std::vector<std::string> parts;
                    std::string current;
                    int bracketDepth = 0;
                    bool inQuotes = false;

                    for (char c : content)
                    {
                        if (c == '"')
                        {
                            inQuotes = !inQuotes;
                            current += c;
                        }
                        else if (c == '(' && !inQuotes)
                        {
                            bracketDepth++;
                            current += c;
                        }
                        else if (c == ')' && !inQuotes)
                        {
                            bracketDepth--;
                            current += c;
                        }
                        else if (c == ':' && bracketDepth == 0 && !inQuotes)
                        {
                            if (!current.empty())
                            {
                                parts.push_back(current);
                                current.clear();
                            }
                        }
                        else
                        {
                            current += c;
                        }
                    }
                    if (!current.empty()) parts.push_back(current);

                    DataStruct result;
                    bool key1Ok = false, key2Ok = false, key3Ok = false;

                    for (const auto& part : parts)
                    {
                        size_t spacePos = part.find(' ');
                        if (spacePos == std::string::npos) continue;

                        std::string fieldName = part.substr(0, spacePos);
                        std::string fieldValue = part.substr(spacePos + 1);

                        if (fieldName == "key1" && isDoubleLit(fieldValue))
                        {
                            result.key1 = parseDoubleLit(fieldValue);
                            key1Ok = true;
                        }
                        else if (fieldName == "key2" && isRational(fieldValue))
                        {
                            result.key2 = parseRational(fieldValue);
                            key2Ok = true;
                        }
                        else if (fieldName == "key3" && isQuotedString(fieldValue))
                        {
                            result.key3 = parseQuotedString(fieldValue);
                            key3Ok = true;
                        }
                    }

                    if (key1Ok && key2Ok && key3Ok)
                    {
                        value_ = result;
                        consumed_ = false;
                        return *this;
                    }
                }
            }
            in_ = nullptr;
        }
        else
        {
            consumed_ = true;
            ++(*this);
        }
        return *this;
    }

    InputStreamIterator operator++(int)
    {
        InputStreamIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const InputStreamIterator& other) const
    {
        return in_ == other.in_;
    }

    bool operator!=(const InputStreamIterator& other) const
    {
        return !(*this == other);
    }

private:
    std::istream* in_;
    DataStruct value_;
    bool consumed_;
};

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
    out << "(:key1 " << std::fixed << std::setprecision(1) << data.key1 << "d";
    out << ":key2 (:N " << data.key2.first << ":D " << data.key2.second << ":)";
    out << ":key3 \"" << data.key3 << "\":)";
    return out;
}

int main()
{
    std::vector<DataStruct> dataVector;

    InputStreamIterator inputBegin(std::cin);
    InputStreamIterator inputEnd;
    std::copy(inputBegin, inputEnd, std::back_inserter(dataVector));

    std::sort(dataVector.begin(), dataVector.end(),
        [](const DataStruct& a, const DataStruct& b) {
            if (a.key1 != b.key1) return a.key1 < b.key1;
            double aVal = static_cast<double>(a.key2.first) / a.key2.second;
            double bVal = static_cast<double>(b.key2.first) / b.key2.second;
            if (std::abs(aVal - bVal) > 1e-12) return aVal < bVal;
            return a.key3.length() < b.key3.length();
        });

    std::ostream_iterator<DataStruct> outputBegin(std::cout, "\n");
    std::copy(dataVector.begin(), dataVector.end(), outputBegin);

    return 0;
}
