#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <time.h>
#include <vector>
#include <random>

void init(); 
void loop(); 
void events();
void update(); 
void draw(); 
int sync(); 
void getCursorPos(int(&a)[2]);
void drawRect(const float x, const float y, const float w, const float h, const float, const float, const float);

GLFWwindow* window;

const int width = 800; // 8 * 300
const int height = 800; // 8 * 200


struct Particle {

    int futurePos[2] = { 0, 0 };

    short density = 0;

    bool solid = false; 

    std::string type = "none"; 

    Particle(std::string type, short density, bool solid) {
        this->type = type; 
        this->density = density; 
        this->solid = solid;
    }

    Particle() {

    }
};


std::vector<std::vector<Particle>> particles(100, std::vector<Particle>(100)); 


int main(){

    srand(time(NULL)); 

    init();

    loop(); 

    glfwTerminate();

    return 0;
}


void loop() {

    while (!glfwWindowShouldClose(window)) {

        int temp = sync(); 
        if (temp != -1) {
            std::cout << temp << std::endl; 
        }

        events(); 

        update(); 

        draw(); 

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void events() {

    int pos[2];
    getCursorPos(pos);
    if (pos[0] > 0 && pos[0] < width && pos[1] > 0 && pos[1] < height) {
        
        if (glfwGetKey(window, GLFW_KEY_1) == GL_TRUE) {
            particles[pos[0] / 8][pos[1] / 8] = Particle("sand", 2, false);
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GL_TRUE) {
            particles[pos[0] / 8][pos[1] / 8] = Particle("water", 1, false);
        }
             
        if (glfwGetKey(window, GLFW_KEY_3) == GL_TRUE) {
            particles[pos[0] / 8][pos[1] / 8] = Particle("solid", 128, true);
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GL_TRUE) {
            particles[pos[0] / 8][pos[1] / 8] = Particle("none", 0, false);
        }


    }

}

void update() {
    for (int i = particles.size() - 2; i > 1; i--) {
        for (int i2 = particles[i].size() - 2; i2 > 2; i2--) {
            if (!particles[i][i2].solid) {
                if (particles[i][i2 + 1].density < particles[i][i2].density) {
                    std::swap(particles[i][i2 + 1], particles[i][i2]);
                    //particles[i][i2].futurePos[0] = i; 
                    //particles[i][i2].futurePos[1] = i2 + 1; 
                }
                else {
                    if (particles[i - 1][i2 + 1].density < particles[i][i2].density) {
                        std::swap(particles[i - 1][i2 + 1], particles[i][i2]);
                        //particles[i][i2].futurePos[0] = i - 1;
                        //particles[i][i2].futurePos[1] = i2 + 1;
                    }
                    else {
                        if (particles[i + 1][i2 + 1].density < particles[i][i2].density) {
                            std::swap(particles[i + 1][i2 + 1], particles[i][i2]);
                            //particles[i][i2].futurePos[0] = i + 1;
                            //particles[i][i2].futurePos[1] = i2 + 1;
                        }
                    }
                }


                if (particles[i][i2].type == "water") {
                    if (particles[i - 1][i2].type == "none" && particles[i + 1][i2].type == "none") {
                        if (rand() % 2 == 1) {
                            std::swap(particles[i + 1][i2], particles[i][i2]);
                        }
                        else {
                            std::swap(particles[i - 1][i2], particles[i][i2]);
                        }
                    }
                    else {
                        if (particles[i - 1][i2].type == "none") {
                            std::swap(particles[i - 1][i2], particles[i][i2]);
                        }
                        else {
                            if (particles[i + 1][i2].type == "none") {
                                std::swap(particles[i + 1][i2], particles[i][i2]);
                            }
                        }
                    }
                }
            }

        }
    }


}

void draw() {

    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < particles.size(); i++) {
        for (int i2 = 0; i2 < particles[i].size(); i2++) {
            if (particles[i][i2].type == "sand") {
                drawRect(i * 8, i2 * 8, 8, 8, 0.85, 0.7, 0.08);
            }
            else {
                if (particles[i][i2].type == "water") {
                    drawRect(i * 8, i2 * 8, 8, 8, 0.1, 0.43, 0.76);
                }
                else {
                    if (particles[i][i2].type == "solid") {
                        drawRect(i * 8, i2 * 8, 8, 8, 1.0, 1.0, 1.0);
                    }
                }
            }
        }
    }

    glfwSwapBuffers(window);
}

void drawRect(const float x, const float y, const float w, const float h, const float r, const float g, const float b) {
    glBegin(GL_QUADS); 
        glColor4f(r, g, b, 1.0);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd(); 
}

void getCursorPos(int(&a)[2]) {

    double x;
    double y;

    glfwGetCursorPos(window, &x, &y);

    a[0] = (int)x;
    a[1] = (int)y;
}

void init() {

    /* Initialize the library */
    if (!glfwInit()) {
        throw "Failed to initialize";
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        throw "Failed to create window"; 
    }


    /* Make the window's context current */
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    glewInit();

    glOrtho(0.f, width, height, 0.f, 0.f, 1.f);

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
}

int sync() {

    static int frames = 0;

    static unsigned int startTime = clock();


    if (clock() - startTime > 1000) {
        startTime = clock();
        int f = frames;
        frames = 0;
        return f;
    }


    frames++;

    return -1;
}