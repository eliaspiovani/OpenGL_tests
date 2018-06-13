#ifndef Simple_Clock_hpp
#define Simple_Clock_hpp

//#include <stdio.h>
//#include <math.h>
#include <vector>
//#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Simple_Clock{
private:
    float x, y;
    std::vector<glm::vec3> circleBuffer, colorBuffer, innerCircleB, innerCircleCB;
    GLuint vb, cb;
    
    struct Line{
        std::vector<glm::vec3> pos;
        float angle;
        float radius;
        glm::vec3 color = {1.0f, 1.0f, 1.0f};
        std::vector<glm::vec3> vertexbuffer, colorbuffer;
    };
    
    Line sec_line, min_line, hour_line;
    
    void calcCircle(float radius, float color, std::vector<glm::vec3> &buffer, std::vector<glm::vec3> &colorB);
    
    void drawToScreen(const std::vector<glm::vec3> &buffer, const std::vector<glm::vec3> &colorB);
    
    void update();
    
    void createBuffer(Line &line);
    
public:
    int sec = 1, min = 0, hour = 0;
    float radius = 1.0f;
    
    Simple_Clock(float x, float y, float radius);
    
    void draw();
    
    void setPos(float x, float y);

};

#endif /* Clock_hpp */
