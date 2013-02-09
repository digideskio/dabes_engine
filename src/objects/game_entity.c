#include "engine.h"
#include "game_entity.h"

int GameEntity_init(void *self) {
    check_mem(self);
    GameEntity *entity = (GameEntity *)self;
    entity->fixture = NULL;
    entity->alpha = 1.f;
    entity->texture = 0;

    return 1;

error:
    return 0;
}

void GameEntity_destroy(void *self) {
    check_mem(self);
    GameEntity *entity = (GameEntity *)self;
// TODO: Remove fixture
    free(entity);
    return;
error:
    return;
}

void GameEntity_render(GameEntity *self, void *engine) {
    GameEntity *entity = self;
    Graphics *graphics = ((Engine *)engine)->graphics;

    GfxRect rect = Fixture_display_rect(entity->fixture);
    GLdouble color[4] = {0.f, 0.f, 0.f, entity->alpha};
    float degrees = Fixture_rotation_degrees(entity->fixture);
    Graphics_draw_rect(graphics, rect, color, entity->texture, degrees);
}

Object GameEntityProto = {
    .destroy = GameEntity_destroy,
    .init = GameEntity_init,
};

