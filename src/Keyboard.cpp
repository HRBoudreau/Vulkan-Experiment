#include <Keyboard.h>

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

    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_W ) == GLFW_PRESS ) {
        instanceData.camera.position[2] += -0.01f;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_S ) == GLFW_PRESS ) {
        instanceData.camera.position[2] += 0.01f;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_A ) == GLFW_PRESS ) {
        instanceData.camera.position[0] += -0.01f;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_D ) == GLFW_PRESS ) {
        instanceData.camera.position[0] += 0.01f;
    }
    if ( glfwGetKey(instanceData.windowData.glfwWindow, GLFW_KEY_G ) == GLFW_PRESS && made == false) {
        Entity t = createEntity();
        addComponent<Mesh>(t);
        Mesh* m = getComponent<Mesh>(t);
        m->indices = indexBufferStruct2;
        m->vertices = vertexBufferStruct2;
        m->indicesCount = 12;
        made = true;
    }

    if ( leftMouseButtonPressed == true ) {
        glfwGetCursorPos(instanceData.windowData.glfwWindow, &xpos, &ypos);
        std::cout << "CURS: " << xpos << "  " << ypos << std::endl;
        leftMouseButtonPressed = false;
    }
    return false | glfwWindowShouldClose(instanceData.windowData.glfwWindow);
}
