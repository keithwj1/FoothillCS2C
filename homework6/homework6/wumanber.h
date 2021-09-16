#pragma once
#include <vector>
#include <hash_map>

#include "finder.h"

#ifndef WUMANBER_H
#define WUMANBER_H

namespace thru {
    namespace matching {

        class WuManberFinder : public Finder {
        public:

            WuManberFinder(std::string const& text, std::vector<std::string> const& patterns);

            bool find();

            std::size_t position() const;

            std::size_t pattern_index() const;

        private:

            template <typename K, typename V>
            struct HashMap {
                typedef stdext::unordered_map<K, V> Type;
            };

            typedef HashMap<std::string, std::size_t>::Type shift_type;
            typedef HashMap<std::string, std::vector<std::size_t> >::Type hash_type;

            std::string const& m_text;
            std::vector<std::string> const& m_patterns;
            shift_type m_shift;
            hash_type m_hash;
            std::size_t m_pos;
            std::size_t m_find_pos;
            std::size_t m_find_pattern_index;
            std::size_t m_lmin;
            std::size_t m_lmax;
            std::size_t m_B;
        };

    }
} // namespace thru::matching

#endif // !defined(WUMANBER_H)