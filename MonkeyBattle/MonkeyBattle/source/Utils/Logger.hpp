#pragma once

#define LOG_GAME_INFO(fmtstr, ...) printf(fmtstr,##__VA_ARGS__)

#ifdef _DEBUG
#define LOG(s)	printf(s)
#define CLOG(fmts, ...) LOG_GAME_INFO(fmts,##__VA_ARGS__)
#else
#define LOG(s)
#define CLOG(fmts, ...)
#endif

