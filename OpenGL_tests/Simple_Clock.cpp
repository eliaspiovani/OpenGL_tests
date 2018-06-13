#include "Simple_Clock.hpp"

#define PI 3.1415

void Simple_Clock::calcCircle(float radius, float color, std::vector<glm::vec3> &buffer, std::vector<glm::vec3> &colorB)
{
    float _x, _y, _z;
    buffer.clear();
    colorB.clear();
    
    buffer.push_back({x, y, 0.0f});
    colorB.push_back({color - 0.1, color - 0.1, color - 0.1});

    // Calculate the vertices for the circle and add them to the vector
    for (float theta = 0, i = 0; theta <= 360.0f; theta += 360/30.0f, i++) {
        if (i > 0) {
            buffer.push_back({x, y, 0.0f});
            colorB.push_back({color - 0.1, color - 0.1, color - 0.1});
            buffer.push_back({_x, _y, 0.0f});
            colorB.push_back({color, color, color});

        }
        _x = x + radius * cos(theta * (PI/180));
        _y = y + radius * sin(theta * (PI/180));
        _z = 0.0f;
        colorB.push_back({color, color, color});
        buffer.push_back({_x, _y, 0.0f});
    }
    buffer.push_back({x, y, 0.0f});
    colorB.push_back({color - 0.1, color - 0.1, color - 0.1});

    // For the color create a vector of the same size as vertices but with the color value
//    for (int i = 0; i < buffer.size(); i++)
//        colorB.push_back({color, color, color});
}

void Simple_Clock::drawToScreen(const std::vector<glm::vec3> &buffer, const std::vector<glm::vec3> &colorB){
    
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec3), &buffer[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &cb);
    glBindBuffer(GL_ARRAY_BUFFER, cb);
    glBufferData(GL_ARRAY_BUFFER, colorB.size() * sizeof(glm::vec3), &colorB[0], GL_STATIC_DRAW);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(
                          0,                  // attribute 0.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );
    
    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, cb);
    glVertexAttribPointer(
                          1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                          3,                                // size
                          GL_FLOAT,                         // type
                          GL_FALSE,                         // normalized?
                          0,                                // stride
                          (void*)0                          // array buffer offset
                          );
    
    // Draw the triangle !
    glPointSize(2.0f);
    glDrawArrays(GL_TRIANGLES, 0, (int)buffer.size());
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
    glDeleteBuffers(1, &vb);
    glDeleteBuffers(1, &cb);
}

void Simple_Clock::update()
{
    // same the new value for sec
    sec = (int)glfwGetTime()%60;
    sec_line.angle = sec * 6.0f * (PI/180.0f); // calculate new angle
    
    createBuffer(sec_line); // create buffer for line
    
    if (sec == 0) { // if sec is zero, add a value in min
        min++;
        
        if (min > 59) { // if min is greater than 59, set it to zero and add one to hour
            min = 0;
            hour++;
            if (hour > 23) // if hour is greater than 23, reset it to 0
                hour = 0;
        }
        hour_line.angle = (hour + min/60.0f) * 30.0f * (PI/180.0f); // calculate angle
        createBuffer(hour_line); // create buffer for line
    }
        
    min_line.angle = (min + sec/60.0f) * 6.0f * (PI/180.0f); // calculate angle
    createBuffer(min_line); // create buffer for lin
    
//    printf("%d:%d:%d\n", hour, min, (int)glfwGetTime()%60); // print on the console the actual values
}

void Simple_Clock::createBuffer(Line &line)
{
    // create a small offset for the pointer, to not be a thin line
    float offset_x = x + line.radius * sin(line.angle + 0.05f);
    float offset_y = y + line.radius * cos(line.angle + 0.05f);
    
    line.vertexbuffer[0] = {x, y, 0.0f};
    line.vertexbuffer[1] = {offset_x, offset_y, 0.0f};
    line.vertexbuffer[2] = {x + line.radius * sin(line.angle), y + line.radius * cos(line.angle), 0.0f};
}

void Simple_Clock::draw()
{
    // if a second has passed then call update
    if (this->sec != (int)glfwGetTime() % 60) update();
    
    // Draw all the buffers to screen
    drawToScreen(circleBuffer, colorBuffer);
    drawToScreen(hour_line.vertexbuffer, hour_line.colorbuffer);
    drawToScreen(min_line.vertexbuffer, min_line.colorbuffer);
    drawToScreen(sec_line.vertexbuffer, sec_line.colorbuffer);
    drawToScreen(innerCircleB, innerCircleCB);
}

Simple_Clock::Simple_Clock(float x, float y, float radius)
{
    // get x, y, and radius for this clock
    this->x = x;
    this->y = y;
    this->radius = radius;
    
    // calculate the angle, save the radius (min and hour has different radius size)
    // color vary from different pointers
    // and for the last part add a the values in a vector to later construct the buffer
    sec_line.angle = sec * 6.0f * (PI/180.0f);
    sec_line.radius = radius;
    sec_line.color = {0.7f, 0.7f, 0.7f};
    
    // create initial buffer for vertices
    sec_line.vertexbuffer.push_back({x , y, 0.0f});
    sec_line.vertexbuffer.push_back({});
    sec_line.vertexbuffer.push_back({});
    
    // create color  buffer for vertices
    sec_line.colorbuffer.push_back({0.2f, 0.2f, 0.2f});
    sec_line.colorbuffer.push_back({0.7f, 0.7f, 0.8f});
    sec_line.colorbuffer.push_back({0.7f, 0.7f, 0.8f});
    
    min_line.angle = (min + sec/60.0f) * 6.0f * (PI/180.0f);
    min_line.radius = radius * 0.9f;
    min_line.color = {1.0f, 1.0f, 0.0f};

    min_line.vertexbuffer.push_back({x, y, 0.0f});
    min_line.vertexbuffer.push_back({});
    min_line.vertexbuffer.push_back({});
    
    min_line.colorbuffer.push_back({1.0f, 0.5f, 0.0f});
    min_line.colorbuffer.push_back({1.0f, 1.0f, 0.0f});
    min_line.colorbuffer.push_back({1.0f, 1.0f, 0.0f});
    
    hour_line.angle = (hour + min/60.0f) * 30.0f * (PI/180.0f);
    hour_line.radius = radius * 0.8f;
    hour_line.color = {1.0f, 0.0f, 0.0f};
    
    hour_line.vertexbuffer.push_back({x, y, 0.0f});
    hour_line.vertexbuffer.push_back({});
    hour_line.vertexbuffer.push_back({});
    
    hour_line.colorbuffer.push_back({0.6f, 0.3f, 0.6f});
    hour_line.colorbuffer.push_back({1.0f, 0.0f, 0.0f});
    hour_line.colorbuffer.push_back({1.0f, 0.0f, 0.0f});
    
    // Create buffers for each clock pointer
    createBuffer(hour_line);
    createBuffer(min_line);
    createBuffer(sec_line);
    
    // Calculate both circles depending on the original position
    calcCircle(radius, 0.2f, circleBuffer, colorBuffer);
    calcCircle(radius*0.1f, 0.5f, innerCircleB, innerCircleCB);
    
}

void Simple_Clock::setPos(float x, float y) { 
    this->x = x;
    this->y = y;
    
    // Create buffers for each clock pointer
    createBuffer(hour_line);
    createBuffer(min_line);
    createBuffer(sec_line);
    
    // Calculate both circles depending on the original position
    calcCircle(radius, 0.2f, circleBuffer, colorBuffer);
    calcCircle(radius*0.1f, 0.5f, innerCircleB, innerCircleCB);
}

