#include "lexer.hh"

std::size_t fell::lex::check_for_string_constant(std::string & line) {
    std::size_t counter = 0;

    std::size_t offset = 0;
    bool found;

    while((offset = line.find_first_of('"', offset)) != std::string::npos) {
        found = false;
        for(std::size_t x = offset + 1; x < line.length(); ++x)
            if(line[x] == '"' && line[x - 1] != '\\') {
                found = true;
                ++counter;

                const std::string data = std::string{line.data() + offset + 1, x - offset - 1};
                line.erase(offset, x - offset + 1);

                const std::string name = "__string_constant_" + std::to_string(counter);
                line.insert(offset, name);

                (*lang::global_table)[name] = util::make_var<types::string>(data);
                break;
            }

        if(!found)
            throw std::runtime_error{"Unterminated string."};
    }

    return counter;
}

void fell::lex::clear_string_constants(std::size_t counter) {
    while(counter > 0) {
        const std::string name = "__string_constant_" + std::to_string(counter);
        util::get_value<types::table::tbl>(lang::global_table)->erase(name);
        --counter;
    }
}
