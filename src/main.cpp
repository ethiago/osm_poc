#include <iostream>

#include "gl.hpp"
#include "image_writer.hpp"

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
  
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 1, 0);
    glEnd();

    glFlush();
}

void configure()
{ 
    glClearColor(0.0, 0.0, 0.0, 1.0);
   
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, const char ** argv)
{
    using namespace osmpoc;
    Context<supported_formats::RGBA> context;

    auto attachedBuffer = attach_buffer(context, 512,512);

    configure();
    draw();
    glFinish();
    
    std::string filename("output.png");
    write(filename, attachedBuffer);

    std::cout << filename << std::endl;

    return 0;
}
