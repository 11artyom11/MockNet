#ifndef __DEBUG_HELPER__
#define __DEBUG_HELPER__
#include <stdio.h>
#include <time.h>

#include <iostream>

#include "../constants.h"

#ifndef _COLORS_
#define _COLORS_
/* FOREGROUND */
#define RST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#endif /* _COLORS_ */

class Debug;

#ifdef ENABLE_DEBUG
  #define debug_i_stream(stream, args...) Debug::info(stream, args)
  #define debug_w_stream(stream, args...) Debug::warning(stream, args)
  #define debug_f_stream(stream, args...) Debug::fatal(stream, args)
  #define debug_r_stream(stream, args...) Debug::fatal(stream, args)
#else 
  #define debug_i_stream(stream, args...) /* Blank */
  #define debug_w_stream(stream, args...) /* Blank */
  #define debug_f_stream(stream, args...) /* Blank */
  #define debug_r_stream(stream, args...) /* Blank */

#endif 

#define debug_i_console(args...) debug_i_stream(std::cout, args) 
#define debug_w_console(args...) debug_w_stream(std::cout, args)
#define debug_f_console(args...) debug_f_stream(std::cout, args)
#define debug_r_console(args...) debug_r_stream(std::cout, args)


/*Functional class to provide debug interface*/
class Debug {
 public:
  Debug() = delete;
  ~Debug() = delete;
  template <typename... mesTL>
  static void info( std::ostream& out, mesTL... messages);

  template <typename... mesTL>
  static void warning(std::ostream& out, mesTL... messages);

  template <typename... mesTL>
  static void fatal(std::ostream& out, mesTL... messages);

  template <typename... mesTL>
  static void raw(std::ostream& out, mesTL... messages);

  static std::string get_current_time(void) noexcept;

};

template <typename... mesTL>
void Debug::info(std::ostream& out, mesTL... messages) {
    out << KGRN /*Green code*/ << get_current_time() << INFO_S;
  ((out  << messages << ' '), ...);
    out  << KWHT /*White code*/ << std::endl;
}

template <typename... mesTL>
void Debug::warning(std::ostream& out, mesTL... messages) {
    out << KYEL << get_current_time() << WARNING_S;
    ((out << messages << ' '), ...);
    out << KWHT << std::endl;
}

template <typename... mesTL>
void Debug::fatal(std::ostream& out, mesTL... messages) {
    out << KRED << get_current_time() << ERROR_S;
    ((out << messages << ' '), ...);
    out << KWHT << std::endl;
}

template <typename... mesTL>
void Debug::raw(std::ostream& out, mesTL... messages) {
    ((out << messages << ' '), ...);
    out << '\n';
}

#endif