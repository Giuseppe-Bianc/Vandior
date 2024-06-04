//
// Created by gbian on 04/06/2024.
//

#pragma once

#include "../headers.hpp"
#include "Nanollds.hpp"

class Times {
public:
    Clocks tseconds;
    Clocks tmillis;
    Clocks tmicros;
    Clocks tnanos;
    std::string seconds;
    std::string millis;
    std::string micros;
    std::string nanos;

    // Costruttore di default
    Times() = default;

    // Costruttore parametrizzato
    Times(Clocks s, Clocks ms, Clocks us, Clocks ns, std::string sec, std::string mil, std::string mic, std::string nan)
      : tseconds(s), tmillis(ms), tmicros(us), tnanos(ns), seconds(sec), millis(mil), micros(mic), nanos(nan) {}
    // Operatore di sottrazione
};