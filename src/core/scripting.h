#ifndef __scripting_h
#define __scripting_h
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
#include "../prefix.h"

typedef struct Scripting {
    lua_State *L;
} Scripting;

struct Engine;
Scripting *Scripting_create(struct Engine *engine, const char *boot_script);
void Scripting_destroy(Scripting *scripting);
int Scripting_test(Scripting *scripting);
void Scripting_register_engine(Scripting *scripting, struct Engine *engine);

struct Engine *luaL_get_engine(lua_State *L);

#define Scripting_bail(L, MSG) { \
    fprintf(stderr, "%s: %s\n", MSG, lua_tostring(L, -1)); \
    return 0; \
}

#define Scripting_num_setter(FNAME, MTABLE, UDTYPE, UDPROP, STYPE, SPROP) \
static inline int FNAME(lua_State *L) { \
    UDTYPE *ud = (UDTYPE *) luaL_checkudata(L, 1, MTABLE); \
    check(lua_isnumber(L, 2), \
            "Please provide a number to set "#STYPE"->"#SPROP); \
    lua_Number num = lua_tonumber(L, 2); \
    STYPE *s = ud->UDPROP; \
    printf("Setting ("#STYPE" %p)->"#SPROP": %f\n", s, num); \
    s->SPROP = num; \
    return 1; \
error: \
    return 0; \
}

#define Scripting_num_getter(FNAME, MTABLE, UDTYPE, UDPROP, STYPE, SPROP) \
static inline int FNAME(lua_State *L) { \
    UDTYPE *ud = (UDTYPE *) luaL_checkudata(L, 1, MTABLE); \
    STYPE *s = ud->UDPROP; \
    lua_Number ret = s->SPROP; \
    printf("Getting ("#STYPE" %p)->SPROP: %f\n", s, ret); \
    lua_pushinteger(L, ret); \
    return 1; \
}

extern const char *SCRIPTING_CL_ENTITY_CONFIG;
extern const char *SCRIPTING_CL_PARALLAX;
extern const char *SCRIPTING_CL_PARALLAX_LAYER;
extern const char *SCRIPTING_ENGINE_REGISTRY_KEY;

#endif
