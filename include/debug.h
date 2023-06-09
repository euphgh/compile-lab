/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#define ANSI_FG_BLACK   "\33[1;30m"
#define ANSI_FG_RED     "\33[1;31m"
#define ANSI_FG_GREEN   "\33[1;32m"
#define ANSI_FG_YELLOW  "\33[1;33m"
#define ANSI_FG_BLUE    "\33[1;34m"
#define ANSI_FG_MAGENTA "\33[1;35m"
#define ANSI_FG_CYAN    "\33[1;36m"
#define ANSI_FG_WHITE   "\33[1;37m"
#define ANSI_BG_BLACK   "\33[1;40m"
#define ANSI_BG_RED     "\33[1;41m"
#define ANSI_BG_GREEN   "\33[1;42m"
#define ANSI_BG_YELLOW  "\33[1;43m"
#define ANSI_BG_BLUE    "\33[1;44m"
#define ANSI_BG_MAGENTA "\33[1;35m"
#define ANSI_BG_CYAN    "\33[1;46m"
#define ANSI_BG_WHITE   "\33[1;47m"
#define ANSI_NONE       "\33[0m"
#define ASSERT_GDB 1
#define ANSI_FMT(str, fmt) fmt str ANSI_NONE
#include "macro.h"

#ifdef __cplusplus
#include <csignal>
#include <cassert>
#else 
#include <signal.h>
#include <assert.h>
#endif /* ifdef __cplusplus */

#ifdef C_FMT
#define _Log(...) IFONE(LOGOUT, printf(__VA_ARGS__));
#define Log(format, ...) \
    _Log(ANSI_FMT("[%s:%d %s] " format, ANSI_FG_BLUE) "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#define Assert(cond, fmtstr, ...) \
  do { \
      if (!(cond)) { \
          fflush(stdout); \
          fprintf(stderr, ANSI_FMT(fmtstr "\n", ANSI_FG_RED) , ## __VA_ARGS__); \
          IFDEF(ASSERT_GDB, raise(SIGTRAP), assert(cond)); \
    } \
  } while (0)
#else 
#define _Log(...) IFONE(LOGOUT, fmt::print(__VA_ARGS__));
#define Log(format, ...) \
    _Log(ANSI_FMT("[{:s}:{:d} {:s}] " format, ANSI_FG_BLUE) "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#define Assert(cond, fmtstr, ...) \
  do { \
      if (!(cond)) { \
          fflush(stdout); \
          fmt::print(stderr, ANSI_FMT(fmtstr "\n", ANSI_FG_RED) , ## __VA_ARGS__); \
          IFDEF(ASSERT_GDB, raise(SIGTRAP), assert(cond)); \
    } \
  } while (0)
#endif

#define panic(format, ...) Assert(0, format, ## __VA_ARGS__)

#define TODO() panic("please implement me")

#endif
