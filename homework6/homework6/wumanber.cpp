#include <cmath>
#include <iostream>

#include "wumanber.h"

using namespace std;

namespace thru {
    namespace matching {

        WuManberFinder::WuManberFinder(string const& text, vector<string> const& patterns)
            : m_text(text)
            , m_patterns(patterns)
            , m_shift()
            , m_hash()
            , m_pos()
            , m_find_pos(0)
            , m_find_pattern_index(0)
            , m_lmin(m_patterns[0].size())
            , m_lmax(m_patterns[0].size())
            , m_B()
        {
            for (size_t i = 0; i < m_patterns.size(); ++i) {
                if (m_patterns[i].size() < m_lmin)
                    m_lmin = m_patterns[i].size();
                else if (m_patterns[i].size() > m_lmax)
                    m_lmax = m_patterns[i].size();
            }

            m_pos = m_lmin;
            m_B = static_cast<size_t>(ceil(log(2.0 * m_lmin * m_patterns.size()) / log(256.0)));

            for (size_t i = 0; i < m_patterns.size(); ++i)
                m_hash[m_patterns[i].substr(m_patterns[i].size() - m_B)].push_back(i);

            for (size_t i = 0; i < m_patterns.size(); ++i) {
                for (size_t j = 0; j < m_patterns[i].size() - m_B + 1; ++j) {
                    string bgram = m_patterns[i].substr(j, m_B);
                    size_t pos = m_patterns[i].size() - j - m_B;

                    shift_type::iterator old = m_shift.find(bgram);
                    if (old == m_shift.end())
                        m_shift[bgram] = pos;
                    else
                        old->second = min(old->second, pos);
                }
            }
        }

        bool WuManberFinder::find() {
            while (m_pos <= m_text.size()) {
                string bgram = m_text.substr(m_pos - m_B, m_B);
                shift_type::iterator i = m_shift.find(bgram);
                if (i == m_shift.end())
                    m_pos += m_lmin - m_B + 1;
                else {
                    if (i->second == 0) {
                        vector<size_t>& list = m_hash[bgram];
                        // Verify all patterns in list against the text.
                        ++m_pos;
                        for (size_t j = 0; j < list.size(); ++j) {
                            string const& str = m_patterns[list[j]];
                            m_find_pos = m_pos - str.size() - 1;
                            size_t k = 0;

                            for (; k < str.size(); ++k)
                                if (str[k] != m_text[m_find_pos + k])
                                    break;

                            if (k == str.size()) {
                                m_find_pattern_index = list[j];
                                return true;
                            }
                        }
                    }
                    else
                        m_pos += i->second;
                }
            }

            return false;
        }

        size_t WuManberFinder::position() const {
            return m_find_pos;
        }

        size_t WuManberFinder::pattern_index() const {
            return m_find_pattern_index;
        }

    }
} // namespace thru::matching