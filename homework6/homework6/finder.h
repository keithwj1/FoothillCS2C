#pragma once
#ifndef FINDER_H
#define FINDER_H

#include <string>

namespace thru {
    namespace matching {

        class Finder {
        public:
            virtual bool find() = 0;

            virtual std::size_t position() const = 0;

            virtual ~Finder() = 0;

        protected:
            static size_t code_from_chr(char c) {
                return static_cast<size_t>(static_cast<unsigned char>(c));
            }
        };

        inline Finder::~Finder() { }

    }
} // namespace thru::matching

#endif // !defined(FINDER_H)