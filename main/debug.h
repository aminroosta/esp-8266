#include <stdio.h>
#include <stdint.h>
#include <string.h>

inline auto log(char const *name, const char *v) {
    printf("%s :\t%s\n", name, v);
    return v;
}
inline auto log(char const *name, int32_t v) {
    printf("%s :\t%#010x\n", name, v);
    return v;
}

#define _NUM_ARGS(X,X9,X8,X7,X6,X5,X4,X3,X2,X1,N,...) N
#define NUM_ARGS(...) _NUM_ARGS(0, __VA_ARGS__ ,9,8,7,6,5,4,3,2,1,0)

#define dbg_1(a)      log(#a, a)
#define dbg_2(a, ...) log(#a, a), dbg_1(__VA_ARGS__)
#define dbg_3(a, ...) log(#a, a), dbg_2(__VA_ARGS__)
#define dbg_4(a, ...) log(#a, a), dbg_3(__VA_ARGS__)
#define dbg_5(a, ...) log(#a, a), dbg_4(__VA_ARGS__)
#define dbg_6(a, ...) log(#a, a), dbg_5(__VA_ARGS__)
#define dbg_7(a, ...) log(#a, a), dbg_6(__VA_ARGS__)
#define dbg_8(a, ...) log(#a, a), dbg_7(__VA_ARGS__)
#define dbg_9(a, ...) log(#a, a), dbg_8(__VA_ARGS__)

#define dbg__(N, ...) dbg_ ## N(__VA_ARGS__)
#define dbg_(N, ...)  dbg__(N, __VA_ARGS__)
#define dbg(...)      dbg_(NUM_ARGS(__VA_ARGS__), __VA_ARGS__)