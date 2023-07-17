#include "Scene.h"

class LevelA : public Scene {
public:
    int ENEMY_COUNT = 1;
    
    ~LevelA();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
