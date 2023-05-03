#include <scanner.hh>
#include <error.hh>

void fell::scan::number(
    scanned & data,
    const std::string_view candidate,
    const std::size_t line,
    const std::size_t column
) {
    std::size_t i = 0;
    var::integer ret{0};

    for(; i < candidate.length(); ++i) {
        if(candidate[i] == '.') break;

        if(!std::isdigit(candidate[i]))
            throw err::common{
                line,
                column,
                "Malformed number: " +
                std::string(candidate.data(), candidate.size())
            };

        ret = ret * 10 + candidate[i] - '0';
    }

    if(i == candidate.length()) {
        data.push_constant(ret, {line, column});
    } else {
        const auto j = i;
        ++i;

        var::number fl = static_cast<var::number>(ret);

        for(; i < candidate.length(); ++i) {
            if(!std::isdigit(candidate[i]))
                throw err::common{
                    line,
                    column,
                    "Malformed number: " +
                    std::string(candidate.data(), candidate.size())
                };

            fl += static_cast<decltype(fl)>(candidate[i] - '0') / std::pow(10, i - j);
        }

        data.push_constant(fl, {line, column});
    }
}
