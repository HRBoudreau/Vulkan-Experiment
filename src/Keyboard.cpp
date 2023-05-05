#include <Keyboard.h>

bool KEY_A;
bool KEY_B;
bool KEY_C;
bool KEY_D;
bool KEY_E;
bool KEY_F;
bool KEY_G;
bool KEY_H;
bool KEY_I;
bool KEY_J;
bool KEY_K;
bool KEY_L;
bool KEY_M;
bool KEY_N;
bool KEY_O;
bool KEY_P;
bool KEY_Q;
bool KEY_R;
bool KEY_S;
bool KEY_T;
bool KEY_U;
bool KEY_V;
bool KEY_W;
bool KEY_X;
bool KEY_Y;
bool KEY_Z;
bool KEY_SPACE;
bool KEY_1;
bool KEY_2;
bool KEY_3;
bool KEY_4;
bool KEY_5;
bool KEY_6;
bool KEY_7;
bool KEY_8;
bool KEY_9;
bool KEY_0;
bool KEY_MINUS;
bool KEY_PLUS;
bool KEY_LEFT_BRACKET;
bool KEY_RIGHT_BRACKET;
bool KEY_SEMICOLON;
bool KEY_PIPE;
bool KEY_PARENTHESIS;
bool KEY_COMMA;
bool KEY_PERIOD;
bool KEY_TILDE;
bool KEY_LEFT_SHIFT;
bool KEY_RIGHT_SHIFT;
bool KEY_SHIFT;
bool KEY_CAPS;
bool KEY_TAB;
bool KEY_LEFT_CTRL;
bool KEY_RIGHT_CTRL;
bool KEY_CTRL;
bool KEY_LEFT_ALT;
bool KEY_RIGHT_ALT;
bool KEY_ALT;
bool KEY_MENU;
bool KEY_BACKSPACE;


bool leftMouseButtonPressed = false;
bool isMinimized = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    leftMouseButtonPressed = true;
}

void iconifyCallback(GLFWwindow* window, int iconified) {
    if ( iconified == true ) {
        isMinimized = true;
    }
    else {
        isMinimized = false;
    }
}

extern BufferData indexBufferStruct2;
extern BufferData vertexBufferStruct2;
bool made = false;

void InitKeyboard(InstanceData &instanceData) {
    
    glfwSetMouseButtonCallback(instanceData.windowData.glfwWindow, mouse_button_callback);
    glfwSetWindowIconifyCallback(instanceData.windowData.glfwWindow, iconifyCallback);
}

bool ProcessInputs(InstanceData &instanceData) {
    double xpos, ypos;
    glfwPollEvents();
    while ( isMinimized ) {
        glfwPollEvents();
        isMinimized = !glfwWindowShouldClose(instanceData.windowData.glfwWindow) & isMinimized;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_E ) == GLFW_PRESS ) {
        return true;
    }
    KEY_W = false;
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_W ) == GLFW_PRESS ) {
        KEY_W = true;
        if ( KEY_W ) {
            instanceData.camera.position += glm::vec4(.0f,.0f,-.01f,.0f)*instanceData.camera.rotation;
        }
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_S ) == GLFW_PRESS ) {
        instanceData.camera.position += glm::vec4(.0f,.0f,.01f,.0f)*instanceData.camera.rotation;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_A ) == GLFW_PRESS ) {
        instanceData.camera.position += glm::vec4(-0.01f,.0f,.0f,.0f)*instanceData.camera.rotation;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_D ) == GLFW_PRESS ) {
        instanceData.camera.position += glm::vec4(0.01f,.0f,.0f,.0f)*instanceData.camera.rotation;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_UP ) == GLFW_PRESS ) {
        instanceData.camera.position += glm::vec4(0.0f,-.005f,.0f,.0f)*instanceData.camera.rotation;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_DOWN ) == GLFW_PRESS ) {
        instanceData.camera.position += glm::vec4(0.0f,.005f,.0f,.0f)*instanceData.camera.rotation;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_LEFT ) == GLFW_PRESS ) {
        instanceData.camera.rotation = glm::rotate(instanceData.camera.rotation, glm::radians(.1f), glm::vec3(0.0f, -1.0f, 0.0f));
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_RIGHT ) == GLFW_PRESS ) {
        instanceData.camera.rotation = glm::rotate(instanceData.camera.rotation, glm::radians(.1f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_G ) == GLFW_PRESS && made == false) {
        Entity t = createEntity();
        addComponent<Mesh>(t);
        Mesh* m = getComponent<Mesh>(t);
        m->indices = indexBufferStruct2;
        m->vertices = vertexBufferStruct2;
        m->indicesCount = 12*3;
        made = true;
    }

    if ( leftMouseButtonPressed == true ) {
        glfwGetCursorPos(instanceData.windowData.glfwWindow, &xpos, &ypos);
        std::cout << "CURS: " << xpos << "  " << ypos << std::endl;
        leftMouseButtonPressed = false;
    }

    KEY_LEFT_SHIFT = false;
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS ) {
        KEY_LEFT_SHIFT = true;
    }
    KEY_RIGHT_SHIFT = false;
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_RIGHT_SHIFT ) == GLFW_PRESS ) {
        KEY_RIGHT_SHIFT = true;
    }

    KEY_LEFT_ALT = false;
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS ) {
        KEY_LEFT_ALT = true;
    }
    KEY_RIGHT_ALT = false;
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS ) {
        KEY_RIGHT_ALT = true;
    }

    KEY_LEFT_CTRL = false;
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS ) {
        KEY_LEFT_CTRL = true;
    }
    KEY_RIGHT_CTRL = false;
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_RIGHT_CONTROL ) == GLFW_PRESS ) {
        KEY_RIGHT_CTRL = true;
    }

    KEY_SHIFT = KEY_LEFT_SHIFT | KEY_RIGHT_SHIFT;
    KEY_ALT = KEY_RIGHT_ALT | KEY_LEFT_ALT;
    KEY_CTRL = KEY_LEFT_CTRL | KEY_RIGHT_CTRL;

    return false | glfwWindowShouldClose(instanceData.windowData.glfwWindow);
}
