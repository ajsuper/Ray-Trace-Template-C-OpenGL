#include </usr/include/c++/13/fstream>
#include </usr/include/c++/13/sstream>
#include </usr/include/c++/13/iostream>
#include </usr/include/GL/glew.h>
#include </usr/include/GL/freeglut.h>
#include </usr/include/c++/13/math.h>
#include </usr/include/c++/13/vector>

//Initialize our shaderProgram
GLuint shaderProgram;

float timeValue = 0.0f;
float movementSpeed = 0.1f;
struct camera {
    float position[3] = {0, 0, 0};
    float direction[3] = {1, 0, 0};
    float up[3] = { 0, 1, 0 };
} cam;


void defineTriangles() {
    glBegin(GL_TRIANGLES);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();
}


void reinitializeEachFrame() {
    glUseProgram(shaderProgram);
    timeValue = timeValue + 0.05;
}


void passValues() {
    GLint resolutionLoc = glGetUniformLocation(shaderProgram, "resolution");
    float res[2] = { static_cast<GLfloat>(glutGet(GLUT_WINDOW_WIDTH)), static_cast<GLfloat>(glutGet(GLUT_WINDOW_HEIGHT)) };
    glUniform2fv(resolutionLoc, 1, res);

    GLint timeLocation = glGetUniformLocation(shaderProgram, "time");
    if (timeLocation != -1) {
        glUniform1f(timeLocation, (timeValue / 2000));
    }

    GLint cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
    GLint cameraDirLoc = glGetUniformLocation(shaderProgram, "cameraDir");
    glUniform3fv(cameraPosLoc, 1, cam.position);
    glUniform3fv(cameraDirLoc, 1, cam.direction);
    
}


void keys(unsigned char key, int x, int y) {
    float right[3];
    // Cross product
    right[0] = cam.up[1] * cam.direction[2] - cam.up[2] * cam.direction[1];
    right[1] = cam.up[2] * cam.direction[0] - cam.up[0] * cam.direction[2];
    right[2] = cam.up[0] * cam.direction[1] - cam.up[1] * cam.direction[0];
    // Normalize
    float norm = sqrt(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
    right[0] /= norm;
    right[1] /= norm;
    right[2] /= norm;
    float up[3];
    up[1] = 1;
    switch (key) {
        //Direction
        case 'q':
            cam.direction[0] = cam.direction[0] * cos(-movementSpeed) - cam.direction[2] * sin(-movementSpeed);
            cam.direction[2] = cam.direction[0] * sin(-movementSpeed) + cam.direction[2] * cos(-movementSpeed);
            break;
        case 'e':
            cam.direction[0] = cam.direction[0] * cos(movementSpeed) - cam.direction[2] * sin(movementSpeed);
            cam.direction[2] = cam.direction[0] * sin(movementSpeed) + cam.direction[2] * cos(movementSpeed);
            break;
        // Position
        case 'w':
            cam.position[0] += cam.direction[0] * movementSpeed;
            cam.position[1] += cam.direction[1] * movementSpeed;
            cam.position[2] += cam.direction[2] * movementSpeed;
            break;
        case 'a':
            cam.position[0] += right[0] * movementSpeed;
            cam.position[1] += right[1] * movementSpeed;
            cam.position[2] += right[2] * movementSpeed;
            break;
        case 's':
            cam.position[0] -= cam.direction[0] * movementSpeed;
            cam.position[1] -= cam.direction[1] * movementSpeed;
            cam.position[2] -= cam.direction[2] * movementSpeed;
            break;
        case 'd':
            cam.position[0] -= right[0] * movementSpeed;
            cam.position[1] -= right[1] * movementSpeed;
            cam.position[2] -= right[2] * movementSpeed;
            break;      
    }
}


std::string loadShaderSource(const char* filepath) {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(filepath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return "";
    }
}


void compileAndUseProgramShader() {
    //Get shader coad
    std::string vertexCode = loadShaderSource(R"(./shaders/vertexShader.glsl)");
    std::string fragmentCode = loadShaderSource(R"(./shaders/fragmentShader.glsl)");

    //Convert it to a string
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    // Define the vertexShaderSource as a GL Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Define the fragmentShaderSource as a GL Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Combine the seperate shaders into the one shader shaderProgram
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Now that we have shaderProgram we no longer need each seperate shader so we can delete them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void display(void)
{
    static float last_time = 0;
    static float fps_start = 0;
    float current_time = glutGet(GLUT_ELAPSED_TIME);
    if (last_time > 0) {
        float frame_time = current_time - last_time;
        printf("%.2f fps\n", (1000.0f/frame_time));
    }

    last_time = current_time;
    reinitializeEachFrame();
    passValues();
    defineTriangles();
    glFinish();
    glutPostRedisplay();
}


int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowPosition(1, 1);
    glutInitWindowSize(100, 100);
    glutCreateWindow("RayTracer!");
    glewInit();
    compileAndUseProgramShader();
    glutDisplayFunc(display);
    glutKeyboardFunc(keys);
    glutMainLoop();
    return 0;
}