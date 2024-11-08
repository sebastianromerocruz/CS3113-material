#include "Scene.h"

class LevelA : public Scene {
public:
    static const int LEVEL_WIDTH  = 14;
    static const int LEVEL_HEIGHT = 8;
    
    ~LevelA();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
