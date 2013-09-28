#import <GameKit/GameKit.h>
#import <DABGameCenterNetMatch.h>
#include "net_match.h"
#include "engine.h"

typedef struct GameCenterNetMatchCtx {
    CFTypeRef gcn_match;
} GameCenterNetMatchCtx;

int GameCenterNetMatch_init(NetMatch *match, Engine *engine) {
    GameCenterNetMatchCtx *ctx = calloc(1, sizeof(GameCenterNetMatchCtx));
    
    match->context = ctx;
    
    DABGameCenterNetMatch *gcn_match =
        [[DABGameCenterNetMatch alloc] initWithEngine:engine];
    ctx->gcn_match = CFBridgingRetain(gcn_match);
    return 1;
}

int GameCenterNetMatch_cleanup(NetMatch *match) {
    GameCenterNetMatchCtx *ctx = match->context;
    if (ctx->gcn_match) {
        CFBridgingRelease(ctx->gcn_match);
        ctx->gcn_match = NULL;
    }
    free(ctx);
    return 1;
}

int GameCenterNetMatch_associate_native(NetMatch *match, void *assoc) {
    // Our *assoc is a GKMatch in disguise!
    GKMatch *gk_match = (__bridge GKMatch *)assoc;
    
    GameCenterNetMatchCtx *ctx = match->context;
    DABGameCenterNetMatch *gcn_match =
        (__bridge DABGameCenterNetMatch *)ctx->gcn_match;
    
    gcn_match.gkMatch = gk_match;
    gcn_match.netMatch = match;
    
    return 1;
}

int GameCenterNetMatch_handshake(NetMatch *match, struct Engine *engine) {
    GameCenterNetMatchCtx *ctx = match->context;
    DABGameCenterNetMatch *gcn_match =
        (__bridge DABGameCenterNetMatch *)ctx->gcn_match;
    
    [gcn_match derivePlayerNumber];
    
    return 1;
}

int GameCenterNetMatch_all_ready_cb(NetMatch *match, struct Engine *engine) {
    Scripting_call_hook(engine->scripting, match, "all_ready");
    
    return 1;
}

int GameCenterNetMatch_get_player_count(NetMatch *match, struct Engine *engine) {
    GameCenterNetMatchCtx *ctx = match->context;
    DABGameCenterNetMatch *gcn_match =
        (__bridge DABGameCenterNetMatch *)ctx->gcn_match;
    
    return gcn_match.playerCount;
}

int GameCenterNetMatch_get_player_number(NetMatch *match, struct Engine *engine) {
    GameCenterNetMatchCtx *ctx = match->context;
    DABGameCenterNetMatch *gcn_match =
        (__bridge DABGameCenterNetMatch *)ctx->gcn_match;
    
    return gcn_match.localPlayerNumber;
}

int GameCenterNetMatch_send_msg(struct NetMatch *match, struct Engine *engine,
                                NetMatchMsg *msg) {
    GameCenterNetMatchCtx *ctx = match->context;
    DABGameCenterNetMatch *gcn_match =
        (__bridge DABGameCenterNetMatch *)ctx->gcn_match;
    return [gcn_match sendMsg:msg];
}

int GameCenterNetMatch_rcv_msg_cb(struct NetMatch *match, struct Engine *engine,
                                  NetMatchMsg *msg) {
    Scripting_call_hook(engine->scripting, match, "received_msg");
    
    return 1;
}

NetMatchProto GameCenterNetMatchProto = {
    .init = GameCenterNetMatch_init,
    .cleanup = GameCenterNetMatch_cleanup,
    .associate_native = GameCenterNetMatch_associate_native,
    .handshake = GameCenterNetMatch_handshake,
    .all_ready_cb = GameCenterNetMatch_all_ready_cb,
    .get_player_count = GameCenterNetMatch_get_player_count,
    .get_player_number = GameCenterNetMatch_get_player_number,
    .send_msg = GameCenterNetMatch_send_msg,
    .rcv_msg_cb = GameCenterNetMatch_rcv_msg_cb
};